import os
from src.logger import logging_config 
import yaml

# Ensure the logs directory exists
if not os.path.exists('logs'):
    os.makedirs('logs')

# Load the logging configuration from the YAML file
with open('logging_config.yaml', 'r') as file:
    config = yaml.safe_load(file)
    logging.config.dictConfig(config)

# Example logger usage
logger = logging.getLogger('app')
logger.info("Logging setup complete!")
