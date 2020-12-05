#include <xc.h>
#include <p18f4620.h>
#include "Interrupt.h"
#include "Main.h"

extern int INT0_flag, INT1_flag, INT2_flag;

void Init_Interrupt(void)
{
    RCONbits.IPEN = 0;                                                                              // disable interrupt priority

    INTCONbits.INT0IF   = 0;                                                                        // clear INT0 status flag
    INTCON3bits.INT1IF  = 0;                                                                        // clear INT1 status flag
    INTCON3bits.INT2IF  = 0;                                                                        // clear INT2 status flag

    INTCON2bits.INTEDG0 = 1;                                                                        // config INT0 interrupt at rising edge
    INTCON2bits.INTEDG1 = 1;                                                                        // config INT1 interrupt at rising edge
    INTCON2bits.INTEDG2 = 1;                                                                        // config INT2 interrupt at rising edge

    INTCONbits.INT0IE   = 1;                                                                        // Enable INT0
    INTCON3bits.INT1IE  = 1;                                                                        // Enable INT1
    INTCON3bits.INT2IE  = 1;                                                                        // Enable INT2

    INTCONbits.PEIE     = 1;                                                                        // Enable peripheral interrupt
    INTCONbits.GIE      = 1;                                                                        // Enable interrupt globally
}

void interrupt  high_priority chkisr()
{
    if (INTCONbits.INT0IF == 1) INT0_ISR();                                                         // check the INT0 interrupt
    if (INTCON3bits.INT1IF == 1) INT1_ISR();                                                        // check the INT1 interrupt
    if (INTCON3bits.INT2IF == 1) INT2_ISR();                                                        // check the INT2 interrupt
}

void INT0_ISR()
{
    INTCONbits.INT0IF=0;                                                                            // Clear the interrupt flag
    __delay_ms(5);
    INT0_flag = 1;                                                                                  // set software INT0_flag
}

void INT1_ISR()
{
    INTCON3bits.INT1IF=0;                                                                           // Clear the interrupt flag
    __delay_ms(5);
    INT1_flag = 1;                                                                                  // set software INT1_flag
}

void INT2_ISR()
{
    INTCON3bits.INT2IF=0;                                                                           // Clear the interrupt flag
    __delay_ms(5);
    INT2_flag = 1;                                                                                  // set software INT2_flag
}

