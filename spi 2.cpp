#include <Arduino.h>
#include "spi.h"
#include <avr/io.h>

#define wait_for_completion while(!(SPSR & (1<<SPIF)));

#define DDR_SPI DDRB // Data Direction Register on ATMEGA2560 for SPI is DDRB
#define DD_SS DDB0  // SS Chip Select data direction bit B0 of ATMEGA2560 is DDB0
#define DD_SCK DDB1  // Clock pin connection data direction bit B1 on ATMEGA2560 is DDB1
#define DD_MOSI DDB2 // MOSI pin datadirection on ATMEGA2560 is DDB0
#define SPI_PORT PORTB // PortB for SPI on ATMEGA2560 is PORTB
#define SPI_SS_BIT PORTB0 // Port B register Bit B0 of Chip Select on ATMEGA2560 is PORTB0

void SPI_MASTER_Init() {
    // set MOSI,SCK,and SS direction to outputs
    DDR_SPI = (1 << DD_MOSI)| (1 << DD_SCK) | (1 << DD_SS);

    // set SS high initially (chip select off)
    SPI_PORT  |= (1 << SPI_SS_BIT);

    // enable SPI, master mode, CPOL, CPHA, default clock and fosc/128
    // datasheet says sample on rising edge CPOL = 1 CPHA =1
    SPCR  |= (1 <<SPE)| (1 << MSTR) | (1 << CPOL) | (1 << CPHA ) | (1 << SPR1) | (1 << SPR0);

    }
void spi_write(unsigned char regAddress, unsigned char value){
    // set SS low to begin SPI frame
    PORTB &= ~(1 << PORTB0);
    SPDR = regAddress;
    wait_for_completion;
    SPDR = value;
    wait_for_completion;
    // set SS high to end SPI frame
    PORTB |= (1 << PORTB0);
  
}

//initialize 8x8 display using Datasheet
void initMAX7219() {
    spi_write(0x09, 0x00); // Decode mode: No decoding
    spi_write(0x0A, 0x03); // Brightness intensity (0x00 to 0x0F)
    spi_write(0x0B, 0x07); // Scan limit: All digits (0x00 to 0x07)
    spi_write(0x0C, 0x01); // Shutdown mode: Normal operation (0x00), Shutdown (0x01)
    spi_write(0x0F, 0x00); // Display test: Normal operation (0x00), Display test mode (0x01)
}
//finishing up 8x8 setup
void setup() {
    SPI_MASTER_Init();
    initMAX7219();
}
//similey
unsigned char smiley[8] = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100
};
//frown
unsigned char frown[8] = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10011001,
    0b10100101,
    0b01000010,
    0b00111100
};
//for loop to display face on 8x8 display
void displayFace(unsigned char *face) {
    for (int i = 1; i <= 8; i++) {
        spi_write(i, face[i - 1]);
    }
}
//functions for happy and sad face to not pass a parameter from main
void ledsmiley(){
    displayFace(smiley);
}
void ledfrown(){
    displayFace(frown);
}