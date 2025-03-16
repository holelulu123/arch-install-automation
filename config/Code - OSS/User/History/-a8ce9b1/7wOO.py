#! venv/bin/python3

import argparse
import os
import json
import numpy as np
import scipy.signal as signal
import globals
import matplotlib.pyplot as plt
from logger import logger



def energy(array):
    return 10 * np.log10(np.mean(np.abs(array)**2))

def main(directory, fs, samples_record_time, number_of_replicas,directory_of_saving):
    '''
    directory - directory that contains all the folders
    fs - sample frequency of the record
    samples_record_time - time of one recording
    number_of_replicas - number of replicas per one file
    '''
    items = os.listdir(directory)
    # For loop on all the folders in stock_data directory

    for j,item in enumerate(items):
        directory_recordings = os.path.join(directory, item)
        recordings = os.listdir(directory_recordings)
        for i,record in enumerate(recordings):
            file_path = os.path.join(directory_recordings, record)
            try:
                samples = np.fromfile(file_path, dtype=np.complex64)
            except:
                logger.error(f'There is a problem with file: {record}')
            logger.info(f'Running now on file {item} - {i} / {len(recordings)}')
            # Now signal procces need to be made in order to find start and end indexes of downlink / uplink
            # for each segment (signal), signal mean needs to be checked. after that, check the mean for all the means
            # and that would use us for the next part of the (create data with different snrs) 
            min_packet_len = globals.signal_len[0]
            max_packet_len = globals.signal_len[1]
            width_limit = [np.int32(min_packet_len * fs), np.int32(max_packet_len * fs)]
            window_size = globals.Smoothing_window_size
            signal_abs = np.abs(samples)
            ma_window = np.ones(window_size) / window_size
            smooth_signal = np.convolve(signal_abs,ma_window,mode='same')
            above_level = (smooth_signal > globals.threshold_time_domain * np.max(smooth_signal)).astype(int)
            peaks = []
            start = 0
            for i in range(1, len(above_level)):
                if above_level[i] == 1 and above_level[i - 1] == 0:
                    start = i
                elif above_level[i] == 0 and above_level[i - 1] == 1:
                    end = i
                    segment_length = np.int32(end - start)
                    if width_limit[0] <= segment_length <= width_limit[1]:
                        peaks.append((start, end))
                        
            if len(peaks) < 5:
                # logger.error(f"No Peaks found in file : {item}/{record}")
                continue
            for i in range(len(peaks)):
                start = peaks[i][0] 
                end = peaks[i][1] 
                if globals.plot_preperation:

                    time_domain = np.linspace(0, samples_record_time*fs, np.int32(samples_record_time*fs))
                    plt.plot(time_domain, np.real(smooth_signal), color='blue')
                    plt.axhline(globals.threshold_time_domain * np.max(smooth_signal),color='orange')
                    plt.axvline(start, color='purple') # start of the signal in vertical line
                    plt.axvline(end, color='purple') # end of the signal in vertical line
                    plt.xlabel("Time")
                    plt.ylabel("Amplitude")

                    plt.show()
                
                # Save csv / json file here. 
            
            # Here after the peaks are good, the files needs to be saved at directory and duplicated with different snrs 
            saving_path = os.path.join(directory_of_saving,item)
            os.makedirs(saving_path, exist_ok=True)
            # Saving the csv file here.
            # partial_data = {key: properties[key].tolist() for key in ["left_bases", "right_bases"]}            
            partial_data = {
                'left_bases': [item[0] for item in peaks],
                'right_bases': [item[1] for item in peaks]
            }
            record_path = os.path.join(saving_path,f'{record}.json')
            
            with open(record_path, "w") as file:
                # Convert the dictionary to a JSON string and write it to the file
                json.dump(partial_data, file, indent=4)
            
            if number_of_replicas != 0:
                snrs = np.int32(np.linspace(45,-10,number_of_replicas))
                for i in range(len(snrs)):
                    SNR_dB = snrs[i]  # Desired SNR in dB

                    SNR_linear = 10**(SNR_dB / 10) 
                    noise_power = signal_power / SNR_linear
                    noise = np.sqrt(noise_power) * (np.random.normal(0, 1, len(samples)) + 1j * np.random.normal(0, 1, len(samples)))
                    # noisy_data may be corrupted because i dont add noise in the imaginary axis
                    noisy_data = samples + noise    
                    if globals.plot_noisy_signal:
                        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 5)) 
                        time_domain = np.linspace(0, samples_record_time, np.int32(samples_record_time*fs))
                        
                        # First subplot
                        ax1.plot(time_domain*1000000, np.real(samples), color='blue')
                        ax1.set_title('Clean Signal in the time domain')
                        ax1.set_xlabel("Time")
                        ax1.set_ylabel("Amplitude")

                        # Second subplot
                        ax2.plot(time_domain*1000000,np.real(noisy_data), color='blue')
                        ax2.set_title("Noisy Signal in the time domain")
                        ax2.set_xlabel("Time")
                        ax2.set_ylabel("Amplitude")
                            
                        plt.suptitle(f'Additive Noise comparison, SNR : {snrs[i]} dB for file : {item}/{record}')
                        # Adjust layout to prevent overlap
                        plt.tight_layout()
                        plt.show()
                    
                    filename = f'{item}_snr_{snrs[i]}'
                    if not filename in content: 
                        record_file_path = os.path.join(record_path,filename)
                        noisy_data.astype(np.complex64).tofile(record_file_path)
                        logging.info(f'The file {record_path}/{filename} has been succesfully written !')
                    else:
                        logging.info(f'The file {record_path}/{filename} is already exist !')

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Enter your configuration for Data Preperation.")
    parser.add_argument("--recording_dir", type=str, help="Local directory of drone recording", required=False)    
    parser.add_argument("--fs", type=float, help="Sample rate of RF recording", required=False)
    parser.add_argument("--time", type=float, help="Length of RF recording (in Seconds)", required=False, default=30)
    parser.add_argument("--replicas", type=int, help="Replicas per one RF recording", required=False, default=0)
    parser.add_argument("--save_dir", type=str, help="Saving direcory of labels", required=True, default=30)
    args = parser.parse_args()

    main('/mnt/hard_drive/drone_dataset_palmahim', 30.72e6, 0.4, 0,'/mnt/hard_drive/drone_labels')
