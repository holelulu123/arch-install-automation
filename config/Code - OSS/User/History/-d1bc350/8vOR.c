#include <stdlib.h>
#include <stdio.h>
#include "nmea_parser.h"
#include "l80_m39.h"



void GPRMC_MessageParser(char message[]){
    /*
    This function Parses GPRMC NMEA message type, and saves the new values.
    
    */
   char temp[20] = {'\0'}; // Temporary array that contains the data of each field. 
   char checksum = 0; // Final calculated checksum
   char temp_check = 0; // Temporary checksum
   char *ptr = message; // Pointer to the input message 
   __uint8_t fields = 0; // Counts the number of fields we have been through
   __uint8_t index = 0; // counter for each field, every field it gets reset
   while(*ptr){
       // Calculate checksum
       if (*ptr != '$' && *ptr != '*'){
           temp_check ^= *ptr;
       }
       // Implementation of delimiter for character ','
       if (*ptr == NMEA_MESSAGE_FIELD_END_SIGN){
           temp[index] = '\0';
            index = 0;
            switch (fields){
                // UTC Time
                case 1:
                    timeParser(temp);
                    break;
                // Data Valid 
                case 2:
                    DataValid_Check(temp);
                    break;
                    
                    
           }
           // Switch needs to be here
           fields++;
       }
       // Implementaiton of delimiter but now for character '*' that represent end of packet.
       else if (*ptr == NMEA_MESSAGE_FINISH_SIGN){
           temp[index] = '\0';
           index = 0;
           printf("word number %d is: %s\n",fields ,temp);
           fields++;    
           checksum = temp_check;
       }
       // Keeps adding the words to the temp array upuntil we reached the delimiter.
       else {
           temp[index] = *ptr;
           index++;
       }
       ptr++;
   }
   temp[index] = '\0';
   unsigned int checksum_hex; // the checksum that sent to us for the receiver
   sscanf(temp, "%x", &checksum_hex);  // Convert string to hex
   
   char result = (char)checksum_hex;  // Cast to char
   if (result == checksum){
       printf("Equal\n");
   }
   else {
       printf("Not Equal\n");
   }
    


}
