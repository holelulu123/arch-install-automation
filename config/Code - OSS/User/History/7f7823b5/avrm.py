import math 
import time
from datetime import datetime
from src import stocks_db
from src import alpaca_requests
from logger import logger

def string_to_bool(bool_str : str):
    '''
    This function converts string into boolean

    Parameters: 
    -----------
    bool_str: str
        string that is either 'true' or 'false'
    Returns:
    -----------
    bool:
        True or False depends on the input
    '''
    if bool_str.lower() == 'true':
        return True
    elif bool_str.lower() == 'false':
        return False


def daily_stocks_generation(data, config):
    ''' 
    This function takes Daily Model predictions, correlates it with recommended stocks and
    calculate the quantities of each stock to buy and the total daily investment

    Parameters:
    ------------
    data : dictionary
        data contains model predictions, 
        format e.g -> {'ticker': [[last_price], [predicted_price], [predicted_precent], [actual_price], [actual_precent], [date]] }
    config: dictionary
        dictionary cotains all the user trading configuration data 
        format e.g -> {"min_range":1, "max_range":5, "daily_invest":1000, "enable":True}
            daily_invest -> the total investment in this function needs to be closed to this number here.
    Returns:
    ------------
    trade_data : dictionary
        dict that contains all the stocks and quantities to buy.
    '''
    # Fetch the recommended
    reccommended_stocks = stocks_db.fetch_recommended_stocks()
    if not reccommended_stocks:
        return False
    # Sort by count_profit and take the first 1000
    sorted_items = sorted(reccommended_stocks.items(), key=lambda item: (item[1][2], item[1][0]), reverse=True)
    result = dict(sorted_items[:500])
    trade_data = {}
    total_investment = 0
    max_price = 0
    # This first loop is to calculate the Max Price.
    for key, value in data.items():
        if key in result:
            temp = value[0][0]
            max_price =  max(temp, max_price)
    # Second loop for calculating the stats
    for key, value in data.items():
        if key in result:
            try:
                if value[2][0] > 0: # Check if prediction precent is bigger than 0 
                    stocks = int(math.floor(max_price / value[0][0]))
                    total_investment += stocks * value[0][0]
                    trade_data[key] = [value[0][0], stocks] # [price, quantity]
            except:
                pass
    
    # here the stocks count and 
    division = math.floor(config['daily_invest'] / total_investment) 
    if not division:
        for key in trade_data:
            trade_data[key][1] *= division
    
    return trade_data

def equities_purchase(trade_data: dict):
    '''
    This function get all the stocks and quantites and execute orders
    with the use of API Calls.

    Parameters:
    -------------
    trade_data: dict
        keys are the symbols and values are the price and quantities

    Returns:
    -------------
    receipt: dict
        receipt containing all the return data from the execution of the order.
    '''
    # Needs to check here if i have enough buy power, (alpaca_requests.get_account_details)
    
    receipt = {}
    for key, value in trade_data.items():
        qty = value[1]
        symbol = key
        order = alpaca_requests.execute_order(symbol, qty, 'buy')
        if order:
            order_id = order['id']
            date = order['submitted_at']
            dt = datetime.fromisoformat(date.replace('Z', '+00:00'))
            # Format it to the desired output
            formatted_time = dt.strftime("%Y-%m-%d")
            side = order['side']
            receipt[key] = [key, order_id, side, value[0], qty, formatted_time] # [Symbol, order_id, side, last_price, quantity, date->'YYYY-MM-DD_hhmmss']
        else:
            logger.info(f'Cant buy stock for ticker: {key}')
            pass
    
    return receipt

def equities_sell(open_positions: list):
    '''
    This function takes data from the trade_history and sell the relevant equities

    Parameters:
    ------------
    open_positions: dict
        contains the relevant data for selling the stocks, quantity symbol and etc... 
    '''
    receipt = {}
    for value in open_positions:
        qty = value['qty']
        side = value['side']
        symbol = value['symbol']
        if side == 'long':
            order = alpaca_requests.execute_order(symbol, qty, 'sell')
            if order:
                order_id = order['id']
                date = order['submitted_at']
                dt = datetime.fromisoformat(date.replace('Z', '+00:00'))
                # Format it to the desired output
                formatted_time = dt.strftime("%Y-%m-%d")
                side = order['side']
                receipt[symbol] = [symbol, order_id, side, value['current_price'], qty, formatted_time] # [Symbol, order_id, side, last_price, quantity, date->'YYYY-MM-DD_hhmmss']
            else:
                logger.info(f'Cant sell stock for ticker: {symbol}')
                pass
    flag = False
    while not flag:
        orders = alpaca_requests.all_open_positions()
        size = len(orders)
        logger.info(f'Size of orders is: {size}')
        if size:
            time.sleep(0.5)
        else:
            flag = True
            logger.info(f"Positions are clear, All stocks are sold.")
    return receipt