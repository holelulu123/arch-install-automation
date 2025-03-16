#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "l80_m39.h"
#include "uart.h"
#include "nmea_parser.h"

extern int data_valid = 0; 
extern int nmea_hour = 0; 
extern int nmea_minute = 0;
extern int nmea_second = 0; 
extern int nmea_millisecond = 0; 

void dataValid_Check(char data_valid[]){
    /*
    Checks if the data is valid.
    'V' means that the data is invalid
    */

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

void longitudeParser(char longitude[]){
    /*
    This function Takes as an argument literal string that represent the longitude in the format of 
    ddmm.mmmm(degree and minutes) and saved the value in a float value in a global variable.
    
    */
}


void GPS_NMEA_MessageParser(char message[]){
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
        GPRMC_MessageParser(message);
        UART_printf("Hour: %d | Minute: %d | Second: %d | miliseconds: %d\r\n",nmea_hour, nmea_minute, nmea_second, nmea_millisecond);

    }
    


}
    
