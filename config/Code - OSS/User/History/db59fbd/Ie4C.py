
from src import api_requests
import time
from datetime import datetime 
from main import bi_monthly_stocks_update


x = api_requests.get_date_model_predictions()
print(x)