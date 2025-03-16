# SDR MOCK
This project is supposed to mimic the SDR service (node) and send packets to the server.
We simulate in here an outdoor enviorment with various parameters like:
    - Bandwidth
    - Number of packets
    - Time of signal
    - Different delta T to change position of Transmitter

# How to Start
1. install virtual enviorment -> python3 -m venv venv
2. enter the virtual enviorment -> source venv/bin/activate
3. install packages through requirements.txt file ->  pip install -r requirements.txt
4. In order to run the mock -> ./mock --h, this command will output you the args of the mock
5. enter the right args and you are ready to go.

#### NOTE:
The development was on python version 3.13.1 
    

