import os
import logger
import json
import torch
from dotenv import load_dotenv

#------ Pathes of files -------#
tickers_file_path = 'jsons/tickers_sectors_industries.json'
tickers_encoding_path = 'jsons/tickers_encoding.json'
sector_mapping_path = 'jsons/sector_mapping.json'
industry_mapping_path = 'jsons/industry_mapping.json'

# Data configuration
input_matrix_columns = 8
input_matrix_rows = 20
output_matrix_columns = 1
output_matrix_rows = 1

# Model configuration
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
batch_size = 512
input_size = 8
output_size = 1
num_lstm_layers = 2
lstm_size = 32
dropout = 0.2
extra_slots_ticker = 1000
extra_slots_sector = 10
extra_slots_industry = 100
embedding_dim = 16
model_categories = 3
os.makedirs('jsons',exist_ok=True)

new_stocks_update_frequnecy = 60 # In Days 

if os.path.exists(sector_mapping_path):
    with open(sector_mapping_path, "r") as f:
        sector_size = len(json.load(f))

else:
    logger.error(f"Sector mapping file doesnt Exist")
    raise ValueError

if os.path.exists(industry_mapping_path):
    with open(industry_mapping_path, "r") as f:
        industry_size = len(json.load(f))

else:
    logger.error(f"Industry mapping file doesnt Exist")
    raise ValueError

if os.path.exists(tickers_encoding_path):
    with open(tickers_encoding_path, "r") as f:
        ticker_size = len(json.load(f))

else:
    logger.error(f"Industry mapping file doesnt Exist")
    raise ValueError

# Load the correct .env file based on the environment
environment = os.getenv("ENVIRONMENT", "dev") # Default to 'dev'
if environment == "prod":
    load_dotenv(".env.prod")
else:
    load_dotenv(".env.dev")

server_ip = os.getenv("server_ip")
POLYGON_API_TOKEN = os.getenv("POLYGON_API_TOKEN")
