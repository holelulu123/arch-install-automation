import os
import numpy as np


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

def ()