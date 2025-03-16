import os
import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt
from src import position
from logger import logger
import global_variables

def distance_between_recievers(d):
    '''
    The function returns the time it took for the signal to propogate at that distance
    t - the time that the signal travels in seconds
    '''
    c = 299792458 # m/s
    t = d / c
    return t

def linear_frequency_modulation(fs, time, f_start, f_end):
    '''
    Creates a baseband signal of lfm
    Parameters:
    -----------
    fs - sample rate of the signal
    time - time of the pulse in Seconds
    f_start = start frequency of the sweep
    f_end = finish frequency of the sweep

    returns the signal as an N-array
    '''
    time_to_samples = np.int32(time * fs)
    # Calculate instantaneous frequency: linear sweep from f_start to f_end
    instantaneous_freq = np.linspace(f_start,f_end,time_to_samples)

    # Calculate phase by integrating instantaneous frequency
    instantaneous_phase = 2 * np.pi * np.cumsum(instantaneous_freq) / fs

    # Generate the LFM signal
    lfm_signal = np.exp(1j * instantaneous_phase)

    # Generate the LFM signal using the calculated phase

    return lfm_signal

def triangulate_from_time_shifts_3d(p1, p2, p3, p4, dt1, dt2, dt3, dt4):
    """
    Triangulate the position of a point based on time shifts measured at four receivers in 3D.

    Args:
    p1, p2, p3, p4: Tuples representing the coordinates of the known points (x, y, z).
    dt1, dt2, dt3, dt4: Time shifts (in seconds) measured at the receivers.

    Returns:
    A tuple representing the coordinates (x, y, z) of the unknown point.
    """
    speed = 299792458 # speed of light

    # Convert time shifts to distance differences
    d1 = dt1 * speed
    d2 = dt2 * speed
    d3 = dt3 * speed
    d4 = dt4 * speed

    # Unpack known points
    x1, y1, z1 = p1
    x2, y2, z2 = p2
    x3, y3, z3 = p3
    x4, y4, z4 = p4

    # Create the coefficient matrix and the constant terms
    A = np.array([
        [2 * (x2 - x1), 2 * (y2 - y1), 2 * (z2 - z1)],
        [2 * (x3 - x1), 2 * (y3 - y1), 2 * (z3 - z1)],
        [2 * (x4 - x1), 2 * (y4 - y1), 2 * (z4 - z1)]
    ])

    B = np.array([
        d1**2 - d2**2 - (x1**2 + y1**2 + z1**2) + (x2**2 + y2**2 + z2**2),
        d1**2 - d3**2 - (x1**2 + y1**2 + z1**2) + (x3**2 + y3**2 + z3**2),
        d1**2 - d4**2 - (x1**2 + y1**2 + z1**2) + (x4**2 + y4**2 + z4**2)
    ])

    # Solve for x, y, z using least squares approximation
    x, y, z = np.linalg.lstsq(A, B, rcond=None)[0]

    return x, y, z

def triangulate_from_time_shifts_2d(p1, p2, p3, dt1, dt2, dt3, z=0):
    """
    Triangulate the position of a point based on time shifts measured at four receivers in 3D.

    Args:
    p1, p2, p3 : Tuples representing the coordinates of the known points (x, y, z).
    dt1, dt2, dt3 : Time shifts (in seconds) measured at the receivers.

    Returns:
    A tuple representing the coordinates (x, y, z) of the unknown point.
    """
    dt = [dt1, dt2, dt3]
    if np.min(dt) < 0:
        dt += np.abs(np.min(dt))

    speed = 299792458 # speed of light

    # Convert time shifts to distance differences
    d1 = dt[0] * speed
    d2 = dt[1] * speed
    d3 = dt[2] * speed
    
    # Unpack known points
    x1, y1 = p1[0:2]
    x2, y2 = p2[0:2]
    x3, y3 = p3[0:2]

    # Simplify the problem by subtracting out the fixed z component
    
    # Create the coefficient matrix and the constant terms
    A = np.array([
        [2 * (x2 - x1), 2 * (y2 - y1)],
        [2 * (x3 - x1), 2 * (y3 - y1)]
    ])
    
    B = np.array([
        d1**2 - d2**2 - (x1**2 + y1**2) + (x2**2 + y2**2),
        d1**2 - d3**2 - (x1**2 + y1**2) + (x3**2 + y3**2)
    ])
    
    # Solve for x, y, z using least squares approximation
    x, y = np.linalg.lstsq(A, B, rcond=None)[0]
    
    return x, y
   
