import os
import numpy as np
import pandas as pd
from logger import logger
import globals_variables
import json
from src import api_requests
from datetime import timedelta, datetime
ss
def cyclical_encode_hour(hour,minute):
    '''
    Encodes the time of the day as a Cyclical Encoding

    hour -> hour of the day, number from 0 -> 23
    minute -> minute of the day, number from 0 -> 59 

    outputs sin and cos point which represent the time of the day   
    '''
    total_minute = (hour * 60) + minute
    time_sin = np.sin(2 * np.pi * total_minute / 1440)
    time_cos = np.cos(2 * np.pi * total_minute / 1440)

    return time_sin, time_cos

def cyclical_encode_weekday(weekday):
    '''
    Encodes the day of the week as a Cyclical Encoding

    weekday -> day of the month, number from 0 -> 6 

    outputs sin and cos point which represent the day   
    '''

    weekday_sin = np.sin(2 * np.pi * weekday / 7)
    weekday_cos = np.cos(2 * np.pi * weekday / 7)
    return weekday_sin, weekday_cos

def cyclical_encode_monthday(day_of_month):    
    '''
    Encodes the day of the month as a Cyclical Encoding

    day_of_month -> day of the month, number from 1 -> 31 

    outputs sin and cos point which represent the day of the month  
    '''
    monthday_index = day_of_month - 1

    monthday_sin = np.sin(2 * np.pi * monthday_index / 31)
    monthday_cos = np.cos(2 * np.pi * monthday_index / 31)
    return monthday_sin, monthday_cos

def cyclical_encode_month(month):
    '''
    Encodes the month as a Cyclical Encoding

    month -> month of the year, number from 1 -> 12 

    outputs sin and cos point which represent the month   
    '''
    month_index = month - 1

    month_sin = np.sin(2 * np.pi * month_index / 12)
    month_cos = np.cos(2 * np.pi * month_index / 12)
    return month_sin, month_cos

def statistics_run(ticker: str, actual_values: list, predicted_values: list):
    '''
    The function takes a list of actual values of the prices of tickers, and 
    the predicted values that the model evaluated (which are both on the same time domain)
    and output a statistics outputs the deseribe how good the model works on that ticker.

    Parametes:
    ----------
    Ticker : String
        The ticker that statistics evaluating on.
    actual_values: List
        List of values that represent the Actual Close price of the ticker in the time domain.

    predicted_values: List
        List of values that represent the predicted Close price of the ticker in the time domain. 
    
    Returns:
    ----------

    net_profit : Float
        Sums all the actual profits and losses of the ticker
    profits : Int 
        total number of profits that the model evaluated on this ticker
    losses : Int
        total number of losses that the model evaluated on this ticker
    total : Int
        Total number of days that the statistics looks for.
    diff : Float
        Number that represent the total difference between the actual and predicted values.
        The lower the better (Means the predictions is very close to the actual prices)
    '''
    net_income = []
    profits = 0
    losses = 0
    total = 0 
    if len(actual_values) == len(predicted_values) and len(actual_values) != 0:
        for i in range(1,len(actual_values)):
            total += 1
            if predicted_values[i] > actual_values[i-1] and actual_values[i] > actual_values[i-1]: # Profit - Predicted Higher | Actual Was Higher - Buy
                net_income.append((actual_values[i] - actual_values[i-1]) / actual_values[i-1] * 100)
                profits += 1

            elif predicted_values[i] > actual_values[i-1] and actual_values[i] < actual_values[i-1]: # Loss - Predicted Higher | Actual Was Lower - Buy 
                net_income.append((actual_values[i] - actual_values[i-1]) / actual_values[i-1] * 100)
                losses += 1
            
        normalization_factor = max(np.max(np.abs(predicted_values)),np.max(np.abs(actual_values)))
        predicted_values /= normalization_factor 
        actual_values /= normalization_factor
        diff = np.average(np.abs(predicted_values - actual_values))
        net_profit = np.sum(net_income)
        return net_profit, profits, losses, total, diff
    else:
        return None, None, None, None, None

