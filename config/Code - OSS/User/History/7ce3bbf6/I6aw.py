import time as tt 
import numpy as np
import matplotlib.pyplot as plt

def find_angle(real_value: float, imag_value: float):
    if real_value > 0 and imag_value > 0: 
        angle = np.arctan2(imag_value, real_value) * 180 / np.pi
    elif real_value < 0 and imag_value > 0:
        angle = (np.arctan2(imag_value, real_value) * 180 / np.pi) + 90
    elif real_value < 0 and imag_value < 0:
        angle = (np.arctan2(imag_value, real_value) * 180 / np.pi) + 180
    elif real_value > 0 and imag_value < 0:
        angle = (np.arctan2(imag_value, real_value) * 180 / np.pi) + 270
    else:
        raise ValueError("No available angle")
    return angle
# s = 1 # Distance Between Two Antennas
time = 0.001 # Seconds
fs = 1e6
freq = 2e3
time_domain = np.linspace(0, time, int(time*fs)) 
phases = np.linspace(0, 2 * np.pi, 10)
freq_list = np.linspace(-fs/2, fs/2, len(time_domain))
while True:
    for phase in phases: 
        signal = np.exp(1j * (2 * np.pi * time_domain* freq + phase))
        freq_sig = np.fft.fftshift(np.fft.fft(signal))
        # plt.plot(np.real(signal), color='orange', label='real signal')
        # plt.plot(np.imag(signal), color='blue', label='imag signal')
        plt.plot(freq_list, np.real(freq_sig), color = 'orange')
        plt.plot(freq_list, np.imag(freq_sig), color = 'blue')
        
        print(f'Maximum real value is: {max(np.real(freq_sig))}')
        print(f'Minimum real value is: {min(np.real(freq_sig))}')
        print(f'Maximum imag value is: {max(np.imag(freq_sig))}')
        print(f'Minimum imag value is: {min(np.imag(freq_sig))}')
        
        if np.abs(min(np.real(freq_sig))) > max(np.real(freq_sig)):
            real_value = min(np.real(freq_sig))
        else: 
            real_value = max(np.real(freq_sig))
        
        if np.abs(min(np.imag(freq_sig))) > max(np.imag(freq_sig)):
            imag_value = min(np.imag(freq_sig))
        else: 
            imag_value = max(np.imag(freq_sig))
        print(f'imag_value is: {imag_value}')
        print(f'real_value is: {real_value}')
        
        # print(f'The current phase is: {phase*180/np.pi}')
        # print(f'Maximum real value is: {max(np.real(freq_sig))}')
        # print(f'Maximum imag value is: {max(np.imag(freq_sig))}')
        # plt.scatter(max(np.real(freq_sig)), max(np.imag(freq_sig)))
        plt.show()
