import os
from dotenv import load_dotenv
load_dotenv()

def get_env_var(name, default=None, cast_type=str):
    value = os.getenv(name, default)
    if value is not None:
        try:
            return cast_type(value)
        except ValueError:
            raise ValueError(f"Invalid type for {name}. Expected {cast_type.__name__} but got '{value}'")
    return value

configs = {
    "receiver_coordination_1": get_env_var("receiver_coordination_1"),
    "receiver_coordination_2": get_env_var("receiver_coordination_2"),
    "receiver_coordination_3": get_env_var("receiver_coordination_3"),
    "max_distance": get_env_var("max_distance", cast_type=float),
    "fs": get_env_var("fs", cast_type=int),
    "time": get_env_var("time", cast_type=float),
    "chunk_time": get_env_var("chunk_time", cast_type=float),
    "batch_size": get_env_var("batch_size", cast_type=int),
    "windows_per_batch": get_env_var("windows_per_batch", cast_type=int),
    "device_mapping_path": get_env_var("device_mapping_path"),
    "identification_checkpoint_path": get_env_var("identification_checkpoint_path"),
    "classification_checkpoint_path": get_env_var("classification_checkpoint_path", "src/model/classification_best_model.pth"),
    "num_windows": get_env_var("num_windows", cast_type=int),
    "bind": get_env_var("bind"),
    "workers": get_env_var("workers", cast_type=int),
    "timeout": get_env_var("timeout", cast_type=int)
}

# Check for missing variables
empty_vars = [key for key, value in configs.items() if value is None]
if empty_vars:
    raise ValueError(f"Empty Variables: {', '.join(empty_vars)}. Recheck that all variables are filled.")
