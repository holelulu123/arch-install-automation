from src import stocks_db
from src import trades_db
from logger import logger
import sqlite3
import globals_variables
import requests
from src import alpaca_requests
import time

x = stocks_db.fetch_model_predictions('2025-01-16', '2025-01-16', 1, 5)
print(x)
# alpaca_requests.delete_all_orders()
# orders = trades_db.fetch_trade_history()
# print(orders)

# orders = alpaca_requests.get_orders(status='open')    

    
# print(len(rows))
# conn.commit()
