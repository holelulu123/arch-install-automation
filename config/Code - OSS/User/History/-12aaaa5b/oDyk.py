#! venv/bin/python3

# from helpers import cross_corr_2_receivers
from flask import Flask, request, jsonify
import datetime
import os

os.makedirs("samples",exist_ok=True)
os.makedirs("samples/node01",exist_ok=True)
os.makedirs("samples/node02",exist_ok=True)
os.makedirs("samples/node03",exist_ok=True)

app = Flask(__name__)
app.debug = True
# Route: /time - GET
@app.route('/config', methods=['GET'])
def get_time():
    current_time = datetime.datetime.now().strftime("%Y-%m-%d_%H%M%S")
    return jsonify({"time": current_time, "freq": 2.4e9})

# Route: /file - POST
@app.route('/upload', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        return jsonify({"error": "No file part in the request"}), 400
    
    file = request.files['file']
    if file.filename == '':
        return jsonify({"error": "No file selected"}), 400
    else:
        filename = file.filename
        splitted = filename.split('_')
        node = splitted[0]
        directory = os.path.join('samples',f"{node}/{filename}")
        file.save(directory)


    return jsonify({"message": "uploaded successfully!"}), 200

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=8000, debug=True)