def cross_corr_3_receivers(signal_1, signal_2, signal_3, fs, correlation_plot=True, max_distance = 2500):
    '''
    This function calculates the delta t of between the signals 
    signal_1-4 - signals from each reciever, place None if you dont use that reciever
    max_distance - maximum distance between two recievers that in the cross correlation calculation in meters
    returns - delta t for each signal 
    '''
    # Check if the sizes of the array are equal. if not, the correlation can't be done.
    if (len(signal_1) != len (signal_2)) or (len(signal_1) != len(signal_3)):
        logger.info("The signals are not equal in size")
        return None

    # Appending the signals into dictionary
    signals = dict()
    signals['signal_1'] = signal_1
    signals['signal_2'] = signal_2
    signals['signal_3'] = signal_3

    # Divide the signals into chunks and then correlating them with each other
    chunk_time = global_variables.chunk_time
    chunk_size = np.int32(chunk_time * fs)

    if len(signal_1) % chunk_size != 0:
        logger.info("Chunks cant be divided - Data may be lost")

    chunk_count = np.int32(len(signal_1) / chunk_size)
    start_index = 0
    end_index = chunk_size - 1

    deltas_1_2 = dict()
    deltas_1_3 = dict()
    for i in range(chunk_count):

        deltas_1_2[i] = []
        deltas_1_3[i] = []
        # Compute cross-correlation
        cross_core_1_2 = signal.correlate(signal_2[start_index:end_index], signal_1[start_index:end_index], mode='full')
        cross_core_1_3 = signal.correlate(signal_3[start_index:end_index], signal_1[start_index:end_index], mode='full')

        # Removes the spike that ocurr when the signal is noisy
        cross_core_1_2[np.int32(len(cross_core_1_2)/2)] = 0
        cross_core_1_3[np.int32(len(cross_core_1_3)/2)] = 0

        # Normalize the correlation results between 0 to 1
        cross_core_normalized_1_2 = np.divide(cross_core_1_2, np.max(np.abs(cross_core_1_2)))
        cross_core_normalized_1_3 = np.divide(cross_core_1_3, np.max(np.abs(cross_core_1_3)))

        # find us the maximum time that relavent to us (distance between recievers)
        max_t = distance_between_recievers(max_distance)
        max_t = np.int32(max_t/(1/fs))

        # Set new bounderies based on the timing
        cross_core_clean_1_2 = np.abs(cross_core_normalized_1_2[np.int32(len(cross_core_normalized_1_2)/2 - max_t) : np.int32(len(cross_core_normalized_1_2)/2 + max_t)]) # Removes the sides which are out of interest
        cross_core_clean_1_3 = np.abs(cross_core_normalized_1_3[np.int32(len(cross_core_normalized_1_3)/2 - max_t) : np.int32(len(cross_core_normalized_1_3)/2 + max_t)]) # Removes the sides which are out of interest
        
        # 
        peaks_1_2, properties = signal.find_peaks(cross_core_clean_1_2,height=[0.9, 1.0],distance=np.int32(1e-6 * fs))
        for j in range(len(peaks_1_2)):
            t1 = (peaks_1_2[j]  - len(cross_core_clean_1_2)/2) * (1/fs) # Multiply by 1,000,000 to convert it to microseconds
            if(properties['peak_heights'][j] >= 0.95):
                deltas_1_2[i].append(t1)
                logger.info(f"Delta T: {t1*1000000} Microsecond | Peak: {properties['peak_heights'][j]} | Chunk: {i+1}/{chunk_count}")

        peaks_1_3, properties = signal.find_peaks(cross_core_clean_1_3,height=[0.9, 1.0],distance=np.int32(100e-9 * fs))
        for j in range(len(peaks_1_3)):
            t1 = (peaks_1_3[j]  - len(cross_core_clean_1_3)/2) * (1/fs) # Multiply by 1,000,000 to convert it to microseconds
            if(properties['peak_heights'][j] >= 0.95):
                deltas_1_3[i].append(t1)
                logger.info(f"Delta T: {t1*1000000} Microsecond | Peak: {properties['peak_heights'][j]} | Chunk: {i+1}/{chunk_count}")

        # Plots the correlation results
        if correlation_plot:
            fig, (ax1, ax2, ax3, ax4, ax5) = plt.subplots(5, 1, figsize=(15, 7)) 

            # First subplot
            ax1.plot(np.real(signals['signal_1'][start_index:end_index]), color='blue')
            ax1.plot(np.imag(signals['signal_1'][start_index:end_index]), color='orange')
            
            # Second subplot
            ax2.plot(np.real(signals['signal_2'][start_index:end_index]), color='blue')
            ax2.plot(np.imag(signals['signal_2'][start_index:end_index]), color='orange')
            ax2.sharex(ax1)
            ax2.sharey(ax1)
            
            # Third subplot
            ax3.plot(np.real(signals['signal_3'][start_index:end_index]), color='blue')
            ax3.plot(np.imag(signals['signal_3'][start_index:end_index]), color='orange')
            ax3.sharex(ax1)
            ax3.sharey(ax1)

            # Fourth subplot
            for peak in peaks_1_2:
                ax4.axvline(peak,color='orange')
            ax4.plot(np.abs(cross_core_clean_1_2))

            # Fifth subplot
            for peak in peaks_1_3:
                ax5.axvline(peak,color='orange')
            ax5.plot(np.abs(cross_core_clean_1_3))

            # Adjust layout to prevent overlap
            plt.tight_layout()
            plt.suptitle(f'Chunk {i}/{chunk_count-1}')
            plt.show()

        start_index += chunk_size
        end_index += chunk_size

        # Naive way to find the maximum - later idea is to use find local maxima algo
        # max_corr_index = np.argmax(cross_core_normalized)

    return deltas_1_2, deltas_1_3

