#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "TFT_ST7735.h"
#include "I2C_Support.h"
#include "Setup_Time.h"
#include "Main_Screen.h"
#include "Main.h"

extern unsigned char second, minute, hour, dow, day, month, year;
extern unsigned char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;

extern int INT0_flag, INT1_flag, INT2_flag;

extern char setup_time[];
extern char setup_date[];

extern char buffer[31];
extern char *nbr;
extern char *txt;
extern char tempC[];
extern char tempF[];
extern char time[];
extern char date[];

void Setup_Time(void)
{
    char Key_Next_Flag, Key_Up_Flag, Key_Dn_Flag;
    char Select_Field;
    Select_Field = 0;
    DS3231_Read_Time();                                                                             // Read time
    setup_second = bcd_2_dec(second);                                                               // Set setup time as current time
    setup_minute = bcd_2_dec(minute);
    setup_hour = bcd_2_dec(hour);
    setup_day = bcd_2_dec(day);
    setup_month = bcd_2_dec(month);
    setup_year = bcd_2_dec(year);
    Initialize_Setup_Time_Screen();                                                                 // Initialize setup screen
    Update_Setup_Time_Screen();                                                                     // Update screen with latest information

    while (enter_setup == 1)
    {
        if (INT0_flag == 1)                                                                         // if software INT0 flag is set
        {
            INT0_flag = 0;                                                                          // clear the flag
            Key_Up_Flag = 1;                                                                        // set increment flag
        }
        if (INT1_flag == 1)                                                                         // if software INT1 flag is set
        {
            INT1_flag = 0;                                                                          // clear the flag
            Key_Dn_Flag = 1;                                                                        // set decrement flag
        }
        if (INT2_flag == 1)                                                                         // if software INT2 flag is set
        {
            INT2_flag = 0;                                                                          // clear the flag
            Key_Next_Flag = 1;                                                                      // set next flag
        }

        if (Key_Up_Flag == 1)                                                                       // If user increments
        {
            switch (Select_Field)                                                                   // Increment the proper field
            {
                case 0:
                    setup_hour++;
                    if (setup_hour == 24) setup_hour = 0;
                    break;

                case 1:
                    setup_minute++;
                    if (setup_minute == 60) setup_minute = 0;
                    break;

                case 2:
                    setup_second++;
                    if (setup_second == 60) setup_second = 0;
                    break;

                case 3:
                    setup_month++;
                    if (setup_month == 13) setup_month = 1;
                    break;

                case 4:
                    setup_day++;
                    if (setup_day == 32) setup_day = 1;
                    break;

                case 5:
                    setup_year++;
                    if (setup_year == 100) setup_year = 0;
                    break;

                default:
                    break;
            }
            Update_Setup_Time_Screen();                                                             // Update screen with latest info
            Key_Up_Flag = 0;                                                                        // Clear the increment flag
        }

        if (Key_Dn_Flag == 1 )                                                                      // If user decrements
        {
            switch (Select_Field)                                                                   // Decrement the proper field
            {
                case 0:
                    if (setup_hour == 0) setup_hour = 23;
                    else --setup_hour;
                    break;

                case 1:
                    if (setup_minute == 0) setup_minute = 59;
                    else --setup_minute;
                    break;

                case 2:
                    if (setup_second == 0) setup_second = 59;
                    else --setup_second;
                    break;

                case 3:
                    if (setup_month == 1) setup_month = 12;
                    else --setup_month;
                    break;

                case 4:
                    if (setup_day == 1) setup_day = 31;
                    else --setup_day;
                    break;

                case 5:
                    if (setup_year == 0) setup_year = 99;
                    else --setup_year;
                    break;

                default:
                break;
            }
            Update_Setup_Time_Screen();                                                             // Update screen with latest info
            Key_Dn_Flag = 0;                                                                        // Clear the decrement flag
        }

        if (Key_Next_Flag == 1 )                                                                    // If user switches
        {
            Select_Field++;                                                                         // Move to next field
            if (Select_Field == 6) Select_Field = 0;                                                // Wrap around if necessary
            Update_Setup_Screen_Cursor(Select_Field);                                               // Update cursor
            Key_Next_Flag = 0;                                                                      // Clear flag
        }
    }

    DS3231_Write_Time();                                                                            // Write time
    DS3231_Read_Alarm_Time();                                                                       // Read alarm time
    DS3231_Read_Time();                                                                             // Read current time
    Initialize_Screen();                                                                            // Initialize the screen before returning
}