def get_model_data(from_date: str, to_date: str):
    '''
    This function is Wrapper for polygon API call, it outputs dictionary that represent the stocks data within 
    these dates.
    
    Parameters
    ----------
    from_date: string
        String that formatted as YYYY-MM-DD that represent the starting date to collect the data.
    to_date: string
        String that formatted as YYYY-MM-DD that represent the ending date to collect the data.
    
    Returns:
    ----------
        data: dictionary
            Dictionary that represent the stocks data, Keys are Tickers names, values are list of prices within these dates. 
    '''
    # Open the file that contains all our tickers enocoding
    if os.path.exists(globals_variables.tickers_file_path):
        with open(globals_variables.tickers_file_path, "r") as f:
            tickers = json.load(f)['T']
        data = {key: [[] for _ in range(9)] for key in tickers}
    # Convert strings to datetime objects
    start = datetime.strptime(from_date, "%Y-%m-%d")
    end = datetime.strptime(to_date, "%Y-%m-%d")

    # For loop on all the dates between our need.
    for i in range((end - start).days + 1):
        current_date = (start + timedelta(days=i))
        formatted_date = current_date.strftime('%Y-%m-%d')
        response = api_requests.polygon_daily_group(by_date=formatted_date)
        
        if response:
            weekday_sin, weekday_cos = cyclical_encode_weekday(current_date.weekday())
            monthday_sin, monthday_cos = cyclical_encode_monthday(current_date.day)
            month_sin, month_cos = cyclical_encode_month(current_date.month)
            
            for value in response['results']:
                if value['T'] in tickers:
                    data[value['T']][0].append(value['c'])
                    data[value['T']][1].append(value['v'])
                    data[value['T']][2].append(weekday_sin) 
                    data[value['T']][3].append(weekday_cos)
                    data[value['T']][4].append(monthday_sin)
                    data[value['T']][5].append(monthday_cos)
                    data[value['T']][6].append(month_sin)
                    data[value['T']][7].append(month_cos)
                    data[value['T']][8].append(formatted_date)

        else:
            logger.info(f'response: {response}')
            if from_date == to_date:
                return False 
    return data

def model_periodic_statistics(from_date: str, to_date: str):
    '''
    Fetches Predictions Data from server and evaluate the model for period of time

    Parameters: 
    -----------
    from_date: String
        Date formatted as YYYY-MM-DD and represent the start of fetching model predictions
    to_date: String
        Date formatted as YYYY-MM-DD and represent the end of fetching model predictions
    
    Returns:
    ---------
    flag: Bool
    True if opertion succeed, False if not
    '''
    performance = {}
    # Get the predictons for the speicific period of time
    data = api_requests.get_model_predictions(from_date, to_date)
    if data:
        for key,value in data.items():
            if None in value[1] or None in value[3]:
                continue # Skips faulty stocks
            net_profit, profits, losses, total, diff = statistics_run(key, actual_values=value[3], predicted_values=value[1])
            if not net_profit:
                continue
            performance[key] = [net_profit, profits, losses, total, diff]
    
        flag = api_requests.post_model_statistics(from_date, to_date, performance)
        if flag:
            return True
    return False

def generate_recommended_stocks():
    '''
    Function generates the New recommended stocks, It receives all the statistics of all times, does an algorithm 
    that that marks every ticker with grade. later when we need to buy stocks, we buy by the grade.

    '''
    data = api_requests.get_model_statistics()
    output = {}
    for key,value in data.items():
        count = sum(1 for x in value[0] if x > 0)
        output[key] = [np.sum(value[0]), np.average(value[0]), count]
    # Post request of this.
    flag = api_requests.post_reccommended_stocks(output)
    if flag:
        return True
    return False