def cross_corr_2_receivers(signal_1, signal_2, fs, correlation_plot=True, max_distance = 2500):
    '''
    This function calculates the delta t of between the signals 
    used for tests.
    ''' 
    # Check if the sizes of the array are equal. if not, the correlation can't be done.
    if len(signal_1) != len (signal_2):
        print("The signals are not equal in size")
        return None 
    
    # Appending the signals into dictionary 
    signals = dict()
    signals['signal_1'] = signal_1
    signals['signal_2'] = signal_2
    
    # Divide the signals into chunks and then correlating them with each other
    chunk_time = global_variables.chunk_time
    chunk_size = np.int32(chunk_time * fs)
    
    if len(signal_1) % chunk_size != 0:
        print("Chunks cant be divided - Data may be lost")

    chunk_count = np.int32(len(signal_1) / chunk_size)
    start_index = 0
    end_index = chunk_size - 1

    deltas_1_2 = dict()
    for i in range(chunk_count):
        deltas_1_2[i] = []
        # Compute cross-correlation
        cross_core_1_2 = signal.correlate(signal_2[start_index:end_index], signal_1[start_index:end_index], mode='full')
        
        # Removes the spike that ocurr when the signal is noisy
        cross_core_1_2[np.int32(len(cross_core_1_2)/2)] = 0 

        # Normalize the correlation results between 0 to 1
        cross_core_normalized_1_2 = np.divide(cross_core_1_2, np.max(np.abs(cross_core_1_2)))
        
        # find us the maximum time that relavent to us (distance between recievers)
        max_t = distance_between_recievers(max_distance)
        max_t = np.int32(max_t/(1/fs))
        
        # Set new bounderies based on the timing 
        cross_core_clean_1_2 = np.abs(cross_core_normalized_1_2[np.int32(len(cross_core_normalized_1_2)/2 - max_t) : np.int32(len(cross_core_normalized_1_2)/2 + max_t)]) # Removes the sides which are out of interest
        
        peaks_1_2, properties = signal.find_peaks(cross_core_clean_1_2,height=[0.7,1.0],distance=np.int32(300e-9 * fs))
        for j in range(len(peaks_1_2)):
            t1 = (peaks_1_2[j]  - len(cross_core_clean_1_2)/2) * (1/fs) # Multiply by 1,000,000 to convert it to microseconds
            if(properties['peak_heights'][j] >= 0.9):    
                deltas_1_2[i].append(t1)
                print("Delta T is ",{t1*1e9}, " Nanosecond And Peak Value is, ",{properties['peak_heights'][j]})

        # Plots the correlation results
        if correlation_plot:
            fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(15, 7)) 

            freq_domain = np.linspace(-fs/2,fs/2,len(signal_1))

            # First subplot
            ax1.plot(signals['signal_1'][start_index:end_index],color='orange')
            ax1.set_title('First signal Time domain')
            # Second subplot
            ax2.plot(signals['signal_2'][start_index:end_index],color='blue')
            ax2.set_title('Second signal Time domain')
            ax2.sharex(ax1)
            ax2.sharey(ax1)
                    
            # Fourth subplot            
            for peak in peaks_1_2:
                ax3.axvline(peak,color='orange')
            ax3.plot(np.abs(cross_core_clean_1_2))
            ax3.set_title('Correlation between signal 1 and signal')
            
            # Fourth subplot 
            signal_1_freq = np.abs(np.fft.fftshift(np.fft.fft(signal_1)))
            signal_2_freq = np.abs(np.fft.fftshift(np.fft.fft(signal_2)))
            ax4.plot(freq_domain, signal_1_freq, color='orange')
            ax4.plot(freq_domain, signal_2_freq, color='blue')
            ax4.set_title('Frequnecy domain of signal 1 and signal 2')
            # Adjust layout to prevent overlap
            plt.tight_layout()

            plt.show()
        
        
        start_index += chunk_size
        end_index += chunk_size

        # Naive way to find the maximum - later idea is to use find local maxima algo
        # max_corr_index = np.argmax(cross_core_normalized)

    return deltas_1_2

