#include <xc.h>
#include <p18f4620.h>
#include "Interrupt.h"
#include "Main.h"

extern int INT0_flag, INT1_flag, INT2_flag, Tach_cnt;

void Init_Interrupt(void)
{
    RCONbits.IPEN = 0;                                                                              // disable interrupt priority

    INTCONbits.TMR0IF   = 0;                                                                        // clear TMR0 status flag
    INTCONbits.INT0IF   = 0;                                                                        // clear INT0 status flag
    INTCON3bits.INT1IF  = 0;                                                                        // clear INT1 status flag
    INTCON3bits.INT2IF  = 0;                                                                        // clear INT2 status flag

    INTCON2bits.INTEDG0 = 0;                                                                        // config INT0 interrupt at falling edge
    INTCON2bits.INTEDG1 = 0;                                                                        // config INT1 interrupt at falling edge
    INTCON2bits.INTEDG2 = 1;                                                                        // config INT2 interrupt at  rising edge

    INTCONbits.TMR0IE   = 0;                                                                        // Enable TMR0 interrupt
    INTCONbits.INT0IE   = 1;                                                                        // Enable INT0
    INTCON3bits.INT1IE  = 1;                                                                        // Enable INT1
    INTCON3bits.INT2IE  = 1;                                                                        // Enable INT2

    INTCONbits.PEIE     = 1;                                                                        // Enable peripheral interrupt
    INTCONbits.GIE      = 1;                                                                        // Enable interrupt globally
}

void interrupt  high_priority chkisr()
{
    if (INTCONbits.TMR0IF == 1) T0_ISR();                                                           // Timer Interrupt Routine
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

void T0_ISR()
{
    INTCONbits.TMR0IF=0;                                                                            // Clear the interrupt flag
    T0CONbits.TMR0ON=0;                                                                             // Turn off Timer0
    TMR0H = 0x00;                                                                                   // copy the code from Wait_Half_Sec()
    TMR0H = 0x00;
    SEC_LED = !SEC_LED;                                                                             // Invert LED
    Tach_cnt = TMR1L;                                                                               // Store TMR1L into Tach Count
    TMR1L = 0;                                                                                      // Clear TMR1L
    T0CONbits.TMR0ON=1;                                                                             // Turn on Timer0
}

