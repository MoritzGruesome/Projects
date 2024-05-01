#include <stdint.h>
#include <pic32mx.h>
#include "utils.h"
#include "gamefunc.h"
#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9


#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200


void delay(int cyc) {
    int i;
    for(i = cyc; i > 0; i--);
}


void SPI_Config( void ) {
    /* Set up peripheral bus clock */
    OSCCON &= ~0x180000;
    OSCCON |= 0x080000;
    
    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;
    
    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;
    
    /* Set up input pins */
    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);
    
    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    
    /* Clear SPIROV*/
    SPI2STATCLR &= ~0x40;
    /* Set CKP = 1, MSTEN = 1; */
        SPI2CON |= 0x60;
    
    /* Turn on SPI */
    SPI2CONSET = 0x8000;
    
}
/*
spi_send_recv:
spi_PowerOnDisplay:
SPI_Config:
disp_Write:
 
Copyright (c) 2015, Axel Isaksson
Copyright (c) 2015, F Lundevall

If you're a student, and you have modified one or more files,
you must add your name here.
Moritz Gruss
Viktor Lådö Næss

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
uint8_t spi_send_recv(uint8_t data)
{
	while (!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while (!(SPI2STAT & 1));
	return SPI2BUF;
}

void spi_PowerOnDisplay( void ) {
    DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
    // clears the appropriate bits of PORTF
    delay(10);
    DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
    //Clears the appropriate bits(6) of PORTF
    delay(1000000);
    
    spi_send_recv(0xAE);
    // Display off command (sleep mode)
    DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
    delay(10);
    DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
    delay(10);
    // Bring reset LOW and then HIGH
    
    spi_send_recv(0x8D);
    spi_send_recv(0x14);
    
    spi_send_recv(0xD9);
    spi_send_recv(0xF1);
    
    DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
    delay(10000000);
    // turn on VCC and wait
    
    spi_send_recv(0xA1);
    // set segment remap, column address 127 is mapped to SEG0
    spi_send_recv(0xC8);
    // Set Com output scan direction : remapped mode. Scan from COM[N-1] to COM0
    //Where N is the Multiplex ratio.
    // previous two commands invert the display setting display origin as top left corner
    spi_send_recv(0xDA);
    // Set COM pins Hardware Configuration
    spi_send_recv(0x20);
    // Set Memory Addressing Mode(Horizontal addressing mode)
    //makes the display memory non-interleaved
    spi_send_recv(0xAF);
    //Set display on in normal mode
}

void updateDisp() {     // Moritz
    int i, j, k;
    uint8_t dispbyte = 0;
    for(i = 0; i < 4; i++) {
        // loops through pages
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
        spi_send_recv(0x22);
        //set page address
        spi_send_recv(i);
        //sets page start address
        spi_send_recv(0);
        //sets column start address
        spi_send_recv(128);
        //sets column end address
        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
        
            for(j = 0; j < 128; j++) {
                for(k = 0; k < 8; k++) {
                    dispbyte |= (d_mat[k + 8*i][j] << k);
                }
                spi_send_recv(dispbyte);
                dispbyte = 0;
        //nested loop converts a 128 x 32 array where each element corresponds to a bit, into byte data
        }
    }
}






