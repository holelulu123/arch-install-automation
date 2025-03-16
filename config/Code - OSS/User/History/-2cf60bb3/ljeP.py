import numpy as np
import matplotlib.pyplot as plt

def receiver_sensitivity(bw, snr, nf):

    '''
    Sensitivity of a Receiver (MDS) 
    this is the calculation which presents the lowerst recieved power which your reciver can detect.
    bw - bandwidth of the reciever in Hz
    snr - Required SNR in db of your reciever. 
    nf_total - total noise figure of your RF Chain

    return the MDS of the reciever 
    '''
    # User Configurable Settings
    t0 = 310  # Thermal noise temperature in Kelvin
    k = 1.38e-23  # Boltzmann's constant

    # Calculating Noise Floor
    noise_floor = 10 * np.log10(k * t0 * 1000) + nf + 10 * np.log10(bw)

    # Calculating Minimum Detectable Signal (MDS)
    mds = noise_floor + snr
    return mds

def nf_calculator(amp_gain_array_db, amp_noise_array_db): 
    '''
    Noise Figure and Total Gain Calculation ---
    amp_gain_array_db - an array that each element represent the gain in dB of the component for for 
                        example [3,6,10] - 3 components, first element has 3 dB gain, second has 6 dB gain....
    amp_noise_array_db - an array that each element represent the noise figure in dB of the component for for 
                        example [3,6,10] - 3 components, first element has 3 dB noise figure, second has 6 dB noise figure....

    returns the total noise figure and total gain of the RF chain.
    '''
    
    # User Configurable Settings

    # Converting dB values to linear scale
    amp_gain_array_linear = 10 ** (amp_gain_array_db / 10)
    amp_noise_array_linear = 10 ** (amp_noise_array_db / 10)

    # Calculating total gain in dB
    gain_total = np.sum(amp_gain_array_db)

    # Calculating total noise figure
    noise_total = amp_noise_array_linear[0]
    for i in range(1, len(amp_gain_array_db)):
        sum_gain = np.prod(amp_gain_array_linear[:i])
        noise_total += (amp_noise_array_linear[i] - 1) / sum_gain

    # Converting total noise figure back to dB
    noise_total_db = 10 * np.log10(noise_total)
    return noise_total_db, gain_total

def friis_transmittion_equation(frequency, power_transmit, transmit_antenna_gain, receive_antenna_gain):
    '''
    --- Friis Transmission Path Loss Calculator ---
    frequency - frequnecy you are looking to investigate in Hz
    power_transmit - transmit power of the transmitter in dBm 
    transmit_antenna_gain - antenna gain of the tranmistter in dBi
    receive_antenna_gain - antenna gain of the reciever in dBi

    the function shows graph of the receieved power relative to the distance
    '''
    # User Configurable Settings
    c = 299792458
    max_distance = 50000  # in meters

    # Calculating Wavelength
    wavelength = c / frequency

    # Initializing power received array
    power_receive = np.zeros(max_distance)

    # Calculating the received power over distance
    for i in range(1, max_distance + 1):
        power_receive[i - 1] = power_transmit + transmit_antenna_gain + receive_antenna_gain + 20 * np.log10(wavelength / (4 * np.pi * i))

    # Plotting the received power vs. distance
    plt.plot(range(1, max_distance + 1), power_receive)
    plt.title("Received Power vs. Distance")
    plt.xlabel("Distance (meters)")
    plt.ylabel("Received Power (dBm)")
    plt.grid(True)
    plt.show()

def distance_between_recievers(d):
    '''
    The function returns the time it took for the signal to propogate at that distance
    t - the time that the signal travels in seconds
    '''
    c = 299792458 # m/s 
    t = d / c
    return t   

nf_low_band = nf_calculator([18, 20],[1, 18]) 
nf_high_band = nf_calculator([24.5, 20], [1.5,13])
print(f'NF for Low band is: {nf_low_band}')
print(f'NF for High band is: {nf_high_band}')
sen_low_band = receiver_sensitivity(30.72e6, 0, nf_low_band)
sen_high_band = receiver_sensitivity(30.72e6, 0, nf_high_band)
print(f'Receiver Sensitivity for Low band: {sen_low_band}')
print(f'Receiver Sensitivity for High band: {sen_high_band}')
