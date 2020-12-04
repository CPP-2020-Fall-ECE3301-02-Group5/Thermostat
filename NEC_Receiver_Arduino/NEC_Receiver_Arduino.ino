#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include "IRremote.h"

#define MAG0 6
#define MAG1 7
#define MAG2 8
#define MAG3 9
#define MAG4 10
#define MAG5 11
#define MAG6 12
#define HEATER 2
#define COOLER 4

int RECV_PIN = 3;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN,OUTPUT);
    pinMode(MAG0,OUTPUT);
    pinMode(MAG1,OUTPUT);
    pinMode(MAG2,OUTPUT);
    pinMode(MAG3,OUTPUT);
    pinMode(MAG4,OUTPUT);
    pinMode(MAG5,OUTPUT);
    pinMode(MAG6,OUTPUT);
    pinMode(HEATER,OUTPUT);
    pinMode(COOLER,OUTPUT);
    irrecv.enableIRIn();
}

void loop() {
    if (irrecv.decode(&results)) {
            uint32_t raw_val = results.value;
            uint8_t lsb_val = raw_val & 0xFF;
            int8_t val = lsb_val;
            bool sign = (val<0);
            uint8_t mag = abs(val); 
            Serial.print(raw_val, HEX);
            Serial.print(' ');
            Serial.print(lsb_val,HEX);
            Serial.print(' ');
            Serial.print(val, DEC);
            Serial.print(' ');
            Serial.print(mag, DEC);
            Serial.println(' ');
            Serial.print(mag & 0x01);
            Serial.println(' ');
            Serial.print(mag & 0x02);
            Serial.println(' ');
            Serial.print(mag & 0x04);
            Serial.println(' ');
            Serial.print(mag & 0x08);
            Serial.println(' ');
            Serial.print(mag & 0x10);
            Serial.println(' ');
            Serial.print(mag & 0x20);
            Serial.println(' ');
            Serial.print(mag & 0x40);
            Serial.println(' ');

            for (int i = 6; i >= 0; i--)
            {
                if ((mag&(0x01<<i))>0)
                {
                  digitalWrite(MAG6+i,HIGH);
                }
                else digitalWrite(MAG6+i,LOW);
            }
            digitalWrite(COOLER, ( sign? HIGH : LOW ) );
            digitalWrite(HEATER, ( ( sign || val == 0 )? LOW : HIGH) );
            irrecv.resume();
    }
}
