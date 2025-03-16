import os
import json
import requests
import numpy as np

FS = 30.72e6
TIME = 0.4 
PORT = "5000"
IP = f"10.0.0.3:{PORT}"

def post_test():
    dict = {
        "path1" : "testtest1",
        "path2" : "testtest2",
        "path3" : "testtest3",
        "frequency" : 24222222,
    }
    response = requests.post("http://localhost:8000/algo", json=)


def post_samples(filename: str ,directory: str):
    '''
    POST Request to Server, sending a file contains an np.complex64 array. 
    
    Parameters:
    -----------
    filename: String
        name of the file contains the baseband data.
    directory: String
        directory of the file
    node: Int
        represent the node number that send that request to the server 
    
    Returns:
    ---------
    flag: Bool
        True means operation succeed, False is the opposite
    '''
    dir_path = os.path.join(directory, filename)
    with open(dir_path, 'rb') as file:
        files = {'file': (filename, file)}
        response =  requests.post(f'http://{IP}/upload', files=files) # Need to change URL here
    if response: 
        print(f'Samples Request Sent to the server {response.status_code}')
        try:
            os.remove(dir_path)
        except:
            pass
        return True
    print(f"Samples Request couldn't Sent to the server {response.status_code}")
    return False

def fetch_server_time():
    '''
    GET Request to server, to fetch the time.

    Returns:
    -----------
    my_time: String
        date formatted as 'YYYY-MM-DD_hhmmss' And represent the time that server 
        sent.
    '''
    response = requests.get(f'http://{IP}/time') # Need to change URL here
    if response:
        print(f'Successfully Received time from server, {response.status_code}')
        return response.json()
    print(f"Couldn't Receive time from server, {response.status_code}")
    return False

def mock_signal(node: int):
    '''
    Creates a mock signal, fetches time from the server 
    and send the signal with the timestamp to the server

    Parameters:
    ------------
    node: int
        the node you want to mock from, 
        e.g format -> 0, will send it from node01
                      1, will send it from node02
                      2, will send it from node03
    
        
    '''
    os.makedirs('mocks', exist_ok=True)
    
    time_domain = np.linspace(0, TIME, int(FS * TIME))
    # Complex array created
    signal = np.complex64(np.random.normal(0, 1, len(time_domain)) + 1j * np.random.normal(0, 1, len(time_domain)))

    # GET Request from server 
    what_time = fetch_server_time()
    filename = f'node0{node}_{what_time}'
    directory = 'samples'
    file_path = f'{directory}/{filename}'
    signal.tofile(file_path)
    post_samples(filename, directory)
    print(f'File sent: {filename}')