#! venv/bin/python3

from flask import Flask, request, jsonify
from bcrypt import checkpw, gensalt, hashpw
from src import authentication_db
from src import stocks_db
from src import secret_code_setup
from src import trades_db
from src import helpers
from src import alpaca_requests
import globals_variables
from datetime import datetime, timedelta
from logger import logger

app = Flask(__name__)

@app.route("/state/<int:chat_id>", methods=["GET"]) 
def get_state(chat_id):
    """
    Client-Side Sends GET Request from Server-side.
    Server-Side Fetch the state of the chat_id from the DB.
    """
    result = authentication_db.fetch_state(chat_id)
    if result:
        return jsonify({"state": result["state"][0]})


    logger.info(f"Didn't found the User: {chat_id}")
    return jsonify({"error": None}), 404

@app.route("/update_data", methods=["POST"])
def update_data():
    """
    This function is Responsible for the Communication Between Client-Side
    and Server-Side, Client-Server Send POST Request And Retrieve An answer After
    Querying the DB.

    """
    data = request.json
    state = list(data.keys())[0]
    value = list(data.values())[0]

    if state == "check_exist":

        if authentication_db.check_chat_id_exists(
            value, "users"
        ) and authentication_db.check_chat_id_exists(value, "states"):
            # change state of the id to login_username
            new_state = authentication_db.check_chat_id_exists(value, "states")
            return jsonify({new_state: False})

        # change state of the id to register_username
        authentication_db.add_states_db(value, "register_username")
        return jsonify({"register_username": False})

    if state == "register_username":
        chat_id = value[1]
        if not authentication_db.check_username_exists(value[0]):
            authentication_db.set_state(chat_id, "register_password")
            authentication_db.update_temp_data(chat_id, {"username": value[0]})
            return jsonify({"register_password": True})
        return jsonify({"register_username": False})

    if state == "register_password":
        chat_id = value[1]
        temp = authentication_db.get_temp_data(chat_id)
        password = hashpw(value[0].encode(), gensalt())
        temp["password"] = password.decode()
        authentication_db.update_temp_data(chat_id, temp)
        authentication_db.set_state(chat_id, "register_password_repeat")
        return jsonify({"register_password_repeat": True})

    if state == "register_password_repeat":
        chat_id = value[1]
        temp = authentication_db.get_temp_data(chat_id)
        password = value[0]
        password_hashed = temp["password"]
        if checkpw(password.encode(), password_hashed.encode()):
            authentication_db.set_state(chat_id, "secret_code")
            return jsonify({"secret_code": True})
        authentication_db.set_state(chat_id, "register_password")
        authentication_db.update_temp_data(chat_id, temp)
        return jsonify({"register_password_repeat": False})

    if state == "secret_code":
        chat_id = value[1]
        secret = secret_code_setup.show_secret_code()
        if checkpw(value[0].encode(), secret):
            authentication_db.set_state(chat_id, "logged_in")
            # remove data from temp and insert it into the database.
            creds = authentication_db.get_temp_data(chat_id)
            authentication_db.add_users_db(
                chat_id, creds["username"], creds["password"]
            )
            authentication_db.update_temp_data(chat_id, {})
            return jsonify({"logged_in": True})

        return jsonify({"secret_code": False})

    if state == "logout":
        chat_id = value[1]
        authentication_db.set_state(chat_id, "login_username")
        return jsonify({"login_username": True})

    if state == "login_username":
        chat_id = value[1]
        if authentication_db.check_user_and_chat_id(value[0], chat_id):
            authentication_db.set_state(chat_id, "login_password")
            return jsonify({"login_username": True})
        return jsonify({"login_username": False})

    if state == "login_password":
        chat_id = value[1]
        temp = authentication_db.get_password_from_db(chat_id)
        if checkpw(value[0].encode(), temp[0].encode()):
            authentication_db.set_state(chat_id, "logged_in")
            return jsonify({"username_password": True})
        return jsonify({"username_password": False})

