#include <stdlib.h>
#include <stdio.h>
#include "nmea_parser.h"
#include "l80_m39.h"



void GPRMC_MessageParser(char message[]){
    /*
    This function Parses GPRMC NMEA message type, and saves the new values.
    
    */
    char temp[20] = {'\0'};
    char checksum = 0;
    char temp_check = 0;
    char *ptr = message;
    __uint8_t count_words = 1;
    __uint8_t index = 0;
    __uint8_t finish = 0;
    while(*ptr){
        // Calculate checksum
        if (*ptr != '$' && *ptr != '*'){
            temp_check ^= *ptr;
        }
        // Implementation of delimiter for character ','
        if (*ptr == STOP_CHAR){
            temp[index] = '\0';
            index = 0;
            printf("word number %d is: %s\n",count_words ,temp);
            count_words++;
        }
        // Implementaiton of delimiter but now for character '*' that represent end of packet.
        else if (*ptr == FINISH_CHAR){
            temp[index] = '\0';
            index = 0;
            printf("word number %d is: %s\n",count_words ,temp);
            count_words++;    
            checksum = temp_check;
            finish = 1; // finish end of packet and now we want to catch checksum that sent to us for GPS Receiver.        
        }
        // Keeps adding the words to the temp array upuntil we reached the delimiter.
        else {
            temp[index] = *ptr;
            index++;
        }
        ptr++;
    }
    printf("Checksum is %x\n", checksum);
    


}
