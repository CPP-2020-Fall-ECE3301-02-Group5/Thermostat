#ifndef I2C_SUPPORT_H
#define I2C_SUPPORT_H

void DS1621_Init(void);
void DS3231_Read_Time(void);
void DS3231_Write_Time(void);
void DS3231_Read_Alarm_Time(void);
void DS3231_Write_Alarm_Time(void);
int DS3231_Read_Temp(void);
int DS1621_Read_Temp(void);
void DS3231_Turn_On_Alarm();
void DS3231_Turn_Off_Alarm();
void DS3231_Init();
void DS3231_Write_Initial_Alarm_Time(void);
int dec_2_bcd (char);
int bcd_2_dec (char);

#endif /* I2C_SUPPORT_H */