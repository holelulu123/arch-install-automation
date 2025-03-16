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
#define NMEA_MESSAGE_UTC_SIZE 10
#define NMEA_MESSAGE_UTCTIME_START_INDEX 7


// GPS Parameters
extern float* longtitude;
extern float* latitude;
extern float* altitude;
extern float* velocity;
extern int number_of_sv;
extern int fixed_3d;
extern int antenna;
extern int hour;
extern int minute;
extern int second;
extern int millisecond;


void GPS_NMEA_MessageParser(char message[]);

#endif
