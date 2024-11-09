
from logger import logger
import globals
import torch 
import numpy as np
import json
from dataset import lstm_model
import os 
from torch.utils.data import DataLoader
import torch.optim as optim
import torch.nn as nn

def main():
    # Model initialization
    
    if os.path.exists(os.path.join('model','latest_model.txt')):
        logger.info(f'Loading your Model...')
        logger.info(f'The device Your model Work on is {globals.device}')

        model = lstm_model.LSTMModel(input_size=globals.input_size, hidden_layer_size=globals.lstm_size,num_layers=globals.num_lstm_layers, output_size=globals.output_size, dropout=globals.dropout)
        model = model.to(globals.device)       

        criterion = nn.MSELoss()
        optimizer = optim.Adam(model.parameters(), lr=globals.learning_rate, betas=(0.9, 0.98), eps=1e-9)
        
        #scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=globals.scheduler_step_size, gamma=0.1)     
        scheduler = optim.lr_scheduler.CosineAnnealingWarmRestarts(optimizer, T_0=globals.num_epoch, T_mult=globals.T_mult, eta_min=globals.minimum_lr)
        
        index_start = lstm_model.load_model(model,optimizer,scheduler)
        
        if not globals.model_training:
            index_start = 0
       
        if globals.model_statistics:
            index_start = 0
            # Initialize some kind of file to save statistics
            statistics_data = []
    
    else:
        logger.error(f'Your model cannot be found.')

    with open(globals.queue_file, 'r') as json_file:
        data = json.load(json_file)
    data_x = np.empty((len(data), globals.input_matrix_rows, globals.input_matrix_columns), dtype=np.float32)
    data_norm_sd = np.empty((len(data), globals.input_matrix_columns), dtype=np.float32)
    data_norm_mu = np.empty((len(data), globals.input_matrix_columns), dtype=np.float32)
    ticker_idx = np.empty((len(data),globals.input_matrix_rows,1))#,dataset_helpers.get_ticker_indexes(item)[0])
    sector_idx = np.empty((len(data),globals.input_matrix_rows,1))#,dataset_helpers.get_ticker_indexes(item)[1])
    industry_idx = np.empty((len(data),globals.input_matrix_rows,1))#,dataset_helpers.get_ticker_indexes(item)[2])
    for value in data.items():
        value[1] = np.float32(value[1]).transpose()

        scaler = lstm_model.Normalizer()
        normalized_data_x = scaler.fit_transform(value[1])
        data_x[i, :, :] = normalized_data_x
        data_norm_sd[i, :] = scaler.sd
        data_norm_mu[i, :] = scaler.mu
        


if __name__ == "__main__":
    main()