def object_coordinates(deltas_1_2,deltas_1_3, p1, p2, p3, cf, label_dict):
    '''
    This function gets the deltas dict from each pair of reciever and return a list of coordinates of the object
    deltas_1_2 - deltas between reciever 1 and reciever 2 
    deltas_1_3 - deltas between reciever 1 and reciever 3
    p1-3 - GPS coordinates of the recievers
    '''

    reciever_pos_1 = position.lat_lon_to_utm(p1[0],p1[1])
    reciever_pos_2 = position.lat_lon_to_utm(p2[0],p2[1])
    reciever_pos_3 = position.lat_lon_to_utm(p3[0],p3[1])

    coordinates = dict()
    for i in range(len(deltas_1_2)):
        if deltas_1_2[i] == [] or deltas_1_3[i] == []:
            continue
        delta_t_1_2 = deltas_1_2[i][0]
        delta_t_1_3 = deltas_1_3[i][0]
        object_pos = triangulate_from_time_shifts_2d(reciever_pos_1,reciever_pos_2,reciever_pos_3,0,delta_t_1_2,delta_t_1_3)
        object_pos = position.utm_to_lat_lon(object_pos[0],object_pos[1],36,'N')
        object_pos = list(object_pos)
        object_pos.append(label_dict[i])
        object_pos.append(cf)
        coordinates[i] = object_pos

    return coordinates

def drone_labels_extraction(drone_labels_1, drone_labels_2, drone_labels_3):
    '''
    This function Takes 3 List with the same size that represent the output 
    of the Model and return dictionary contains the input data.

    Parameters:
    -----------
    drone_labels_1: List
        List of labels for each chunk
    drone_labels_2: List
        List of labels for each chunk
    drone_labels_3: List
        List of labels for each chunk
    
    Returns:
    -----------
    output: Dict
        Dict contains all the lists.
    ''' 
    if len(drone_labels_1) != len(drone_labels_2) != len(drone_labels_3):
        logger.info(f"Drone Labels have different size")
        return False
    output = {}
    labels = [drone_labels_1, drone_labels_2, drone_labels_3]
    temp_dict = {}
    for i in range(len(drone_labels_1)):
        for label in labels:
            if label[i][0] in temp_dict.keys(): 
                temp_dict[label[i][0]] += 1  
            else:
                temp_dict[label[i][0]] = 1  
        keys = list(temp_dict.keys())
        values = list(temp_dict.values())
        if len(keys) == 1:
            output[i] = keys[0]
        else:
            if 'None' in keys: 
                keys.remove('None')
                output[i] = keys[0]
        temp_dict = {}
    return output
         