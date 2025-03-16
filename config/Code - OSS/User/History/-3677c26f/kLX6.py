#! venv/bin/python3

import numpy as np
from src import helpers, position
from src.logger.logger import logger

fs = 30.72e6
time = 0.4
time_domain = np.linspace(0,time,np.int32(fs*time))
freq = 300e3

raw_data = np.exp(1j*freq*time_domain*2*np.pi)

# Noise to be added for the signal
noise_amplitude = 0.8
white_noise1 = noise_amplitude * (np.random.normal(0, 1, len(time_domain)) + 1j * np.random.normal(0, 1, len(time_domain)))
white_noise2 = noise_amplitude * (np.random.normal(0, 1, len(time_domain)) + 1j * np.random.normal(0, 1, len(time_domain)))
white_noise3 = noise_amplitude * (np.random.normal(0, 1, len(time_domain)) + 1j * np.random.normal(0, 1, len(time_domain)))

# Initalize the arrays of the Recievers
node_signal1 = np.complex64(np.zeros(len(raw_data)))
node_signal2 = np.complex64(np.zeros(len(raw_data)))
node_signal3 = np.complex64(np.zeros(len(raw_data)))

#-------- Configuration for the First signal ------------

# first_index = 1500000 # index
# time_for_signal = 0.003 # Seconds
# delay_1 = 1 # Microseconds
# delay_2 = 1.5 # Microseconds

# signal_time_calc = np.int32((time_for_signal/(1/fs)))
# delay_calc_1 = np.int32(((delay_1 * 1e-6)/(1/fs)))
# delay_calc_2 = np.int32(((delay_2 * 1e-6)/(1/fs)))

# lfm_signal = linear_frequency_modulation(fs,time_for_signal,f_start = -0.5e6,f_end = 0.5e6)

# node_signal1[first_index : first_index + signal_time_calc - 1]                                = lfm_signal[0 : signal_time_calc - 1] 
# node_signal2[first_index + delay_calc_1 :  first_index + signal_time_calc + delay_calc_1 - 1] = lfm_signal[0 : signal_time_calc - 1]
# node_signal3[first_index + delay_calc_2 :  first_index + signal_time_calc + delay_calc_2 - 1] = lfm_signal[0 : signal_time_calc - 1] 

# #-------- Configuration for the Second signal ------------

# first_index = 200000 # index
# time_for_signal = 0.002 # Seconds
# delay_1 = 0.5 # Microsconds
# delay_2 = 0.2 # Microsconds

# signal_time_calc = np.int32((time_for_signal/(1/fs)))
# delay_calc_1 = np.int32(((delay_1 * 1e-6)/(1/fs)))
# delay_calc_2 = np.int32(((delay_2 * 1e-6)/(1/fs)))

# lfm_signal = linear_frequency_modulation(fs,time_for_signal,f_start = -0.5e6,f_end = 0.5e6)

# node_signal1[first_index : first_index + signal_time_calc - 1]                                = lfm_signal[0 : signal_time_calc - 1] 
# node_signal2[first_index + delay_calc_1 :  first_index + signal_time_calc + delay_calc_1 - 1] = lfm_signal[0 : signal_time_calc - 1] 
# node_signal3[first_index + delay_calc_2 :  first_index + signal_time_calc + delay_calc_2 - 1] = lfm_signal[0 : signal_time_calc - 1] 

#-------- Configuration for the Third signal ------------
first_index = 100000 # index
time_for_signal = 0.003 # Seconds
delay_1 = -5 # Microsecods
delay_2 = 1 # Microsecods 

signal_time_calc = np.int32((time_for_signal/(1/fs)))
delay_calc_1 = np.int32(((delay_1 * 1e-6)/(1/fs)))
delay_calc_2 = np.int32(((delay_2 * 1e-6)/(1/fs)))

lfm_signal = helpers.linear_frequency_modulation(fs,time_for_signal,f_start = -5e6,f_end = 5e6)

node_signal1[first_index : first_index + signal_time_calc - 1]                                = lfm_signal[0 : signal_time_calc - 1] 
node_signal2[first_index + delay_calc_1 :  first_index + signal_time_calc + delay_calc_1 - 1] = lfm_signal[0 : signal_time_calc - 1] 
node_signal3[first_index + delay_calc_2 :  first_index + signal_time_calc + delay_calc_2 - 1] = lfm_signal[0 : signal_time_calc - 1] 


# adding the white noise
node_signal1 += white_noise1
node_signal2 += white_noise2
node_signal3 += white_noise3

if __name__ == "__main__":

    deltas_1_2, deltas_1_3 = helpers.cross_corr_3_receivers(node_signal1, node_signal2, node_signal3, correlation_plot=True, fs=fs,max_distance=4000)

    reciever_1 = [31.498890974766184, 34.804414382025286]
    reciever_2 = [31.488720250381718, 34.81917131535559]
    reciever_3 = [31.505826171295062, 34.81090871869788]

    data = helpers.object_coordinates(deltas_1_2,deltas_1_3,reciever_1,reciever_2,reciever_3, 2.4e9)
    print(data)
    position.location_on_map(reciever_1,reciever_2,reciever_3,data)
