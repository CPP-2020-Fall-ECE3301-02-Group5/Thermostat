#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>

#include "Main.h"
#include "NEC_Transmitter.h"
#include "I2C_Support.h"
#include "I2C_Soft.h"
#include "TFT_ST7735.h"
#include "Interrupt.h"
#include "Main_Screen.h"
#include "Setup_Alarm_Time.h"
#include "Setup_Fan_Temp.h"
#include "Setup_Time.h"

#pragma config OSC      =   INTIO67
#pragma config BOREN    =   OFF
#pragma config WDT      =   OFF
#pragma config LVP      =   OFF
#pragma config CCP2MX   =   PORTBE

void Initialize_Screen(void);
void Update_Screen(void);
void Do_Init(void);
float read_volt();
int get_duty_cycle(int,int);
int get_RPM();
void Monitor_Fan();
void Turn_Off_Fan();
void Turn_On_Fan();
unsigned int get_full_ADC();
void Get_Temp(void);
void Update_Volt(void);
void Test_Alarm(void);
void Activate_Buzzer();
void Deactivate_Buzzer();

void Main_Screen(void);
void Do_Setup(void);
void do_update_pwm(char);
void Set_RGB_Color(char color);

char buffer[31]         = "ECE3301 Fall20 Final\0";
char *nbr;
char *txt;
char tempC[]            = "+25";
char tempF[]            = "+77";
char time[]             = "00:00:00";
char date[]             = "00/00/00";
char alarm_time[]       = "00:00:00";
char Alarm_SW_Txt[]     = "OFF";
char Fan_SW_Txt[]       = "OFF";                // text storage for Heater Mode
char Fan_Set_Temp_Txt[] = "075F";
char Volt_Txt[]         = "0.00V";              // text storage for Volt
char DC_Txt[]           = "000";                // text storage for Duty Cycle value
char RTC_ALARM_Txt[]    = "0";                  //
char RPM_Txt[]          = "0000";               // text storage for RPM

char setup_time[]       = "00:00:00";
char setup_date[]       = "01/01/00";
char setup_alarm_time[] = "00:00:00";
char setup_fan_text[]   = "075F";

signed int DS1621_tempC, DS1621_tempF;

int INT0_flag, INT1_flag, INT2_flag, Tach_cnt;
int ALARMEN;
int FANEN;
int alarm_mode, MATCHED, color;
unsigned char second, minute, hour, dow, day, month, year, old_sec;
unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;
unsigned char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;
unsigned char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
unsigned char setup_fan_temp = 75;
float volt;
int duty_cycle;
int rpm;

int Tach_cnt = 0;


void putch (char c)
{
    while (!TRMT);
    TXREG = c;
}

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x70;
}

void Init_ADC()
{
    ADCON0 = 0x01;                                                                                  // AN0, ADC on
    ADCON1 = 0x0E;                                                                                  // VREF = Vdd-Vss, all DIO except AN0
    ADCON2 = 0xA9;                                                                                  // Right justified (10 bits), 12 Tad, Fosc/8
}

void Init_IO()
{
    TRISA = 0xDF;                                                                                   // set PORTA 1101 1111
    TRISB = 0xF7;                                                                                   // set PORTB 1111 0111
    TRISC = 0xC3;                                                                                   // set PORTC 1100 0011
    TRISD = 0x00;                                                                                   // set PORTD all output
    TRISE = 0x07;                                                                                   // set PORTE 0000 0111
}

void Do_Init()                                                                                      // Initialize the ports
{
    init_UART();                                                                                    // Initialize the uart
    OSCCON = 0x70;                                                                                  // Set oscillator to 8 MHz
    Init_ADC();
    Init_IO();
    RBPU = 0;

    TMR1L = 0x00;                                                                                   // clear the count
    T1CON = 0x03;                                                                                   // start Timer1 as counter of number of pulses

    T0CON = 0x03;                                                                                   // Timer 0, 16-bit mode, prescaler 1:16
    TMR0L = 0xDB;                                                                                   // set the lower byte of TMR
    TMR0H = 0x0B;                                                                                   // set the upper byte of TMR

    INTCONbits.TMR0IF = 0;                                                                          // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;                                                                           // Turn on the Timer 0
    INTCONbits.TMR0IE = 1;                                                                          // Enable Timer 0 interrupt
    Init_Interrupt();                                                                               // initialize the other interupts

    I2C_Init(100000);
    DS1621_Init();

    MATCHED = 0;
}

void main()
{
    Do_Init();                                                                                      // Initialization

    txt = buffer;

    Initialize_Screen();

    old_sec = 0xff;
    Turn_Off_Fan();
//  DS3231_Write_Initial_Alarm_Time();                                                              // uncommented this line if alarm time was corrupted
    DS3231_Read_Time();                                                                             // Read time for the first time
    DS3231_Read_Alarm_Time();                                                                       // Read alarm time for the first time
    DS3231_Turn_Off_Alarm();

    while(TRUE)
    {
        if (enter_setup == 0)                                                                       // If setup switch is LOW...
        {
            Main_Screen();                                                                          // stay on main screen.
        }
        else                                                                                        // Else,
        {
            Do_Setup();                                                                             // Go to setup screen.
        }
    }
}

