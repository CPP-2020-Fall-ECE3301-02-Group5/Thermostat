#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Main.h"
#include "I2C_Support.h"
#include "Setup_Fan_Temp.h"
#include "Main_Screen.h"
#include "TFT_ST7735.h"

extern char setup_fan_text[];
extern int INT0_flag, INT1_flag, INT2_flag;
extern unsigned char setup_fan_temp;
extern char *txt;

void Setup_Temp_Fan(void)
{
    char Key_Up_Flag, Key_Dn_Flag;
    char Select_Field;
    Select_Field = 0;
    Initialize_Setup_Fan_Screen();                                                                  // Initialize setup heater screen
    Update_Setup_Fan_Screen();                                                                      // Update the screen with latest info

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
            // Do nothing
        }

        if (Key_Up_Flag == 1)                                                                       // If user increments
        {
            if (setup_fan_temp < 110) setup_fan_temp++;
            Update_Setup_Fan_Screen();                                                              // Update screen with latest info
            Key_Up_Flag = 0;                                                                        // Clear the increment flag
        }

        if (Key_Dn_Flag == 1 )                                                                      // If user decrements
        {
            if (setup_fan_temp > 50) setup_fan_temp--;
            Update_Setup_Fan_Screen();                                                              // Update screen with latest info
            Key_Dn_Flag = 0;                                                                        // Clear the decrement flag
        }
    }
    DS3231_Read_Alarm_Time();                                                                       // Read alarm time
    DS3231_Read_Time();                                                                             // Read current time
    Initialize_Screen();                                                                            // Initialize main screen before returning

}

void Initialize_Setup_Fan_Screen(void)
{
    fillScreen(ST7735_BLACK);                                                                       // Fills background of screen with color passed to it

    strcpy(txt, "ECE3301 Fall20 Final\0");                                                         // Text displayed
    drawtext(start_x , start_y, txt, ST7735_WHITE  , ST7735_BLACK, TS_1);                           // X and Y coordinates of where the text is to be displayed

    strcpy(txt, "Temp Setup\0");
    drawtext(start_x , start_y+25, txt, ST7735_YELLOW, ST7735_BLACK, TS_2);

    strcpy(txt, "Set Target Temp");
    drawtext(setup_fan_x , setup_fan_y, txt, ST7735_CYAN  , ST7735_BLACK, TS_1);
}

void Update_Setup_Fan_Screen(void)
{

    setup_fan_text[0] = (setup_fan_temp/100)%10 + '0';
    setup_fan_text[1] = (setup_fan_temp/10 )%10 + '0';
    setup_fan_text[2] = (setup_fan_temp    )%10 + '0';
    drawtext(setup_data_fan_x, setup_data_fan_y ,setup_fan_text, ST7735_RED, ST7735_BLACK, TS_2);
}

