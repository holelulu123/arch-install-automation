import torch
from src.model.model import SignalCNN2D_STFT_Forward
import numpy as np
import json 
from concurrent.futures import ThreadPoolExecutor
import time
import global_variables

class Live_Inference:
    def __init__(self):
        self.BATCH_SIZE = global_variables.variables["batch_size"]
        self.WINDOWS_PER_BATCH = global_variables.variables["windows_per_batch"]
        self.DEVICE_MAPPING = global_variables.variables["device_mapping_path"]
        
        self.IDENTIFICATION_CHECKPOINT_PATH = global_variables.variables["identification_checkpoint_path"]
        self.CLASSIFICATION_CHECKPOINT_PATH = global_variables.variables["classification_checkpoint_path"]

        self.NUM_WINDOWS = global_variables.variables["num_windows"]
        
        self.DEVICE = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
        _ = torch.empty((1,), device=self.DEVICE)

        self.identification_model = self.load_model(self.IDENTIFICATION_CHECKPOINT_PATH).to(self.DEVICE)
        self.classification_model = self.load_model(self.CLASSIFICATION_CHECKPOINT_PATH).to(self.DEVICE)
        
        self.batch_tensor = []

    def load_model(self, checkpoint_path: str):
        checkpoint = torch.load(checkpoint_path, map_location=self.DEVICE)
        # get the number of classes trained on
        num_classes = len(checkpoint['model_state_dict']['classifier.6.bias'])
        
        model = SignalCNN2D_STFT_Forward(307200, num_classes)
        model.load_state_dict(checkpoint['model_state_dict']) 

        return model.eval()

    def split_recording(self, inputs):
        print(type(self.NUM_WINDOWS))
        window_size =  len(inputs) // self.NUM_WINDOWS
        windows = [inputs[i * window_size:(i +1) * window_size] for  i in range(self.NUM_WINDOWS)]
        return windows

    def normalize_batch(self, batch: torch.Tensor) -> torch.Tensor:
        """Normalize complex batch by magnitude while preserving phase."""
       
        magnitude = torch.abs(batch)
        magnitude = torch.clip(magnitude, 0, 10)
                
        mag_mean = magnitude.mean(dim=-1, keepdim=True)
        mag_std = torch.clamp(magnitude.std(dim=-1, keepdim=True), min=1e-6)
                
        norm_factor = (magnitude - mag_mean) / mag_std
        normalized = torch.where(
            magnitude == 0,
            torch.zeros_like(batch),
            batch * (norm_factor / magnitude)
        )
        return normalized
            
    def map_label(self, label_idx: int)-> str:
        
        with open (self.DEVICE_MAPPING) as file:
            device_mapping = json.load(file)
        
        drone_name = device_mapping['idx_to_device'].get(str(label_idx), 'Unknown idx')
        return drone_name

    def get_windows(self, directory : str):
        '''
        Opens the file as a np.complex64 format and divide the file content into Chunks

        Parameters:
        -----------
        directory: str
            represent the location of the file
        Returns:
        -----------
        windows: list
            each value in the list is a chunk of the file, all values together 
            represent the total file content
        '''

        inputs = np.fromfile(directory, dtype =np.complex64)
        windows = self.split_recording(inputs)

        self.WINDOWS_PER_BATCH = int(len(windows) / self.BATCH_SIZE)

        return windows

    def identify(self, batch_windows):
        self.batch_tensor = torch.stack([torch.from_numpy(window) for window in batch_windows]).to(self.DEVICE)
        self.batch_tensor = self.normalize_batch(self.batch_tensor)
        
        with torch.no_grad():
            output = self.identification_model(self.batch_tensor)
        
        identified = torch.argmax(output, dim =1)
        return identified

    def classify(self, detected_indices):
         # if didn't detect anything in the batch, return 
        if detected_indices.numel() == 0:
            return ["None"] * self.WINDOWS_PER_BATCH
        
        detected_windows = self.batch_tensor[detected_indices]
        
        with torch.no_grad():
            output = self.classification_model(detected_windows)
       
        classified_indices = torch.argmax(output, dim =1)
        drone_labels = [self.map_label(idx.item() +1 ) for idx in classified_indices]
        results = np.array(["None"] * self.WINDOWS_PER_BATCH, dtype = object)
        results[detected_indices.cpu().numpy()] = drone_labels

        return results
    
    def process_batches(self, batch_windows):
        identified = self.identify(batch_windows)

        detected_indices = (identified != 0).nonzero(as_tuple=True)[0]

        classified = self.classify(detected_indices)

        return classified

    def warmup_model(self, input_shape =[100,307200], num_iterations = 100):
        real = torch.randn(input_shape, device = self.DEVICE)
        imag = torch.randn(input_shape, device = self.DEVICE)
        dummy_input = torch.complex(real, imag)

        if any(p.dtype == torch.complex64 for p in self.identification_model.parameters()):
            dummy_input = dummy_input.half()
        
        with torch.no_grad():
            for _ in range(num_iterations):
                _ = self.identification_model(dummy_input)

        real = torch.randn(input_shape, device = self.DEVICE)
        imag = torch.randn(input_shape, device = self.DEVICE)
        dummy_input = torch.complex(real, imag)
        if any(p.dtype == torch.complex64 for p in self.classification_model.parameters()):
                    dummy_input = dummy_input.half()

        with torch.no_grad():
            for _ in range(num_iterations):
                _ = self.classification_model(dummy_input)

    def main(self, directory : str):
        '''
        Main function of the Model
        Parameters:
        -----------
        directory: str
            Represent the location of the file to evaluate.
        
        Returns:
        -----------
        results: Dictionary
            represent the Key:Label that the model evaluated
        '''
        windows = self.get_windows(directory)
        batches = [windows[i:i + self.WINDOWS_PER_BATCH] for i in range(self.BATCH_SIZE)]
      
        # self.warmup_model()
        
        with ThreadPoolExecutor() as executor:
            results = list(executor.map(self.process_batches, batches))
        return results
        
