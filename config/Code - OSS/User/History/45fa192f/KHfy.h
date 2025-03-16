#ifndef __NMEA_PARSER
#define __NMEA_PARSER

void GPRMC_MessageParser(char message[]);
void GPVTG_MessageParser(char message[]);
void GPGGA_MessageParser(char message[]);
void GPGSA_MessageParser(char message[]);
void GPGSV_MessageParser(char message[]);
void GPGLL_MessageParser(char message[]);
void GPTXT_MessageParser(char message[]);


#endif