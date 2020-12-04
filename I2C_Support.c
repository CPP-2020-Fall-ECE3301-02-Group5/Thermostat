#include <stdio.h>
#include "Main.h"
#include "I2C_Support.h"
#include <p18f4620.h>
#include "I2C_Soft.h"

extern unsigned char second, minute, hour, dow, day, month, year;
extern unsigned char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
extern unsigned char alarm_second, alarm_minute, alarm_hour, alarm_date;
extern unsigned char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;


void DS1621_Init()
{
    char Device = 0x48;                                                                             // DS1621 thermometer I2C address
    I2C_Write_Cmd_Write_Data(Device, ACCESS_CFG, CONT_CONV);
    I2C_Write_Cmd_Only(Device, START_CONV);
}

int DS1621_Read_Temp()
{
    char Device = 0x48;
    char Cmd = READ_TEMP;
    char Data_Ret;    
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address
    I2C_Write(Cmd);                                                                                 // Send register address
    I2C_ReStart();                                                                                  // Restart I2C
    I2C_Write((Device << 1) | 1);                                                                   // Initialize data read
    Data_Ret = I2C_Read(NAK);                                                                       //
    I2C_Stop(); 
    return Data_Ret;
}

void DS3231_Read_Time()
{
    char Device = 0x68;                                                                             // I2C Address of DS3231
    char Address = 0x00;                                                                            // Register address to read from
    char Data_Ret;
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address
    I2C_Write(Address);                                                                             // Send register address
    I2C_ReStart();                                                                                  // Restart I2C
    I2C_Write((Device << 1) | 1);                                                                   // Initialize data read
    second = I2C_Read(ACK);                                                                         // Read second
    minute = I2C_Read(ACK);                                                                         // Read minute
    hour = I2C_Read(ACK);                                                                           // Read hour
    dow = I2C_Read(ACK);                                                                            // Read day of week
    day = I2C_Read(ACK);                                                                            // Read day
    month = I2C_Read(ACK);                                                                          // Read month
    year = I2C_Read(NAK);                                                                           // Read year
    I2C_Stop();
}

void DS3231_Write_Time()
{
    char Device = 0x68;                                                                             // Device ID
    char Address = 0x00;                                                                            // Beginning Address 0
    second = dec_2_bcd(setup_second);                                                               //
    minute = dec_2_bcd(setup_minute);                                                               //
    hour = dec_2_bcd(setup_hour);                                                                   //
    dow = 0x01;                                                                                     //
    day = dec_2_bcd(setup_day);                                                                     //
    month = dec_2_bcd(setup_month);                                                                 //
    year = dec_2_bcd(setup_year);                                                                   //
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address Write mode
    I2C_Write(Address);                                                                             // Send register address
    I2C_Write(second);                                                                              // Write seconds
    I2C_Write(minute);                                                                              // Write minutes
    I2C_Write(hour);                                                                                // Write hours
    I2C_Write(dow);                                                                                 // Write DOW
    I2C_Write(day);                                                                                 // Write day
    I2C_Write(month);                                                                               // Write month
    I2C_Write(year);                                                                                // Write year
    I2C_Stop();                                                                                     // End I2C protocol
}

void DS3231_Write_Initial_Alarm_Time()
{
    DS3231_Read_Time();                                                                             // Read current time
    alarm_date = day;                                                                               // Set alarm to today
    char Device = 0x68;                                                                             // Device ID given
    char Address = 0x07;                                                                            // Write to register 07
    alarm_hour = dec_2_bcd(0x01);                                                                   // Convert info to BCD
    alarm_minute = dec_2_bcd(0x01);
    alarm_second = dec_2_bcd(0x01);
    alarm_second = alarm_second & 0x7f;                                                             // Mask off bit 7
    alarm_minute = alarm_minute & 0x7f;                                                             // Mask off bit 7
    alarm_hour   = alarm_hour   & 0x7f;                                                             // Mask off bit 7
    alarm_date   = alarm_date   | 0x80;                                                             // Set A1M4 as 1

    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address Write mode
    I2C_Write(Address);                                                                             // Send register address 7

    I2C_Write(alarm_second);                                                                        // Write alarm seconds value to DS3231
    I2C_Write(alarm_minute);                                                                        // Write alarm minute value to DS3231
    I2C_Write(alarm_hour);                                                                          // Write alarm hour value to DS3231
    I2C_Write(alarm_date);                                                                          // Write alarm date value to DS3231
    I2C_Stop();                                                                                     // End I2C protocol
}

void DS3231_Read_Alarm_Time()
{
    char Device = 0x68;                                                                             // Device ID given
    char Address = 0x07;                                                                            // Read from register 07
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address
    I2C_Write(Address);                                                                             // Send register address
    I2C_ReStart();                                                                                  // Restart I2C
    I2C_Write((Device << 1) | 1);                                                                   // Initialize data read
    alarm_second = I2C_Read(ACK);                                                                   // Read seconds from register 7
    alarm_minute = I2C_Read(ACK);                                                                   // Read minutes from register 8
    alarm_hour   = I2C_Read(ACK);                                                                   // Read hour from register 9
    alarm_date   = I2C_Read(NAK);                                                                   // Read hour from register A
    I2C_Stop();                                                                                     // End I2C protocol
}

