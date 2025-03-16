import matplotlib.pyplot as plt
import numpy as np
import os

def visual_representation(raw_data, sample_rate, cf, filename=None, folder_name=None, save_fig=False):
    '''
    this function plots the signal in the time and frequnecy domain.
    
    raw_data - full IQ signal
    time_to_recv - time of the recorded rf signal in seconds
    sample_rate - sample rate of the receiver in Hz
    cf - center frequnecy of the signal in Hz 
    
    '''
    time_domain = np.linspace(0, len(raw_data)/sample_rate, len(raw_data))
    freq_domain = np.linspace(cf - (sample_rate/2), cf + (sample_rate/2), len(raw_data))
    raw_data = raw_data - np.mean(raw_data)
    signal_freq = np.abs(np.fft.fftshift(np.fft.fft(raw_data)))
    # signal_freq[np.argmax(signal_freq) - 50 : np.argmax(signal_freq) + 50] = 0  
    # plt.ion()
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(10, 5)) 


    # First subplot
    ax1.plot(time_domain*1000000, raw_data)
    ax1.set_title('Signal in the time domain')
    ax1.set_xlabel("Time")
    ax1.set_ylabel("Amplitude")

    # Second subplot
    ax2.plot(freq_domain,signal_freq)
    ax2.set_title("Signal in the frequency domain")
    ax2.set_xlabel("Frequnecy")
    ax2.set_ylabel("Amplitude")

    ax3.specgram(raw_data, NFFT=2048, Fs=sample_rate, noverlap=1024, cmap='plasma')
    ax3.set_title("Signal in Spectogram")
    ax3.set_xlabel("Time Domain")
    ax3.set_ylabel("Frequnecy")
    # Adjust layout to prevent overlap
    plt.tight_layout()

    # Show the plot
    if save_fig:
            directory = os.getcwd()

            # Create the full path for the new folder inside the current directory
            folder_path = os.path.join(directory, folder_name)
            folder_file = os.path.join(folder_path, filename)
            plt.savefig(folder_file,format='png', dpi=300)

            # Create the folder if it doesn't exist
            if not os.path.exists(folder_path):
                    os.makedirs(folder_path)
                    print(f"Folder '{folder_name}' created at {folder_path}")
            else:
                    print(f"Folder '{folder_name}' already exists at {folder_path}")
    plt.show()

def visual_check_pps(directory, fs, time):
    '''
    Takes two filenames, read the complex64 files and plots them accordinally.

    Parameters:
    ------------
    directory : String
            String representing the path of the files.
    fs : Int
            Sample rate of the recording SDR in Hz.
    time : Float
            Time of one recording of SDR in Seconds. 
    '''
    files = os.listdir(directory)
    if len(files) % 2 == 0:
        for i in range(int(len(files)/2)):
            filename1 = f'samples_yuval_{i}.bin'
            filename2 = f'samples_nir_{i}.bin'
            path_1 = os.path.join(directory, filename1)
            path_2 = os.path.join(directory, filename2)
            signal_1 = np.fromfile(path_1, dtype=np.complex64)
            signal_2 = np.fromfile(path_2, dtype=np.complex64)
            timedomain = np.linspace(0, time, int(fs * time))
            # Create a figure and subplots
            fig, axes = plt.subplots(1, 2, figsize=(10, 5), sharex=True, sharey=True)  # 1 row, 2 columns

            # Plot on the first subplot
            axes[0].plot(timedomain, np.real(signal_1), label="Real", color="blue")
            axes[0].plot(timedomain, np.imag(signal_1), label="Imagnery", color="orange")
            axes[0].set_xlabel('Time domain')
            axes[0].set_ylabel('Amplitude')
            axes[0].set_title(filename1)
            axes[0].legend()

            # Plot on the second subplot
            axes[1].plot(timedomain, np.real(signal_2), label="Real", color="blue")
            axes[1].plot(timedomain, np.imag(signal_2), label="Imagnery", color="orange")
            axes[1].set_xlabel('Time domain')
            axes[1].set_ylabel('Amplitude')
            axes[1].set_title(filename2)
            axes[1].legend()

            plt.tight_layout()
            plt.show()


# signal = np.fromfile('/opt/uhd/host/build/examples/usrp_samples.dat', dtype=np.complex64)
signal = np.fromfile('/home/holelulu/projects/uhd_plays/samples/test.bin', dtype=np.complex64)
visual_representation(signal, 1e6, 100e6)

