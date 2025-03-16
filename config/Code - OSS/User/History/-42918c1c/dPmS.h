#ifndef __L80_M39
#define __L80_M39
#include <stdio.h>

#define NMEA_MESSAGE_ID_GPRMC "$GPRMC"
#define NMEA_MESSAGE_ID_GPVTG "$GPVTG"
#define NMEA_MESSAGE_ID_GPGGA "$GPGGA"
#define NMEA_MESSAGE_ID_GPGSA "$GPGSA"
#define NMEA_MESSAGE_ID_GPGSV "$GPGSV"
#define NMEA_MESSAGE_ID_GPGLL "$GPGLL"
#define NMEA_MESSAGE_ID_GPTXT "$GPTXT"


#define NMEA_MESSAGE_ID_SIZE 6
#define NMEA_MESSAGE_ID_START_INDEX 0
#define NMEA_MESSAGE_FIELD_END_SIGN ","
#define NMEA_MESSAGE_END_SIGN "*"

#define NMEA_MESSAGE_START_DECODING_INDEX 

// GPS Parameters 
extern float* nmea_longtitude;
extern float* nmea_latitude;
extern float* nmea_altitude;
extern float* nmea_velocity;
extern int nmea_number_of_sv;
extern int nmea_fixed_3d;
extern int nmea_antenna;
extern int nmea_day;
extern int nmea_month;
extern int nmea_year;

extern int nmea_hour;
extern int nmea_minute;
extern int nmea_second;
extern int nmea_millisecond;


void GPS_NMEA_MessageParser(char message[]);

#endif
