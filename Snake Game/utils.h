#include <stdint.h>
#include <stdlib.h>


void spi_PowerOnDisplay(void);

uint8_t spi_send_recv(uint8_t data);

void updateDisp();

uint8_t d_mat[32][128];

int btn(int select);

void Timer3(int time);
void time();




