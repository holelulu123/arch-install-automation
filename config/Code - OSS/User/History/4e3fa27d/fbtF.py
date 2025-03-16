import os
from dotenv import load_dotenv
load_dotenv()

variables = {
    "receiver_coordination_1": os.getenv("receiver_coordination_1"),
    "receiver_coordination_2": os.getenv("receiver_coordination_2"),
    "receiver_coordination_3": os.getenv("receiver_coordination_3"),
    "max_distance": os.getenv("max_distance"),
    "fs": os.getenv("fs"),
    "time": os.getenv("time"),
    "chunk_time": os.getenv("chunk_time"),
    "batch_size": os.getenv("batch_size"),
    "windows_per_batch": os.getenv("windows_per_batch"),
    "device_mapping_path": os.getenv("device_mapping_path"),
    "identification_checkpoint_path": os.getenv("identification_checkpoint_path"),
    "classification_checkpoint_path": os.getenv("classification_checkpoint_path", "src/model/classification_best_model.pth"),
    "num_windows": int(os.getenv("num_windows")),
    "bind": os.getenv("bind"),
    "workers": os.getenv("workers"),
    "timeout": os.getenv("timeout")
}

# Check for empty variables
empty_vars = [key for key, value in variables.items() if not value]
if empty_vars:
    raise ValueError(f"Empty Variables: {', '.join(empty_vars)}. Recheck that all variables are filled.")
