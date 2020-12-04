// Testing NEC Transmitter
#include <xc.h>
#include "NEC_Transmitter.h"

void main(void) {
    OSCCON = 0x70;
    while (1)
    {
        NEC_send(0x7A, 0x5C);
    }
    return;
}
