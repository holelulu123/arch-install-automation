#! venv/bin/python3
import os
import helpers
import random
import datetime
import threading
import requests
import subprocess
import time
import argparse
import numpy as np
from logger import logger

DIRECTORY = 'samples'
os.makedirs(DIRECTORY, exist_ok=True)

def check_ping(ip: str):
    command = ['ping', '-c', '5', '-W', '10', f'{ip}']
    return not subprocess.call(command)

def check_run(url: str, start=False):
    '''
    This function runs by a different thread, it's job is to check for samples that hadn't sent to the server.
    after the  
    '''
    while(True):
        files = os.listdir(DIRECTORY)
        if start:
            if files:
                logger.info(f'Cleaning {DIRECTORY} directory')
                for file in files:
                    logger.info(f'Removing file {file}...')
                    os.remove(f'{DIRECTORY}/{file}')
                logger.info(f'Finished Cleaning the file.')
            break
        else:
            if files:
                file_to_send = files[0]
                dir_path = os.path.join(DIRECTORY, file_to_send)
                with open(dir_path, 'rb') as file:
                    files = {'file': (file_to_send, file)}
                    try:
                        response = requests.post(f"{url}/upload", files=files)
                    except:
                        raise Exception(f"Requests can't be sent to server")
                if response.status_code == 200:
                    logger.info(f'File {file_to_send} has succesfully sent.')
                    os.remove(f'{DIRECTORY}/{file_to_send}')
                else:
                    logger.info(f"File {file_to_send} hasn't succesfully sent.")
                    os.remove(f'{DIRECTORY}/{file_to_send}')
        time.sleep(0.2)

def create_signal(
    time_per_packet: list,
    deltas_t_1_2: list,
    deltas_t_1_3:list,
    first_index_list: list,
    bw : list, 
    noise_level: float,
    node: int,
    fs: float = 30.72e6, 
    time: float = 0.4
    ):
    '''
    Creates an array and fill it with packets of lfm signal with ramdom parameters
    Parameters:
    -----------
    deltas_t_1_2: list
        list that each value of the list represent the delta time between receiver 1 and receiver 2    
    deltas_t_1_3: list
        list that each value of the list represent the delta time between receiver 1 and receiver 3    
    first_index: list
        list that each value of the list represent the starting index of each packet
    bw: list
        list that each value of the list represent the bandwidth in Frequnecy of each packet
    noise_level: float
        the noise level of the signal that is being created, noise levels are : 0 <-> 1
    fs: float
        sampling frequnecy of the signal
    time: float
        total time of the signal

    Returns:
    ----------
    node_signal: np.array
        array that contains all the parameters from type np.complex64 
    '''
    nsamps = int(fs * time)
    white_noise = noise_level * (np.random.normal(0, 1, nsamps) + 1j * np.random.normal(0, 1, nsamps))
    node_signal = np.complex64(np.zeros(nsamps))
    for i in range(len(bw)):
        first_index = first_index_list[i] # index
        time_for_signal = time_per_packet[i] # Seconds
        signal_time_calc = np.int32((time_for_signal/(1/fs)))

        lfm_signal = helpers.linear_frequency_modulation(fs,time_for_signal,f_start = -bw[i]/2, f_end = bw[i]/2)
        if node == 1:
            node_signal[first_index : first_index + signal_time_calc - 1] = lfm_signal[0 : signal_time_calc - 1] 
        
        elif node == 2:
            delay_2 = deltas_t_1_2[i] # Microsecods
            delay_calc_2 = np.int32(((delay_2 * 1e-6)/(1/fs)))
            node_signal[first_index + delay_calc_2 :  first_index + signal_time_calc + delay_calc_2 - 1] = lfm_signal[0 : signal_time_calc - 1] 

        elif node == 3:
            delay_3 = deltas_t_1_3[i] # Microsecods
            delay_calc_3 = np.int32(((delay_3 * 1e-6)/(1/fs)))
            node_signal[first_index + delay_calc_3 :  first_index + signal_time_calc + delay_calc_3 - 1] = lfm_signal[0 : signal_time_calc - 1] 



    # adding the white noise
    node_signal += white_noise
    return node_signal

def main():
    # Static variables
    time = 0.4 # Seconds
    fs = 30.72e6 # MHz
    minimum_time_of_signal = 100e-6 # Seconds
    maximum_time_of_signal = 10e-3 # Seconds

    minimum_bw_of_signal = int(200e3) # MHz
    maximum_bw_of_signal = int(10e6) # MHz

    max_distance = 2000 # In meters 
    delta_t_min = helpers.distance_between_recievers(60e-9) * 1e6
    delta_t_max = helpers.distance_between_recievers(max_distance) * 1e6
    length = int(time * fs) 

    while True:
        # Dynamic variables
        date = datetime.datetime.now()
        formatted_date = date.strftime("%Y-%m-%d_%H%M%S")
        frequency = random.randint(int(2.4e9), int(2.48e9))
        num_packets = random.randint(1,20)
        noise_level = random.random()
        logger.info(f"Noise level is: {noise_level}")
        logger.info(f"Num packets is: {num_packets}")
        logger.info(f"Frequnecy is: {frequency}")
        starting_index_list = []
        lfm_bw_list = []
        lfm_time_list = []
        deltas_t_1_2 = []
        deltas_t_1_3 = []
        for i in range(num_packets):
            starting_index_list.append(random.randint(int(1 + (maximum_time_of_signal * fs)), int(length - (maximum_time_of_signal * fs))))
            lfm_bw_list.append(random.randint(minimum_bw_of_signal, maximum_bw_of_signal))
            lfm_time_list.append(random.uniform(minimum_time_of_signal, maximum_time_of_signal))
            deltas_t_1_2.append(random.uniform(delta_t_max, delta_t_max))
            deltas_t_1_3.append(random.uniform(delta_t_max, delta_t_max))

        for i in range(1,4):

            signal = create_signal(
            time_per_packet=lfm_time_list,
            deltas_t_1_2=deltas_t_1_2,
            deltas_t_1_3=deltas_t_1_3,
            first_index_list=starting_index_list,
            bw=lfm_bw_list,
            noise_level=noise_level,
            node=i
            )
            filename = f'samples/node0{i}_{formatted_date}_{frequency}.bin'
            signal.tofile(filename)

        break


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Enter IP and PORT of the server.")
    parser.add_argument("--ip", type=str, help="Enter the IP of the server", required=True)    
    parser.add_argument("--port", type=str, help='Enter the PORT of the server', required=True)    
    args = parser.parse_args()

    logger.info(f"IP of server: {args.ip}")
    logger.info(f"PORT of server: {args.port}")
    ip = args.ip
    port = args.port
    url = f"http://{ip}:{port}"
    
    time.sleep(3)
    logger.info(f"Testing connection with IP: {ip}")
    pong = check_ping(ip=ip)
    if not pong:
        raise Exception(f'No connection with IP: {ip}')     
    
    check_run(url, True)
    logger.info(f"Starting the mock...")

    t1 = threading.Thread(target=check_run, args=(url,False), daemon=True)
    t2 = threading.Thread(target=main, daemon=True)
    
    t1.start()
    t2.start()
    
    while(True):
        time.sleep(1)

    t1.join()
    t2.join()

    
    