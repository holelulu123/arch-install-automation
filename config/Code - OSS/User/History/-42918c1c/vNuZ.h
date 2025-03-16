#ifndef __L80_M39
#define __L80_M39
#include <stdio.h>

#define NMEA_MESSAGE_ID_GPRMC "GPRMC"
#define NMEA_MESSAGE_ID_GPVTG "GPVTG"
#define NMEA_MESSAGE_ID_GPGGA "GPGGA"
#define NMEA_MESSAGE_ID_GPGSA "GPGSA"
#define NMEA_MESSAGE_ID_GPGSV "GPGSV"
#define NMEA_MESSAGE_ID_GPGLL "GPGLL"
#define NMEA_MESSAGE_ID_GPTXT "GPTXT"


#define NMEA_MESSAGE_ID_SIZE 5
#define NMEA_MESSAGE_ID_START_INDEX 1
#define NMEA_MESSAGE_FIELD_END_SIGN (0x2c)


// GPS Parameters
extern float* longtitude;
extern float* latitude;
extern __uint8_t* antenna;
extern float* velocity;


void GPS_NMEA_MessageParser(char message[]);

#endif
