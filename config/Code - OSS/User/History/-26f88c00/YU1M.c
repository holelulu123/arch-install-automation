#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STOP_CHAR ','
#define FINISH_CHAR '*'

void test_print(char* message){
    while(*message){
        printf("Message is: %c \n", *message);
        printf("pointer: %p \n", &(*message));
        message++;
    }

}

void test_bits(){
    __uint32_t first_one = (0x0000F010 >> 2);
    __uint32_t  second_one = (0x1 << 0) | (0x1 << 1);
    printf("The first one is: %x \n", first_one);
    printf("The second one is: %x \n", second_one);
    
    if(first_one & (0x1 << 0) && first_one & (0x1 << 1)){
        printf("Good \n");
    }
    else {
        printf("No Good \n");
    }

}

void string_find() {
    char message[100] = "$GPRMC,005150.800,V,,,,,0.00,0.00,050180,,,N*48";
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
        if (*ptr == STOP_CHAR){
            temp[index] = '\0';
            index = 0;
            printf("word number %d is: %s\n",fields ,temp);
            fields++;
        }
        // Implementaiton of delimiter but now for character '*' that represent end of packet.
        else if (*ptr == FINISH_CHAR){
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
    printf("Calculated checksum : %x\n", checksum);
    printf("Found Checksum      : %x\n", result);
    
}

void timeParser(char* UTC_Time){
    /*
    This function take as an argument literal string that represent the UTC time in a 
    hhmmdd.sss format, extract the values and saves them in a global variables
    */
    // for (int i = 0; UTC_Time[i] != '\0'; i++){
        // printf("index is: %d\n", i);
        printf("char is: %c \n", UTC_Time[0]);
    // }
    // int index = 0;
    // int nmea_hour = (UTC_Time[0] - '0') * 10 + (UTC_Time[1] - '0'); 
    // int nmea_minute = (UTC_Time[2] - '0') * 10 + (UTC_Time[3] - '0');
    // int nmea_second = (UTC_Time[4] - '0') * 10 + (UTC_Time[5] - '0');
    // int nmea_millisecond = (UTC_Time[7] - '0') * 100 + (UTC_Time[8] - '0') * 10 + (UTC_Time[9] - '0') ;

}

void longitudeParser(char longtitude[]){
    /*
    This function Takes as an argument literal string that represent the longtitude in the format of 
    ddmm.mmmm(degree and minutes) and saved the value in a float value in a global variable.
    first we need to convert it from ddm format to dd 
    this formula is dd + (mm.mmm / 60)
    for example: 3150.7238 = 31 + (50.7238 / 60) = 31.845396
    
    */
    
    if (longtitude[0] != '\0'){
        float t1 = (longtitude[0] - '0') * 10 + (longtitude[1] - '0');
        float t2 = (longtitude[2] - '0') * 10 + 
        (longtitude[3] - '0') + 
        (longtitude[5] - '0') * 0.1 +
        (longtitude[6] - '0') * 0.01 +
        (longtitude[7] - '0') * 0.001 +
        (longtitude[8] - '0') * 0.0001;
        float final = t1 + (t2/60);
        printf("first number is: %f\n", t1);
        printf("second number is: %f\n", t2);
        printf("final number is: %f\n", final);
    }
}



int main() {
    char longtitude[20] = {'\0'};
    longitudeParser(longtitude);
}
