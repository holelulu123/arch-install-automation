#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "l80_m39.h"
#include "uart.h"
#include "nmea_parser.h"


extern float nmea_latitude = 0; 
extern int nmea_data_valid = 0; 
extern int nmea_hour = 0; 
extern int nmea_minute = 0;
extern int nmea_second = 0; 
extern int nmea_millisecond = 0; 


void DataValid_Check(char data_valid[]){
    /*
    Checks if the data is valid.
    'V' - data is invalid
    'A' - data is valid
    */
    if (strcmp(data_valid, NMEA_DATA_INVALID) == 0){
        nmea_data_valid = 0;
    }
    else if (strcmp(data_valid, NMEA_DATA_VALID) == 0){
        nmea_data_valid = 1;
    }

}

void timeParser(char UTC_Time[]){
    /*
    This function take as an argument literal string that represent the UTC time in a 
    hhmmdd.sss format, extract the values and saves them in a global variables
    */
    uint8_t index = 0;
    nmea_hour = (UTC_Time[0] - '0') * 10 + (UTC_Time[1] - '0'); 
    nmea_minute = (UTC_Time[2] - '0') * 10 + (UTC_Time[3] - '0');
    nmea_second = (UTC_Time[4] - '0') * 10 + (UTC_Time[5] - '0');
    nmea_millisecond = (UTC_Time[7] - '0') * 100 + (UTC_Time[8] - '0') * 10 + (UTC_Time[9] - '0');

}

void LatitudeParser(char latitude[]){
    /*
    This function Takes as an argument literal string that represent the latitude in the format of 
    ddmm.mmmm(degree and minutes) and saved the value in a float value in a global variable.
    first we need to convert it from ddm format to dd 
    this formula is dd + (mm.mmm / 60)
    for example: 3150.7238 = 31 + (50.7238 / 60) = 31.845396
    
    */
    
    if (latitude[0] != '\0'){
        float t1 = (latitude[0] - '0') * 10 + (latitude[1] - '0');
        float t2 = (latitude[2] - '0') * 10 + 
        (latitude[3] - '0') + 
        (latitude[5] - '0') * 0.1 +
        (latitude[6] - '0') * 0.01 +
        (latitude[7] - '0') * 0.001 +
        (latitude[8] - '0') * 0.0001;
        float final_lat = t1 + (t2/60);
        nmea_latitude = final_lat;
    }

}


void GPS_NMEA_MessageNavigator(char message[]){
    /*
    This function gets a NMEA message and navigates to the right parser respetively to the
    Message ID. 

    There are a few different messages that each message has his values types
    Here we will step by step understand how to parse the full message.
    We need to figure out what is the message ID, and then Parse the message at the 
    rigth way. every message starts with $GP and then 3 letters that represent the message type
    options are: RMC \ VTG \ GGA \ GSA \ GSV \ GLL \ TXT.
    $GPRMC
    */
    char message_id[10];
    __uint8_t index = 0;
    while(index < NMEA_MESSAGE_ID_SIZE){
        message_id[index] = message[index]; 
        index++;
    }
    message_id[index] = '\0';
    if (strcmp(message_id, NMEA_MESSAGE_ID_GPRMC) == 0){
        UART_printf("%s\r\n",message);
        GPRMC_MessageParser(message);
        // The prints needs to be deleted, but used here as a test.
        UART_printf("Hour: %d | Minute: %d | Second: %d | miliseconds: %d\r\n",nmea_hour, nmea_minute, nmea_second, nmea_millisecond);
        UART_printf("Data Valid is: %d\r\n", nmea_data_valid);
        UART_printf("Latitude is: %f\r\n", nmea_latitude);
    }
    


}
    
