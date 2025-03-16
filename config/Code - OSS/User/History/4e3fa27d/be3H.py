import os
from dotenv import load_dotenv
load_dotenv()

configs_list = list()
# Receivers coordinates
receiver_coordination_1, configs_list = os.getenv("receiver_coordination_1")
configs_list.append(os.getenv("receiver_coordination_1"))
receiver_coordination_2, config_list = os.getenv("receiver_coordination_2"), 
configs_list.append(os.getenv("receiver_coordination_2"))
receiver_coordination_3, config_list = os.getenv("receiver_coordination_3"), 
configs_list.append(os.getenv("receiver_coordination_3"))

# TDoA Parameters
max_distance, config_list = os.getenv("max_distance"), configs_list.append(os.getenv("max_distance"))
fs, config_list = os.getenv("fs"), configs_list.append(os.getenv("fs")) # Hz
time, config_list = os.getenv("time"), configs_list.append(os.getenv("time")) # Second
chunk_time, config_list = os.getenv("chunk_time"), configs_list.append(os.getenv("chunk_time")) # Second

# Model Parameters
batch_size, config_list = os.getenv("batch_size"), configs_list.append(os.getenv("batch_size"))
windows_per_batch, config_list = os.getenv("windows_per_batch"), configs_list.append(os.getenv("windwos_per_batch"))
device_mapping_path, config_list = os.getenv("device_mapping_path"), configs_list.append(os.getenv("device_mapping_path"))
identification_checkpoint_path, config_list = os.getenv("identification_checkpoint_path"), configs_list.append(os.getenv("identification_checkpoint_path"))
classification_checkpoint_path, config_list = os.getenv("src/model/classification_best_model.pth"), configs_list.append(os.getenv("classification_checkpoint_path"))
num_windows, config_list = os.getenv("num_windows"), configs_list.append(os.getenv("num_windows"))

# Gunicorn Parameters
bind, config_list = os.getenv("bind"), configs_list.append(os.getenv("bind")) # ip + port
workers, config_list = os.getenv("workers"), configs_list.append(os.getenv("workers")) # workers count
timeout, config_list = os.getenv("timeout"), configs_list.append(os.getenv("timeout"))  # timeout of a individual request

print(config_list)