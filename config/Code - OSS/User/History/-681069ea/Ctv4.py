import os
import matplotlib.pyplot as plt
import numpy as np
from helpers import cross_corr_2_receivers

directory_1 = '../sdr_mock/samples'
directory_2 = '../sdr_mock/samples'

list_1 = os.listdir(directory_1)
list_2 = os.listdir(directory_2)
for i in list_1:
    for j in list_2:
        time_1 = i.split("_")[2]
        time_2 = j.split("_")[2]
        if time_1 == time_2:
            signal_1 = np.fromfile(f'{directory_1}/{i}', dtype=np.complex64)
            signal_2 = np.fromfile(f'{directory_2}/{j}', dtype=np.complex64)
            fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10,5))
            ax1.plot(signal_1)
            ax1.set_title(f"{directory_1}/{i}")
            
            ax2.plot(signal_2)   
            ax2.set_title(f"{directory_2}/{j}")
            
            plt.tight_layout()
            plt.show()
            
            cross_corr_2_receivers(signal_1, signal_2, 30.72e6)