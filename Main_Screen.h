#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

void Initialize_Screen(void) ;
void Update_Screen(void);

#define start_x             1
#define start_y             1
#define temp_x              24
#define temp_y              11
#define circ_x              40
#define circ_y              23
#define data_tempc_x        1
#define data_tempc_y        23
#define tempc_x             45
#define tempc_y             23
#define cirf_x              110
#define cirf_y              23
#define data_tempf_x        70
#define data_tempf_y        23
#define tempf_x             115
#define tempf_y             23
#define time_x              50
#define time_y              40
#define data_time_x         15
#define data_time_y         52
#define date_x              50
#define date_y              71
#define data_date_x         15
#define data_date_y         82

#define alarm_time_x        4
#define alarm_time_y        100
#define data_alarm_time_x   10
#define data_alarm_time_y   110

#define alarm_sw_x          77
#define alarm_sw_y          100
#define data_alarm_sw_x     95
#define data_alarm_sw_y     110

#define fan_set_temp_x      6
#define fan_set_temp_y      120
#define data_fan_set_temp_x 35
#define data_fan_set_temp_y 130

#define fan_sw_x            85
#define fan_sw_y            120
#define data_fan_sw_x       95
#define data_fan_sw_y       130

#define dc_x                20
#define dc_y                140
#define data_dc_x           15
#define data_dc_y           150

#define rtc_x               48
#define rtc_y               140
#define data_rtc_x          50
#define data_rtc_y          150

#define volt_x              77
#define volt_y              140
#define data_volt_x         75
#define data_volt_y         150

#define rpm_x               95
#define rpm_y               140
#define data_rpm_x          93
#define data_rpm_y          150

#define TS_1                1               // Size of Normal Text
#define TS_2                2               // Size of Number Text 

#endif /* MAIN_SCREEN_H */