void DS3231_Init()
{
    char Device = 0x68;
    char Address_7 = 0x07;
    char Address_E = 0x0E;
    char control_E;

    control_E = I2C_Write_Address_Read_One_Byte(Device, Address_E);
    control_E = control_E & 0x01;
    control_E = control_E | 0x25;
    I2C_Write_Address_Write_One_Byte(Device, Address_E, control_E);
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address
    I2C_Write(Address_7);                                                                           // Send register address
    I2C_ReStart();                                                                                  // Restart I2C
    I2C_Write((Device << 1) | 1);                                                                   // Initialize data read
    alarm_second = I2C_Read(ACK);                                                                   // Read seconds from register 7
    alarm_minute = I2C_Read(ACK);                                                                   // Read minutes from register 8
    alarm_hour   = I2C_Read(ACK);                                                                   // Read hour from register 9
    alarm_date   = I2C_Read(NAK);                                                                   // Read hour from register A

    alarm_second = alarm_second & 0x7f;                                                             // Mask off bit 7
    alarm_minute = alarm_minute & 0x7f;                                                             // Mask off bit 7
    alarm_hour   = alarm_hour   & 0x7f;                                                             // Mask off bit 7
    alarm_date   = alarm_date   | 0x80;                                                             // Mask off bit 7

    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address Write mode
    I2C_Write(Address_7);                                                                           // Send register address 7

    I2C_Write(alarm_second);                                                                        // Reset alarm seconds value to DS3231
    I2C_Write(alarm_minute);                                                                        // Write alarm minute value to DS3231
    I2C_Write(alarm_hour);                                                                          // Write alarm hour value to DS3231
    I2C_Write(alarm_date);                                                                          // Write alarm date value to DS3231
    I2C_Stop();
}

void DS3231_Write_Alarm_Time()
{
    DS3231_Read_Time();                                                                             // Read current time
    alarm_date = day;                                                                               // Set alarm to today
    char Device = 0x68;                                                                             // Device ID given
    char Address = 0x07;                                                                            // Write to register 07
    alarm_hour = dec_2_bcd(setup_alarm_hour);                                                       // Convert info to BCD
    alarm_minute = dec_2_bcd(setup_alarm_minute);
    alarm_second = dec_2_bcd(setup_alarm_second);
    alarm_second = alarm_second & 0x7f;                                                             // Mask off bit 7
    alarm_minute = alarm_minute & 0x7f;                                                             // Mask off bit 7
    alarm_hour   = alarm_hour   & 0x7f;                                                             // Mask off bit 7
    alarm_date   = alarm_date   | 0x80;                                                             // Set A1M4 as 1
    I2C_Start();                                                                                    // Start I2C protocol
    I2C_Write((Device << 1) | 0);                                                                   // Device address Write mode
    I2C_Write(Address);                                                                             // Send register address 7

    I2C_Write(alarm_second);                                                                        // Write alarm seconds value to DS3231
    I2C_Write(alarm_minute);                                                                        // Write alarm minute value to DS3231
    I2C_Write(alarm_hour);                                                                          // Write alarm hour value to DS3231
    I2C_Write(alarm_date);                                                                          // Write alarm date value to DS3231
    I2C_Stop();                                                                                     // End I2C protocol
}

void DS3231_Turn_Off_Alarm()
{
    char Device = 0x68;
    char Address_E = 0x0E;
    char Address_F = 0x0F;
    char control_E;
    char control_F;

    control_E = I2C_Write_Address_Read_One_Byte(Device, Address_E);
    control_E = control_E & 0xFE;
    I2C_Write_Address_Write_One_Byte(Device, Address_E, control_E);
    control_F = I2C_Write_Address_Read_One_Byte(Device, Address_F);
    control_F = control_F & 0xFE;
    I2C_Write_Address_Write_One_Byte(Device, Address_F, control_F);
    DS3231_Init();
}

void DS3231_Turn_On_Alarm()
{
    char Device = 0x68;
    char Address_E = 0x0E;
    char Address_F = 0x0F;
    char control_E;
    char control_F;

    control_E = I2C_Write_Address_Read_One_Byte(Device, Address_E);
    control_E = control_E | 0x01;
    I2C_Write_Address_Write_One_Byte(Device, Address_E, control_E);

    control_F = I2C_Write_Address_Read_One_Byte(Device, Address_F);
    control_F = control_F & 0xFE;
    I2C_Write_Address_Write_One_Byte(Device, Address_F, control_F);
    DS3231_Init();

}

int bcd_2_dec (char bcd)
{
    int dec;
    dec = ((bcd>> 4) * 10) + (bcd & 0x0f);
    return dec;
}

int dec_2_bcd (char dec)
{
    int bcd;
    bcd = ((dec / 10) << 4) + (dec % 10);
    return bcd;
}
