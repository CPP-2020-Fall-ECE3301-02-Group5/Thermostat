#ifndef MAIN_H
#define MAIN_H

#define _XTAL_FREQ      8000000

#define ACK             1
#define NAK             0

#define ACCESS_CFG      0xAC
#define START_CONV      0xEE
#define READ_TEMP       0xAA
#define CONT_CONV       0x02

#define TFT_DC          PORTDbits.RD0
#define TFT_CS          PORTDbits.RD1
#define TFT_RST         PORTDbits.RD2

#define enter_setup     PORTEbits.RE0
#define setup_sel0      PORTEbits.RE1
#define setup_sel1      PORTEbits.RE2

#define SEC_LED         PORTCbits.RC4

#define FANEN_LED       PORTAbits.RA5
#define RTC_ALARM_NOT   PORTAbits.RA4

#endif /* MAIN_H */