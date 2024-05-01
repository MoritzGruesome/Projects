#include <stdint.h>
#include <pic32mx.h>
#include "utils.h"


// Moritz
void Timer3(int time) { // used for more precise time delays
    T3CON |= 0x8030; // control setup for timer three using 8:1 scaling
    TMR3 = 0;
    PR3 = 10; // time period is 10

    int m = 0;
    while (m < time) {
        if (IFS(0) & 0x1000) {
            m++;
            IFS(0) &= 0xffffefff; // Reset flag
        }
    }
    return;
}
// Viktor
void time() { // used for longer time delays
	T2CON |= 0x8070; // prescaling 256:1
	TMR2 = 0;
	PR2 = 3125; // time period is max

	int m = 0;
	while (m < 5) {
		if (IFS(0) & 0x00000100) {
			m++;
			IFS(0) &= 0xFFFFFEFF;
		}
	}
}

// Viktor
int btn(int select) {
    switch (select) {
        case 1:
            return PORTF & 0x2;
            
        case 2:
            return PORTD & 0x20;
            
        case 3:
            return PORTD & 0x40;
            
        case 4:
            return PORTD & 0x80;
            
        default:
            return 0;
    }
}