void Main_Screen()
{

        if (INT0_flag == 1)                                                                         // if software INT0 flag is set
        {
            INT0_flag = 0;                                                                          // clear the flag
            Turn_On_Fan();
        }
        if (INT1_flag == 1)                                                                         // if software INT1 flag is set
        {
            INT1_flag = 0;                                                                          // clear the flag
            Turn_Off_Fan();
        }
        if (INT2_flag == 1)                                                                         // if software INT2 flag is set
        {
            INT2_flag = 0;                                                                          // clear the flag
            ALARMEN = !ALARMEN;
        }

    DS3231_Read_Time();                                                                             // Read time
    if (old_sec != second)
    {
        old_sec = second;

        Get_Temp();
        volt = read_volt();                                                                         // read the light sensor light's voltage and store it into the variable 'volt'
        if (FANEN) Monitor_Fan();                                                                   // control fan operation if enable

        Test_Alarm();                                                                               // call routine to handle the alarm  function

        printf ("%02x:%02x:%02x %02x/%02x/%02x ",hour,minute,second,month,day,year);
        printf ("duty cycle = %d  RPM = %d ", duty_cycle, rpm);

        Update_Screen();
    }
}

void Do_Setup()
{
            if (setup_sel1) Setup_Temp_Fan();                                                       // 10 for Setup_Temp_Fan()
    else    if (setup_sel0) Setup_Alarm_Time();                                                     // 01 for Setup_Alarm_Time()
    else                    Setup_Time();                                                           // 00 for Setup_Time()
}

void Get_Temp(void)
{
    DS1621_tempC = DS1621_Read_Temp();                                                              // Read temp


    if ((DS1621_tempC & 0x80) == 0x80)
    {
        DS1621_tempC = 0x80 - (DS1621_tempC & 0x7f);
        DS1621_tempF = 32 - DS1621_tempC * 9 /5;
        printf ("Temperature = -%dC or %dF\r\n", DS1621_tempC, DS1621_tempF);
        DS1621_tempC = 0x80 | DS1621_tempC;
    }
    else
    {
        DS1621_tempF = DS1621_tempC * 9 /5 + 32;
        printf ("Temperature = %dC or %dF\r\n", DS1621_tempC, DS1621_tempF);
    }
}

void Monitor_Fan()
{
    duty_cycle = get_duty_cycle(DS1621_tempF, setup_fan_temp);
    do_update_pwm(duty_cycle);
    rpm = get_RPM();
}

float read_volt()
{
	int nStep = get_full_ADC();
    volt = nStep * 5 /1024.0;
	return (volt);
}

int get_duty_cycle(int temp, int set_temp)
{
    int dccalc = 2*(set_temp - temp);
    if (dccalc > 99)   return    99;                                                              // cap at  99
    if (dccalc <-99)   return   -99;                                                              // cap at -99
                       return dccalc;                                                             // -99 <= dccalc <= 99, return
}

int get_RPM()
{
    return Tach_cnt*60;
}

void Turn_Off_Fan()
{
    duty_cycle = 0;
    do_update_pwm(duty_cycle);
    rpm = 0;
    FANEN = 0;
    FANEN_LED = 0;
}

void Turn_On_Fan()
{
    FANEN = 1;
    FANEN_LED = 1;
}

void do_update_pwm(char l_duty_cycle)
{
    NEC_send(0x3C,l_duty_cycle);
}

unsigned int get_full_ADC()
{
    unsigned int result;
    ADCON0bits.GO=1;                                                                                // Start Conversion
    while(ADCON0bits.DONE==1);                                                                      // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                                                             // combine result of upper byte and
                                                                                                    // lower byte into result
    return result;                                                                                  // return the result.
}

void Activate_Buzzer()
{
    PR2     = 0b11111001 ;
    T2CON   = 0b00000101 ;
    CCPR2L  = 0b01001010 ;
    CCP2CON = 0b00111100 ;
}

void Deactivate_Buzzer()
{
    CCP2CON = 0x0;
    PORTBbits.RB3 = 0;
}

void Test_Alarm()
{
    // detect whether the alarm is activated, or deactivated or is waiting for the alarm to
    // happen

    // the variable ALARMEN is used as the switch that is toggled by the user
    // the variable alarm_mode stored the actual mode of the alarm. If 1, the alarm is activated. If 0, no
    // alarm is enabled yet

    // the RTC_ALRAM_NOT is the signal coming from the RTC to show whether the alarm time does match with
    // the actual time. This signal is active low when the time matches.

    // a variable MATCHED to register the event that the time match has occurred. This is needed
    // to change the color of the RGB LED

    if (ALARMEN && !alarm_mode)                                                                     // Case 1: switch is turned on but alarm_mode is not on
    {
        alarm_mode = 1;                                                                             // enable alarm_mode
        DS3231_Turn_On_Alarm();
    }
    else if (!ALARMEN && alarm_mode)                                                                // Case 2: switch is turned off but alarm mode is already on
    {
        alarm_mode = 0;                                                                             // disable alarm_mode
        MATCHED = 0;                                                                                // clear MATCHED flag
        DS3231_Turn_Off_Alarm();
    }
    else if (ALARMEN && alarm_mode)                                                                 // Case 3: switch is on and alarm_mode is on. In this case, more checks are to be performed.
    {
        if (!RTC_ALARM_NOT) 
        {
            Activate_Buzzer();                                                                      // activate buzzer when time matches
            MATCHED = 1;
        }
        if (volt>2.5) MATCHED = 0;                                                                  // light sensor blocked, clear MATCHED flag
        if (MATCHED) Set_RGB_Color(++color);                                                        // cycle thru colors if time matched
        else
        {
            DS3231_Turn_Off_Alarm();                                                                // reset RTC alarm
            DS3231_Turn_On_Alarm();
        }
    }
    
    if (!MATCHED)
    {                                                                                               // keep alarm off when alarm_mode is not on
        Deactivate_Buzzer();
        Set_RGB_Color(0);                                                                           // Turn off RGB LED
    }
}

void Set_RGB_Color(char color)
{
    PORTDbits.RD5 =  color    &0x01;                                                                // get   red segment
    PORTDbits.RD6 = (color>>1)&0x01;                                                                // get green segment
    PORTDbits.RD7 = (color>>2)&0x01;                                                                // get  blue segment
}

