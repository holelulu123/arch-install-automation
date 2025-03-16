import numpy as np
from logger import logger
from src.helpers import cross_corr_3_receivers, object_coordinates, drone_labels_extraction
from src.model.inference import *
import global_variables
from flask import Flask, request, jsonify

app = Flask(__name__)
app.debug = True

def run_algo(node1_dir: str, node2_dir: str, node3_dir: str, frequency: float):
    """
    Function that gets three signal directory of all the different nodes and
    outputs a json file containing all the coordinations and labels of transmitters.

    Parameters:
    ------------
    node1_dir: str
        directory of file of signal containts baseband complex data for node 1

    node2_dir: str
        directory of file of signal containts baseband complex data for node 2

    node3_dir: str
        directory of file of signal containts baseband complex data for node 3

    frequency: float
        frequnecy in Hz that the nodes recoreded the RF data

    Returns:
    ------------
    output: Json
        json containing coordinations, labels and frequnecy
        format e.g -> {'index': [lat, lot, label, frequency]}
    """
    inference = Live_Inference()
    
    results_node1 = inference.main(node1_dir)
    results_node2 = inference.main(node2_dir)
    results_node3 = inference.main(node3_dir)
    
    model_results = drone_labels_extraction(results_node1, results_node2, results_node3)
    if not model_results:
        return {}
    
    node1_sig = np.fromfile(node1_dir, dtype=np.complex64)
    node2_sig = np.fromfile(node2_dir, dtype=np.complex64)
    node3_sig = np.fromfile(node3_dir, dtype=np.complex64)

    deltas_1_2, deltas_1_3 = cross_corr_3_receivers(
        node1_sig,
        node2_sig,
        node3_sig,
        correlation_plot=False,
        fs=global_variables.fs,
        max_distance=global_variables.max_distance,
    )

    # AI function needs to sit here.
    data = object_coordinates(
        deltas_1_2,
        deltas_1_3,
        global_variables.receiver_coordination_1,
        global_variables.receiver_coordination_2,
        global_variables.receiver_coordination_3,
        frequency,
        model_results
    )

    # Print the json so it receives in the go code as stdout
    return(data)

@app.route('/algo', methods=['POST'])
def algo_trigger():
    '''
    Trigger to start executing the TDoA algorithm
    '''

    data = request.get_json()
    if not data:
        return jsonify({"Error": "Error"}), 400
    

    path1 = data["path1"]
    path2 = data["path2"]
    path3 = data["path3"]
    freq = float(data["frequency"])
    logger.info(f'Path 1 is: {path1}')
    logger.info(f'Path 2 is: {path2}')
    logger.info(f'Path 3 is: {path3}')
    logger.info(f'Freq is: {freq}')
    result = run_algo(
    'node01_2025-01-27_124633_2422000000.0.bin',
    'node01_2025-01-27_124633_2422000000.0.bin',
    'node01_2025-01-27_124633_2422000000.0.bin',
    freq)
    if result:
        return jsonify(result), 200
    
    return jsonify({"Error": "Error"}), 400

@app.route('/time', methods=['GET'])
def send_time():
    return jsonify('123'), 200



if __name__ == "__main__":
    app.run(debug=True)
