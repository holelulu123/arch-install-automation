#! venv/bin/python3
import os
import numpy as np
import helpers
import random

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
    signal: np.array
        array that contains all the parameters from type np.complex64 
    '''
    white_noise = noise_amplitude * (np.random.normal(0, 1, len(int(fs * time))) + 1j * np.random.normal(0, 1, len(int(fs * time))))
    node_signal1 = np.complex64(np.zeros(len(raw_data)))
    for i in range(len(bw)):
        first_index = first_index[i] # index
        time_for_signal = time_per_packet[i] # Seconds
        signal_time_calc = np.int32((time_for_signal/(1/fs)))

        lfm_signal = helpers.linear_frequency_modulation(fs,time_for_signal,f_start = -bw[i]/2, f_end = bw[i]/2)
        if node == 1:
            node_signal1[first_index : first_index + signal_time_calc - 1]                                = lfm_signal[0 : signal_time_calc - 1] 
        
        elif node == 2:
            delay_2 = deltas_t_1_2[i] # Microsecods
            delay_calc_2 = np.int32(((delay_1 * 1e-6)/(1/fs)))
            node_signal2[first_index + delay_calc_1 :  first_index + signal_time_calc + delay_calc_1 - 1] = lfm_signal[0 : signal_time_calc - 1] 

        elif node == 3:
            delay_3 = deltas_t_1_3[i] # Microsecods
            delay_calc_3 = np.int32(((delay_2 * 1e-6)/(1/fs)))
            node_signal3[first_index + delay_calc_2 :  first_index + signal_time_calc + delay_calc_2 - 1] = lfm_signal[0 : signal_time_calc - 1] 



    # adding the white noise
    signal += white_noise


def main():
    # Static variables
    
    time = 0.4 # Seconds
    fs = 30.72e6 # MHz
    minimum_time_of_signal = 100e-6 # Seconds
    maximum_time_of_signal = 10e-3 # Seconds
    
    minimum_bw_of_signal = int(200e3) # MHz
    maximum_bw_of_signal = int(10e6) # MHz

    maximum_distance_between_receivers = 2000 # In meters 
    delta_t_min = helpers.distance_between_recievers(60e-9)
    delta_t_max = helpers.distance_between_recievers(maximum_distance_between_receivers)
    length = int(time * fs) 
    
    # Dynamic variables
    num_packets = random.randint(1,20)
    noise_level = random.random()
    
    starting_index_list = []
    lfm_bw_list = []
    lfm_time_list = []
    deltas_t_1_2 = []
    deltas_t_1_3 = []
    for i in range(num_packets):
        starting_index_list.append(random.randint(1 + (maximum_time_of_signal * fs), length - (maximum_time_of_signal * fs)))
        lfm_bw_list.append(random.randint(minimum_bw_of_signal, maximum_bw_of_signal))
        lfm_time_list.append(random.uniform(minimum_time_of_signal, maximum_time_of_signal))
        deltas_t_1_2.append(random.uniform(delta_t_max, delta_t_max))
        deltas_t_1_3.append(random.uniform(delta_t_max, delta_t_max))

    for i in range(3):
        create_signal




if __name__ == "__main__":

    # Noise to be added for the signal
    folder = 'samples'
    os.makedirs(folder, exist_ok=True)
    node_signal1.tofile(f'{folder}/node01_2025-01-27_124633_2422000000.0.bin')
    node_signal2.tofile(f'{folder}/node02_2025-01-27_124633_2422000000.0.bin')
    node_signal3.tofile(f'{folder}/node03_2025-01-27_124633_2422000000.0.bin')
    
    