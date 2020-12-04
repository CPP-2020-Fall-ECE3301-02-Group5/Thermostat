#include <xc.h>
#include <p18f4620.h>

char pre_CCPR1L;
char pre_CCP1CON;
char pre_TRISB;
char pre_TRISC;
char pre_PR2;
char pre_T2CON;
char pre_PORTB;
char pre_PORTC;
char pre_TMR3H;
char pre_TMR3L;
char pre_T3CON;
char pre_TMR2IF;
char pre_TMR3IF;

void save_config()
{    
    pre_TRISB   = TRISB;
    pre_TRISC   = TRISC;
    pre_PR2     = PR2;
    pre_T2CON   = T2CON;
    pre_PORTB   = PORTB;
    pre_PORTC   = PORTC;
    pre_CCPR1L  = CCPR1L;
    pre_CCP1CON = CCP1CON;
    pre_TMR3H   = TMR3H;
    pre_TMR3L   = TMR3L;
    pre_T3CON   = T3CON;
    pre_TMR2IF  = PIR1bits.TMR2IF;
    pre_TMR3IF  = PIR2bits.TMR3IF;
}

void load_config()
{
    TRISB   = pre_TRISB;
    TRISC   = pre_TRISC;
    PR2     = pre_PR2;
    T2CON   = pre_T2CON;
    PORTB   = pre_PORTB;
    PORTC   = pre_PORTC;
    CCPR1L  = pre_CCPR1L;
    CCP1CON = pre_CCP1CON;
    TMR3H   = pre_TMR3H;
    TMR3L   = pre_TMR3L;
    T3CON   = pre_T3CON;
    PIR1bits.TMR2IF = pre_TMR2IF;
    PIR2bits.TMR3IF = pre_TMR3IF;
}

void delay_tmr3_562_5us()
{
    // initialize Timer 3 for 562.5us pulses
    // prescaler = 1:1
    // count = delay / (4/F_osc * prescaler) = 537 = 0x0219
    // TMR3H:TMR3L = 0xFFFF - 0x0219 + 1 = 0xFDE7
    TMR3H = 0xFD;
    TMR3L = 0xE7;
    PIR2bits.TMR3IF = 0;  // clear TMR3 flag
    T3CONbits.TMR3ON = 1; // Turn  on TMR3
    while (!PIR2bits.TMR3IF);
    T3CONbits.TMR3ON = 0; // Turn off TMR3
}

void output_byte(char c)
{
    for (int i = 0; i < 8; i++)
    {
        T2CONbits.TMR2ON = 1;
        delay_tmr3_562_5us();
        T2CONbits.TMR2ON = 0;
        delay_tmr3_562_5us();
        if ((c>>i)&0x01) 
        { 
            // 2 extra low period for high bit
            delay_tmr3_562_5us();   
            delay_tmr3_562_5us();   
        }
    }
}

void NEC_send(char address, char command)
{
    save_config();
    
    // initialize CCP1 as output
    TRISCbits.RC2 = 0;
    
    // initialize CCP1 and Timer 2
    // carrier wave F_PWM = 38kHz => T_PWM = 26.31578947 us
    // 1:1 prescale
    // PR2 = ((F_osc)/(4*F_PWM*prescale))-1 ~= 26
    // CCPR1L:DC1B1[1:0] = 50% duty cycle = 13.0
    PR2 = 25;
    T2CON = 0x00;   // keep T2 off for now
    CCPR1L = 12.5;
    CCP1CON = 0x2C;
    
    // initialize T3CON
    T3CON = 0x80;   // 16-bit, 1:1 prescale, off for now
    
    //  on for   9ms = 16 * 562.5us
    T2CONbits.TMR2ON = 1;
    for (int i = 0; i < 16; i++)
        delay_tmr3_562_5us();
    
    // off for 4.5ms =  8 * 562.5us
    T2CONbits.TMR2ON = 0;
    for (int i = 0; i < 8; i++)
        delay_tmr3_562_5us();
    
    output_byte(address);
    output_byte(~address);
    output_byte(command);
    output_byte(~command);
    
    // single high for stop
    T2CONbits.TMR2ON = 1;
    delay_tmr3_562_5us();
    T2CONbits.TMR2ON = 0;

    // top up to 108ms for one message
    for (int i = 0; i < 71; i++)
        delay_tmr3_562_5us();
    
    load_config();
}