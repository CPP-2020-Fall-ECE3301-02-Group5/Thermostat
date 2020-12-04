#ifndef INTERRUPT_H
#define INTERRUPT_H

void interrupt  high_priority chkisr(void) ;
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);
void T0_ISR();
void Init_Interrupt(void);

#endif /* INTERRUPT_H */