@app.route("/stocks/model_data/insert", methods=["POST"])
def model_data_to_db():
    """
    POST REQUEST -  ALGO-API sends Post Request to the server the data that feeds into the model
                    (input data of the model).

    """
    data = request.json
    success = stocks_db.insert_model_data(data)
    if success:
        return jsonify({"Success": "Data has inserted to the DB"}), 200        
    else:
        return jsonify({"Failed": "Data Couldn't be inserted to the DB"}), 400        

@app.route("/stocks/model_data/fetch", methods=["GET"])
def model_data_from_db():
    """
    POST REQUEST -  ALGO-API sends Post Request to the server the data that feeds into the model
                    (input data of the model).

    """
    from_date = request.args.get('from_date')
    success = stocks_db.fetch_model_data(from_date)
    if success:
        return jsonify(success), 200 
    else: 
        return jsonify({"Failed": "Data Couldn't be inserted to the DB"}), 400

@app.route("/stocks/model_predictions/insert", methods=["POST"])
def insert_model_predictions():
    """
    POST REQUEST -  ALGO-API sends Post Request to the server that contains the data that the model
                    predicted.

    """
    data = request.json
    date = data['date']
    del(data['date'])
    success = stocks_db.insert_model_predictions(data, date)
    if success:
        return jsonify({"Success": "Data has inserted to the DB"}), 200        
    else: 
        return jsonify({"Failed": "Data Couldn't be inserted to the DB"}), 400      

@app.route("/stocks/model_predictions/date", methods=["GET"])
def model_predictions_date():
    """
    GET REQUEST -   ALGO-API sends GET Request to the server that contains the latest date
                    that model_predictions table has updated.

    """
    valid = bool(request.args.get('valid'))
    date = stocks_db.fetch_model_prediction_date(valid)
    if date:
        return jsonify(date), 200        
    else: 
        return jsonify({"Failed": "Data Couldn't be inserted to the DB"}), 400 

@app.route("/stocks/model_predictions/update", methods=["POST"])
def update_model_predictions():
    """
    POST REQUEST -  ALGO-API sends Post Request to the server that contains the data that the model
                    predicted.

    """
    data = request.json
    date = data['date']
    del(data['date'])
    success = stocks_db.update_model_predictions(data, date)
    if success:
        return jsonify({"Success": "Data has inserted to the DB"}), 200        
    else: 
        return jsonify({"Failed": "Data Couldn't be inserted to the DB"}), 400 

@app.route("/stocks/model_predictions/fetch", methods=["GET"])
def fetch_model_predictions():
    '''
    GET Request -   Client Sends GET Request and fetch the wanted statistics model performance
                    For the specific date

    '''
    if request.args.get('client') == 'telegram':
        min_val = int(request.args.get('min'))
        max_val = int(request.args.get('max'))
        from_date = request.args.get('from_date')
        to_date = request.args.get('to_date')
        max_date = stocks_db.fetch_model_prediction_date(True)
        if not from_date and not to_date: # If There is no date, picks the highest one
            data =  stocks_db.fetch_model_predictions(max_date, max_date, min_val, max_val)
            if data:
                data['date'] = [max_date, max_date]
        elif (to_date > max_date) or (from_date > max_date): # If date is higher than existing data
            return jsonify('Error'), 404
        else: # 
            data = stocks_db.fetch_model_predictions(from_date, to_date, min_val, max_val)
            data['date'] = [from_date, to_date]
    elif request.args.get('client') == 'algo':
        from_date = request.args.get('from_date')
        to_date = request.args.get('to_date')
        data = stocks_db.fetch_model_predictions(from_date, to_date)

    if data:
        logger.info("Model predictions has sent succesfully")
        return jsonify(data), 200
    else:
        return jsonify("Error"), 404

@app.route("/stocks/model_statistics/insert", methods=["POST"])
def insert_model_statistics():
    '''
    POST Request from ALGO-API, ALGO-API sends the Model statistics
    '''
    from_date = request.args.get('from_date')
    to_date = request.args.get('to_date')
    data = request.json
    if from_date and to_date:
        response = stocks_db.insert_model_statistics(from_date, to_date, data)
        if response:
            return jsonify('Insertion of model Statistics has succeed'), 200
        return jsonify('Insertion of model Statistics has failed'), 404
    return jsonify('Insertion of model Statistics has failed, No Dates specified'), 404

