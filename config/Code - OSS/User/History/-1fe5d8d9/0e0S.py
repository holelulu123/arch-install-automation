import os
from dotenv import load_dotenv

# Load the correct .env file based on the environment
environment = os.getenv("ENVIRONMENT", "prod") # Default to 'dev'
if environment == "prod":
    load_dotenv(".env.prod")
else:
    load_dotenv(".env.dev")

timeout = 30
workers = os.getenv("workers")
bind = os.getenv("IP")
ALPACA_KEY_ID_TRADING_API_PAPER = os.getenv('ALPACA_KEY_ID_TRADING_API_PAPER')
ALPACA_SECRET_KEY_TRADING_API_PAPER = os.getenv('ALPACA_SECRET_KEY_TRADING_API_PAPER')
authentication_database_path = "database/authentication.db"
stocks_database_path = "database/stocks.db"
trades_database_path = "database/trades.db"
