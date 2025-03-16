#! venv/bin/python3

import argparse
import os
import numpy as np
import time
from logger import logger
import uhd
import scipy.signal as signal
from utils.visuals import visual_representation
from datetime import datetime



class SDR:
    def __init__(self):
        self.usrp = uhd.usrp.MultiUSRP("num_recv_frames=1000")
        self.usrp.set_clock_source("internal")
        self.usrp.set_time_source("internal")
        #self.usrp.set_master_clock_rate(13e6)

        self.sample_rate = 30.72e6 # Hz
        self.cf = 2452e6 # Hz
        self.gain = 33  # dB
        self.time_to_recv = 0 # Seconds

        self.usrp.set_rx_rate(self.sample_rate, 0)
        self.usrp.set_rx_freq(uhd.libpyuhd.types.tune_request(self.cf), 0)
        self.usrp.set_rx_gain(self.gain, 0)

        # Set up the stream and receive buffer
        st_args = uhd.usrp.StreamArgs("fc32", "sc16")
        st_args.channels = [0]
        self.metadata = uhd.types.RXMetadata()
        self.streamer = self.usrp.get_rx_stream(st_args)
        self.recv_buffer = np.zeros((1, 1000), dtype=np.complex64)

    def start_recieve(self, cf, gain, time_to_recv):
        '''
        This function command the SDR to start to recieve RF signals
            gain - set the the gain of the internal RF amplifier of the SDR
            cf - set the the center frequnecy of the SDR
            time_to_recv - set the reciving time of the SDR
        
        '''
        
        self.time_to_recv = time_to_recv    
        self.num_samps = np.int32(self.time_to_recv * self.sample_rate)
        # configuoutput, output_size, deltarating the SDR
        self.set_sdr_frequnecy(cf)
        self.set_sdr_gain(gain)
        
        # Start Stream
        stream_cmd = uhd.types.StreamCMD(uhd.types.StreamMode.start_cont)
        stream_cmd.stream_now = True
        self.streamer.issue_stream_cmd(stream_cmd)

        # Receive Samples
        samples = np.zeros(self.num_samps, dtype=np.complex64)
        for i in range(self.num_samps//1000):
            self.streamer.recv(self.recv_buffer, self.metadata)
            samples[i*1000:(i+1)*1000] = self.recv_buffer[0]

        # Stop Stream
        stream_cmd = uhd.types.StreamCMD(uhd.types.StreamMode.stop_cont)
        self.streamer.issue_stream_cmd(stream_cmd)

        return samples 
        # return output
   
    def start_transmit(self, samples, duration, cf, gain):
        self.usrp.send_waveform(samples, duration, cf, self.sample_rate, [0], gain)

    def prints(self):
        logger.info(self.usrp.get_mboard_sensor("ref_locked", 0))
        logger.info(self.usrp.get_mboard_sensor("gps_locked", 0))
   
    def sync_time(self):
        time_at_last_pps = self.usrp.get_time_last_pps().get_real_secs()
        while time_at_last_pps == self.usrp.get_time_last_pps().get_real_secs():
            time.sleep(0.1) # keep waiting till it happens- if this while loop never finishes then the PPS signal isn't there
        self.usrp.set_time_next_pps(uhd.libpyuhd.types.time_spec(0.0))
    
    def what_is_time(self):
        return self.usrp.get_time_last_pps().get_real_secs()
        # logger.info(f"The time is: {self.usrp.get_time_last_pps().get_real_secs()}")
        
    def set_sdr_gain(self,gain):
        self.gain = gain
        self.usrp.set_rx_gain(self.gain, 0)

    def set_sdr_frequnecy(self,cf):
        self.cf = cf 
        self.usrp.set_rx_freq(uhd.libpyuhd.types.tune_request(self.cf), 0)
    
def create_file(directory, folder_name, file_name, array):
    
    # Create the full path for the new folder inside the current directory
    folder_path = os.path.join(directory, folder_name)
    
    # Create the folder if it doesn't exist
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)
        logger.info(f"Folder '{folder_name}' created at {folder_path}")

    # Create the full path for the .bin file
    file_path = os.path.join(folder_path, file_name)
    
    # Save the array to the .bin file in complex64 format
    array.astype(np.complex64).tofile(file_path)
    logger.info(f"Array saved to '{file_path}' as a binary file.")

def energy(array):
    return 10 * np.log10(np.mean(np.abs(array)**2))

