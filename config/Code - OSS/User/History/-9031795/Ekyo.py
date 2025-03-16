import numpy as np

# Variables
bandwidth = 500e3 # in Hz
spreading_factor = 12 # between 6 to 12
coding_rate = 1 # 1 - 4/5 | 2 - 4/6 | 3 - 4/7 | 4 - 4/8
payload_length = 11 # between 1 - 255
preamble_length = 10
implicit_header = 1
low_data_rate_optimize = 0
crc = 1 # either 1 or zero

# Symbol Rate
rs = bandwidth / pow(2,spreading_factor)
ts = 1 / rs

# Premable time
preamble_time = (preamble_length + 4.25) * ts

# Payload Time
n_payload = 8 + np.max(
    np.ceil(
    (8 * payload_length - 4 * spreading_factor + 28 + 16 * crc - 20 * implicit_header)/(4 * (spreading_factor - 2 * low_data_rate_optimize))*(coding_rate + 4))
    , 0)
time_payload = n_payload * ts

total_time = time_payload + preamble_time
print(f"Total packaet time is {total_time * 1000} Miliseconds")
