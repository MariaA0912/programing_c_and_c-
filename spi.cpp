// Author: Emma Halferty
// Net ID: emmahalferty
// Date: 3/18/2024
// Assignment: Lab 4
// lab partners: Maria Arvizu Velazquez, Alireza Shariati, Jerry Myers

#include <avr/io.h>
#include <Arduino.h>
#include <avr/interrupt.h>
#include "spi.h"

#define DDR_SPI DDRB // Data Direction Register on ATMEGA2560 for SPI is DDRB
#define DD_SS DDB0  // SS Chip Select data direction bit B0 of ATMEGA2560 is DDB0
#define DD_SCK DDB1  // Clock pin connection data direction bit B1 on ATMEGA2560 is DDB1
#define DD_MOSI DDB2 // MOSI pin datadirection on ATMEGA2560 is DDB0
#define SPI_PORT PORTB // PortB for SPI on ATMEGA2560 is PORTB
#define SPI_SS_BIT PORTB0 // Port B register Bit B0 of Chip Select on ATMEGA2560 is PORTB0

#define wait_for_complete while(!(SPSR & (1 << SPIF)));

/*1.All communication with the 8 x8 led matrix must be done over the SPI 
related pins.
2.Read the 8x8 MAX7219 datasheet for setting up SPI mode*/

// from lecture slides
//function configures the microcontroller to operate as an SPI master
// sets the MOSI (Master Out Slave In), SCK (Serial Clock), and SS (Slave Select) pins as outputs
// SPI communication parameters are set to work with the MAX7219 (8x8 led matrix display)
// uses slower clock rate to ensure reliable commuinication with LED driver
// samples on rising edge of clock (CPOL and CPHA = 1)
void SPI_MASTER_Init() {
    // set MOSI,SCK,and SS direction to outputs
    DDR_SPI = (1 << DD_MOSI)| (1 << DD_SCK) | (1 << DD_SS);

    // note this program does not use MISO line. IT only writes to device

    // set SS high initially (chip select off)
    SPI_PORT  |= (1 << SPI_SS_BIT);

    // enable SPI, master mode, CPOL, CPHA, default clock and fosc/128
    // datasheet says sample on rising edge CPOL = 1 CPHA =1
    SPCR  |= (1 <<SPE)| (1 << MSTR) | (1 << CPOL) | (1 << CPHA ) | (1 << SPR1) | (1 << SPR0);

    }

// FROM LECTURE SLIDES
// function is used to send commands and data to the MAX7219
void write_execute(unsigned char CMD, unsigned char data) {
    SPI_PORT &= ~(1 << SPI_SS_BIT);  // enable chip select bit to begin SPI frame
    SPDR = CMD; // send command
    wait_for_complete; // wait for flag to raise
    SPDR = data; // send byte of data
    wait_for_complete; // wait for flag to raise
    SPI_PORT |= (1 << SPI_SS_BIT); // raise SS pinto end commuinication
    }

    // initializing the 8 x 8 LED array (info from MAX7219 datasheet)
void initMatrix() {
    SPI_MASTER_Init();
    write_execute(0x0A, 0x08);  // brightness control
    write_execute(0x0B, 0x07); // scanning all rows and columns
    write_execute(0x0C,0x01); // set shutdown register to normal operation (0x01)
    write_execute(0x0F, 0x00); // display test register - set to normal operation (0x01)
}

void write_image(unsigned char * image) {
    // Write each byte line by line into 8x8 matrix
    for(int i = 0; i < 8; i++) {
        write_execute(i + 1, image[i]);
    }
}

// marticies for faces found online...
// Encoding for a smiley face on 8x8 led matrix
unsigned char smile[] = {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
    
// Encoding for a sad face on 8x8 led matrix
unsigned char sad[] = {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C};

void ledSadFace() {
    // Writes the sad face onto the 8x8 matrix
    write_image(sad);
}

void ledSimleyFace() {
    // Wirtes the happy face onto the 8x8 matrix
    write_image(smile);
}