void Initialize_Setup_Time_Screen(void)
{
    fillScreen(ST7735_BLACK);                                                                       // Fills background of screen with color passed to it

    strcpy(txt, "ECE3301 Fall20 Final\0");                                                         // Text displayed
    drawtext(start_x , start_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);                           // X and Y coordinates of where the text is to be displayed


    strcpy(txt, "Time Setup\0");
    drawtext(start_x+3 , start_y+15, txt, ST7735_MAGENTA, ST7735_BLACK, TS_2);

    strcpy(txt, "Time");
    drawtext(time_x  , time_y , txt, ST7735_BLUE   , ST7735_BLACK, TS_1);

    fillRect(data_time_x-1, data_time_y+16, 25,2,ST7735_CYAN);
    strcpy(txt, "Date");
    drawtext(date_x  , date_y , txt, ST7735_RED    , ST7735_BLACK, TS_1);
}

void Update_Setup_Time_Screen(void)
{
    setup_time[0]  = (setup_hour   / 10)  + '0';                                                    // Setup Hour   MSD
    setup_time[1]  = (setup_hour   % 10)  + '0';                                                    // Setup Hour   LSD
    setup_time[3]  = (setup_minute / 10)  + '0';                                                    // Setup Minute MSD
    setup_time[4]  = (setup_minute % 10)  + '0';                                                    // Setup Minute LSD
    setup_time[6]  = (setup_second / 10)  + '0';                                                    // Setup Second MSD
    setup_time[7]  = (setup_second % 10)  + '0';                                                    // Setup Second LSD
    setup_date[0]  = (setup_month  / 10)  + '0';                                                    // Setup month  MSD
    setup_date[1]  = (setup_month  % 10)  + '0';                                                    // Setup month  LSD
    setup_date[3]  = (setup_day    / 10)  + '0';                                                    // Setup day    MSD
    setup_date[4]  = (setup_day    % 10)  + '0';                                                    // Setup day    LSD
    setup_date[6]  = (setup_year   / 10)  + '0';                                                    // Setup year   MSD
    setup_date[7]  = (setup_year   % 10)  + '0';                                                    // Setup year   LSD

    drawtext(data_time_x, data_time_y, setup_time, ST7735_CYAN, ST7735_BLACK, TS_2);
    drawtext(data_date_x, data_date_y, setup_date, ST7735_GREEN, ST7735_BLACK, TS_2);
}

void Update_Setup_Screen_Cursor(char cursor_position)
{
    char xinc = 36;
    char yinc = 30;
    switch (cursor_position)
    {
        case 0:
            fillRect(data_time_x-1+2*xinc, data_time_y+16+yinc, 25,2,ST7735_BLACK);
            fillRect(data_time_x-1+2*xinc, data_time_y+16, 25,2,ST7735_BLACK);
            fillRect(data_time_x-1, data_time_y+16, 25,2,ST7735_CYAN);
            break;

        case 1:
            fillRect(data_time_x-1, data_time_y+16, 25,2,ST7735_BLACK);
            fillRect(data_time_x-1+xinc, data_time_y+16, 25,2,ST7735_CYAN);
            break;

        case 2:
            fillRect(data_time_x-1+xinc, data_time_y+16, 25,2,ST7735_BLACK);
            fillRect(data_time_x-1+2*xinc, data_time_y+16, 25,2,ST7735_CYAN);
            break;

        case 3:
            fillRect(data_time_x-1+2*xinc, data_time_y+16, 25,2,ST7735_BLACK);
            fillRect(data_time_x-1, data_time_y+16+yinc, 25,2,ST7735_CYAN);
            break;

       case 4:
            fillRect(data_time_x-1, data_time_y+16+yinc, 25,2,ST7735_BLACK);
            fillRect(data_time_x-1+xinc, data_time_y+16+yinc, 25,2,ST7735_CYAN);
            break;

       case 5:
            fillRect(data_time_x-1+xinc, data_time_y+16+yinc, 25,2,ST7735_BLACK);
            fillRect(data_time_x-1+2*xinc, data_time_y+16+yinc, 25,2,ST7735_CYAN);
            break;
    }
}




