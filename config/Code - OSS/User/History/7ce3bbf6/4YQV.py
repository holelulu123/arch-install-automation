import time
import numpy as np
import matplotlib.pyplot as plt



s = 1 # Distance Between Two Antennas
time = 0.100 # Seconds
fs = 1e6
freq = 2e3
time_domain = np.linspace(0, time, int(time*fs)) 
phases = np.linspace(0, 2 * np.pi, 10)
freq_list = np.linspace(-fs/2, fs/2, len(time_domain))
while True:
    for phase in phases: 
        signal = np.exp(1j* 2*np.pi*time_domain*freq)
        freq_sig = np.fft.fftshift(np.fft.fft(signal))
        # plt.plot(np.real(signal), color='orange', label='real signal')
        # plt.plot(np.imag(signal), color='blue', label='imag signal')
        # plt.plot(freq_list, np.real(freq_sig), color = 'orange')
        # plt.plot(freq_list, np.imag(freq_sig), color = 'blue')
        # plt.legend()
        print(f'The current phase is: {phase}')
        print(f'Maximum real value is: {max(np.real(freq_sig))}')
        print(f'Maximum imag value is: {max(np.imag(freq_sig))}')
        # plt.scatter(max(np.real(freq_sig)), max(np.imag(freq_sig)))
        # plt.show()
