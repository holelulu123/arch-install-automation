#! venv/bin/python

import numpy as np
from link_budget import *


def lora_airtime_calc(
    bandwidth: float,
    spreading_factor: int,
    coding_rate: int,
    payload_length: int,
    preamble_length: int,
    implicit_header: int,
    low_data_rate_optimize: int,
    crc_on: int,
):
    """
    This function calculates a LoRa signal air time in Seconds

    Args:
    ------------
    bandwidth: float
        bandwidth of the signal in Hz
    spreading_factor: int
        spreading_factor of the signal, a number between 6 - 12
    coding_rate: int
        coding rate of signal, 1 - 4/5 | 2 - 4/6 | 3 - 4/7 | 4 - 4/8
    payload_length: int
        payload data transmitted in Bytes
    preamble_length: int
        premable length
    implicit_header: int
        whether to choose to add an header to the communication or not, 1 - means no header, shorter tranmsition time, 0 - means there's header, longer transmtion time
    low_data_rate_optimize: int
        the IC optimizes and lower the tranmsittion time if it detects low data rate, 0 - disabled, 1 - enabled
    crc_on: int
        1 - crc enabled and adds another data to the tranmsittion to be checked in the recevier

    """
    # Symbol Rate
    rs = bandwidth / pow(2, spreading_factor)
    ts = 1 / rs

    # Premable time
    preamble_time = (preamble_length + 4.25) * ts

    # Payload Time
    n_payload = 8 + np.max(
        np.ceil(
            (
                8 * payload_length
                - 4 * spreading_factor
                + 28
                + 16 * crc_on
                - 20 * implicit_header
            )
            / (4 * (spreading_factor - 2 * low_data_rate_optimize))
            * (coding_rate + 4)
        ),
        0,
    )
    time_payload = n_payload * ts

    total_time = time_payload + preamble_time
    return total_time

bws = [7.8e3, 10.4e3, 15.6e3, 20.8e3, 31.25e3, 41.7e3, 62.5e3, 125e3, 250e3, 500e3]
spreading_factor_snr = [[6, -5], [7, -7.5], [8, -10], [9, -12.5], [10, -15], [11, -17.5], [12, -20]]
for bw in bws:
    for sf in spreading_factor_snr:
        x = receiver_sensitivity(bw, sf[1], 1)
        print(f"Receiver Sensitivity for bw: {bw} is: {x} dBm")