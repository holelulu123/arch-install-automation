import time as tt 
import numpy as np
import matplotlib.pyplot as plt

def find_angle(real_value: float, imag_value: float):
    if real_value > 0 and imag_value > 0:
        angle = np.arctan2(imag_value, real_value) * 180 / np.pi
    if real_value < 0 and imag_value > 0:
        angle = (np.arctan2(imag_value, real_value) * 180 / np.pi) + 90

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
        # plt.legend()
        max_real_value = 
        print(f'The current phase is: {phase*180/np.pi}')
        print(f'Maximum real value is: {max(np.real(freq_sig))}')
        print(f'Maximum imag value is: {max(np.imag(freq_sig))}')
        # plt.scatter(max(np.real(freq_sig)), max(np.imag(freq_sig)))
        plt.show()
