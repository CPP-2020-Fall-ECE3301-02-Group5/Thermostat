#include <p18f4620.h>
#include "I2C_Soft.h"
#include "Main.h"

#define SCL_PIN PORTDbits.RD3
#define SCL_DIR TRISDbits.RD3
#define SDA_PIN PORTDbits.RD4
#define SDA_DIR TRISDbits.RD4

void I2C_Init(unsigned long c)
{
   i2cHighScl();
   i2cHighSda();
}

unsigned char I2C_Read(char ack)
{
   unsigned char inByte, n;
   i2cHighSda();
   for (n=0; n<8; n++)
   {
      i2cHighScl();

      if (SDA_PIN)
         inByte = (inByte << 1) | 0x01; 	// ms bit first
      else
         inByte = inByte << 1;
      i2cLowScl();
   }
   if (ack) i2cAck();
   else i2cNack();


   return(inByte);
}

void I2C_Write(unsigned char outByte )
{
   unsigned char n;
   for(n=0; n<8; n++)
   {
      if(outByte&0x80) i2cHighSda();
      else i2cLowSda();
      i2cHighScl();
      i2cLowScl();
      outByte = outByte << 1;
   }
   i2cHighSda();
   i2cHighScl();
   i2cLowScl();	
}

void i2cNack(void)
{
   i2cHighScl();
   i2cLowScl();						// bring data high and clock
}

void i2cAck(void)
{
   i2cLowSda();	
   i2cHighScl();
   i2cLowScl();
   i2cHighSda();					// bring data low and clock
}

void I2C_Start(void)
{
   i2cHighSda();
   i2cHighScl();	
   i2cLowSda();
   i2cLowScl();						// bring SDA low while SCL is high
}

void I2C_ReStart(void)
{
   i2cLowScl();
   i2cHighSda();
   i2cHighScl();	
   i2cLowSda();
   i2cLowScl();						// bring SDA low while SCL is high
}

void I2C_Stop(void)
{
   i2cLowScl();
   i2cLowSda();
   i2cHighScl();
   i2cHighSda();					// bring SDA high while SCL is high
}

void i2cHighSda(void)
{
   SDA_DIR = 1;						// bring SDA to high impedance
   __delay_us(2);
}

void i2cLowSda(void)
{
   SDA_PIN = 0;						// output a logic zero
   SDA_DIR = 0;						// set to output mode
   __delay_us(2);
}

void i2cHighScl(void)
{
   SCL_DIR = 1;						// bring SCL to high impedance
   __delay_us(10);
}

void i2cLowScl(void)
{
   SCL_PIN = 0;						// output a logic zero
   SCL_DIR = 0;						// set to output mode
   __delay_us(1);
}

char I2C_Write_Address_Read_One_Byte(char Device, char Address)
{
char Data_Ret;    
  I2C_Start();                      // Start I2C protocol
  I2C_Write((Device << 1) | 0);     // DS3231 address Write mode
  I2C_Write(Address);               // Send register address
  I2C_ReStart();                    // Restart I2C
  I2C_Write((Device << 1) | 1);     // Initialize data read
  Data_Ret = I2C_Read(NAK);         //
  I2C_Stop(); 
  return Data_Ret;
}

void I2C_Write_Address_Write_One_Byte(char Device, char Address, char Data_Out)
{
  I2C_Start();                      // Start I2C protocol
  I2C_Write((Device << 1) | 0);     // Device address Write mode
  I2C_Write(Address);               // Send register address
  I2C_Write(Data_Out);              // Initialize data read
  I2C_Stop(); 
}

void I2C_Write_Cmd_Only(char Device, char Cmd)
{
  I2C_Start();                      // Start I2C protocol
  I2C_Write((Device << 1) | 0);     // Device address Write mode
  I2C_Write(Cmd);                   // Send Command
  I2C_Stop(); 
}

void I2C_Write_Cmd_Write_Data(char Device, char Cmd, char Data_Out)
{
  I2C_Start();                      // Start I2C protocol
  I2C_Write((Device << 1) | 0);     // Device address Write mode
  I2C_Write(Cmd);                   // Send Command
  I2C_Write(Data_Out);              // Send Data
  I2C_Stop(); 
}

char I2C_Write_Cmd_Read_One_Byte(char Device, char Cmd)
{
  char Data_Ret;    
  I2C_Start();                      // Start I2C protocol
  I2C_Write((Device << 1) | 0);     // Device address
  I2C_Write(Cmd);                   // Send register address
  I2C_ReStart();                    // Restart I2C
  I2C_Write((Device << 1) | 1);     // Initialize data read
  Data_Ret = I2C_Read(NAK);         //
  I2C_Stop(); 
  return Data_Ret;
}
