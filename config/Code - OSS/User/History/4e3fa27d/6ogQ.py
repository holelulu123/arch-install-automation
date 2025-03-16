import os
from dotenv import load_dotenv
load_dotenv()

configs_list = list()
# Receivers coordinates
receiver_coordination_1 = os.getenv("receiver_coordination_1")
configs_list.append(os.getenv("receiver_coordination_1"))

receiver_coordination_2 = os.getenv("receiver_coordination_2"), 
configs_list.append(os.getenv("receiver_coordination_2"))

receiver_coordination_3 = os.getenv("receiver_coordination_3"), 
configs_list.append(os.getenv("receiver_coordination_3"))

# TDoA Parameters
max_distance = os.getenv("max_distance")
configs_list.append(os.getenv("max_distance"))

fs = os.getenv("fs")
configs_list.append(os.getenv("fs")) # Hz

time = os.getenv("time")
configs_list.append(os.getenv("time")) # Second

chunk_time = os.getenv("chunk_time")
configs_list.append(os.getenv("chunk_time")) # Second

# Model Parameters
batch_size = os.getenv("batch_size")
configs_list.append(os.getenv("batch_size"))

windows_per_batch = os.getenv("windows_per_batch")
configs_list.append(os.getenv("windwos_per_batch"))

device_mapping_path = os.getenv("device_mapping_path")
configs_list.append(os.getenv("device_mapping_path"))

identification_checkpoint_path = os.getenv("identification_checkpoint_path")
configs_list.append(os.getenv("identification_checkpoint_path"))

classification_checkpoint_path = os.getenv("src/model/classification_best_model.pth")
configs_list.append(os.getenv("classification_checkpoint_path"))

num_windows = os.getenv("num_windows")
configs_list.append(os.getenv("num_windows"))

# Gunicorn Parameters
bind = os.getenv("bind")
configs_list.append(os.getenv("bind")) # ip + port

workers = os.getenv("workers")
configs_list.append(os.getenv("workers")) # workers count

timeout = os.getenv("timeout")
configs_list.append(os.getenv("timeout"))  # timeout of a individual request

if None in configs_list:
    raise ValueError