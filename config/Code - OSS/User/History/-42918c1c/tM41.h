#ifndef __L80_M39
#define __L80_M39
#include <stdio.h>

#define NMEA_MESSAGE_ID_GPRMC  "GPRMC"
#define NMEA_MESSAGE_ID_GPVTG  "GPRMC"
#define NMEA_MESSAGE_ID_GPGGA  "GPRMC"
#define NMEA_MESSAGE_ID_GPGSA  "GPRMC"
#define NMEA_MESSAGE_ID_GPGSV  "GPRMC"
#define NMEA_MESSAGE_ID_GPGLL  "GPRMC"
#define NMEA_MESSAGE_ID_GPTXT  "GPRMC"


#define NMEA_MESSAGE_ID_SIZE 5
#define NMEA_MESSAGE_ID_START_INDEX 1
#define NMEA_MESSAGE_FIELD_END_SIGN (0x2c)


// GPS Parameters
extern float longtitude;
extern float latitude;
extern __uint8_t antenna;
extern float velocity;


void GPS_NMEA_MessageParser(char message[]);

#endif