parser = argparse.ArgumentParser(description="Enter Settings for your RF recordings.")
parser.add_argument("--directory", type=str, help="Directory to save the RF recordings", required=False)
parser.add_argument("--folder", type=str, help="Folder that will be created inside the directory", required=False)
parser.add_argument("--gain", type=int, help="Internal RF gain of the SDR", required=False, default=35)
parser.add_argument("--time", type=float, help="Recording time of one record (Secs)", required=False, default=0.4)
parser.add_argument("--cellular_uplink_3G", type=bool, help="Enable recoroding in Cellular Uplink 3G bands", required=False, default=False)
parser.add_argument("--cellular_uplink_4G", type=bool, help="Enable recoroding in Cellular Uplink 4G bands", required=False, default=False)
parser.add_argument("--cellular_uplink_5G", type=bool, help="Enable recoroding in Cellular Uplink 5G bands", required=False, default=False)
parser.add_argument("--band_0_9", type=bool, help="Enable recording in 900 MHz ISM band", required=False, default=False)
parser.add_argument("--band_2_4", type=bool, help="Enable recording in 2.4 MHz ISM band", required=False, default=False)
parser.add_argument("--band_5_8", type=bool, help="Enable recording in 5.8 MHz ISM band", required=False, default=False)
parser.add_argument("--visual", type=bool, help="Enable Visual Representation of the RF Signal", required=False, default=False)
parser.add_argument("--energy", type=bool, help="Enable Prints of Signal Energy Power", required=False, default=False)
parser.add_argument("--energy_threshold", type=float, help="Will visualize signals above the energy threshold (--energy and --visaul needs to be True)", required=False, default=40)
parser.add_argument("--run_model", type=bool, help="Enable Evaluation of the model", required=False, default=False)

args = parser.parse_args()
if __name__ == "__main__":
    tb = SDR()
    cf = []
    # 3G Cellular Bands -> uplink (B1, B5, B8)
    if args.cellular_uplink_3G:
        list_cellular_uplink_3G = [1935.36e6, 1966.08e6, 836.5e6, 895.36e6, 899.64e6]
        for i in list_cellular_uplink_3G:
            cf.append(i)
    # 4G Cellular Bands -> uplink (B1, B3, B5, B7, B28)
    if args.cellular_uplink_4G:
        list_cellular_uplink_4G = [1935.36e6, 1966.08e6, 1725.36e6, 1756.08e6, 1769.64e6, 836.5e6, 718.36e6, 732.64e6]
        for i in list_cellular_uplink_4G:
            cf.append(i)
    # -------------------------
    # ISM BANDS
    if args.band_0_9:
        cf.append(915e6)
    if args.band_2_4:
        list_2_4 = [2415.36e6, 2446.08e6, 2476.8e6, 2484.84e6]
        for i in list_2_4:
            cf.append(i)
    if args.band_5_8:
        list_5_8 = [5740.36e6, 5771.08e6, 5801.8e6, 5832.52e6, 5863.24e6, 5893.96e6, 5908.64e6]
        for i in list_5_8:
            cf.append(i)
    # -------------------------
    if cf == []:
        logger.error(f"No Frequncies bands are chosen")
        raise ValueError
    if args.run_model:
        from model.inference import *
        inference = Live_Inference()
    
    receive_time = args.time
    count = 0
    gain = args.gain
    drone = args.folder
    energy_threshold = args.energy_threshold
    logger.info(f'The Directory is {args.directory}')
    logger.info(f'The List of frequencies is {cf}')
    logger.info(f'The SDR gain is {gain}')
    logger.info(f'The Folder name is {drone}')
    logger.info(f'The Record time is {receive_time}')
    logger.info(f'Energy Threshold is {energy_threshold} dBm')
    time.sleep(5)
    while True:
        t1 = time.time()
        for i in cf:
            logger.info(f"************ Current Frequnecy: {(i/1000000000):.4f} GHz ************")
            now = datetime.now()
            formatted_date = now.strftime("%Y-%m-%d_%H%M%S")
            output = tb.start_recieve(i, gain, receive_time)
            filename = f'{drone}_{np.int64(i)}_{formatted_date}_{count}'

            if args.run_model:
                labels = inference.main(output)

            if args.energy:
                energy_signal = energy(output)
                logger.info(f'The energy for freq : {i} is {energy_signal} dBm')

            if args.visual:
                if args.energy:
                    if energy_signal > energy_threshold:
                        visual_representation(output, tb.sample_rate, i)
                else:
                    visual_representation(output, tb.sample_rate, i)

            if args.directory and args.folder:
                create_file(args.directory, drone, filename, output)

            count+=1
        t2 = time.time()
        logger.info(f"It took {t2-t1} seconds to write {len(cf)} files")