@app.route("/stocks/model_statistics/fetch", methods=['GET'])
def fetch_model_statistics():
    '''
    GET Request from ALGO-API, ALGO-API Requesting for the statistics data.
    '''
    data = stocks_db.fetch_model_statistics()
    if data:
        return jsonify(data), 200
    return jsonify('Error'), 404

@app.route("/stocks/model_statistics/latest_date", methods=['GET'])
def fetch_model_statistics_latest_date():
    '''
    GET Request from ALGO-API, ALGO-API Requesting for the latest date of statistics.
    '''
    data = stocks_db.fetch_model_statistics_last_date()
    if data:
        return jsonify(data), 200
    return jsonify('Error'), 404

@app.route("/stocks/recommended_stocks/insert", methods=['POST'])
def insert_reccommended_stocks():
    '''
    POST Request from ALGO-API to insert the recommended stocks into the database
    '''
    data = request.json
    if data:
        #Function to insert the data in the DB
        flag = stocks_db.insert_recommended_stocks(data)
        if flag:
            return jsonify('Success'), 200
    return jsonify('Error'), 404

@app.route("/stocks/recommended_stocks/fetch", methods=['GET'])
def fetch_reccommended_stocks():
    '''
    GET Request from ALGO-API to fetch the recommended stocks
    '''
    data = stocks_db.fetch_recommended_stocks()
    if data:
        return jsonify(data), 200
    return jsonify('Error'), 404

@app.route("/stocks/buy", methods=['POST'])
def buy_stocks():
    '''
    ALGO-API Sends POST Request to server to buy stocks
    There are few things that happen here
        1. Fetching the predictions of today (for tomorrow) from db.
        2. Fetching the recommended stocks from db.
        3. Fetching the trade_configuration from db.
    
    after all done, all the stocks that meet the criterion can be bought
    through alpaca API command, after that operation, needs to check again
    what bought and what didn't, delete the stocks that didn't make it on time.
    what made it on time should be inserted into trade_history db. 
    
    '''
    # Fetching trade configuration from trades.db
    config = trades_db.fetch_trade_configuration()
    if not config:
        return jsonify("Trade configuration data is unavailable"), 400

    # Fetching latest prediction date and data
    latest_date = stocks_db.fetch_model_prediction_date()
    if not latest_date:
        return jsonify("latest date prediction data is unavailable"), 400
    min_val = config["min_range"]
    max_val = config["max_range"]
    prediction_data = stocks_db.fetch_model_predictions(latest_date, latest_date, min_val, max_val)
    if not prediction_data:
        return jsonify("prediction data is unavailable"), 400
    
    # Check if trading enable flag is on
    if not config['enable']:
        return jsonify("Trading is disabled"), 400
    
    # Generate the stocks list to purchase for today. 
    trade_data = helpers.daily_stocks_generation(prediction_data, config)
    if not trade_data:
        return jsonify('failed'), 400
    # Buy the daily stocks
    receipt = helpers.equities_purchase(trade_data)
    
    # Enter all the filled stocks into the database
    flag = trades_db.insert_trade_history(receipt)
    if flag:
        return jsonify("success"), 200 
    return jsonfify("failed"), 400

@app.route("/stocks/sell", methods=['POST'])
def sell_stocks():
    '''
    ALGO-API Sends POST Request to server to sell stocks,
    server pulls the trade_history of the last day.
    after the list of symbols and quantity has received to the server.
    server executes Sell on everything through ALPACA API.
    
    '''
    
    open_positions = alpaca_requests.all_open_positions()
    if not open_positions:
        return jsonify("Failed, No stocks to sell"), 400
        
    receipt = helpers.equities_sell(open_positions)
    
    if not receipt:
        return jsonify("No, equities to sell"), 400
    
    flag = trades_db.insert_trade_history(receipt)
    if flag:
        return jsonify("success"), 200 
    return jsonfify("failed"), 400

if __name__ == "__main__":
    app.run(debug=True)
