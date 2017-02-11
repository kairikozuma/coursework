// A simple IO demo using the MCP23S17 library
//
// MCP23S17 Library Copyright (c) 2010 Romilly Cocking
// Released under the MIT License: http://mbed.org/license/mit
//
// See http://mbed.org/users/romilly/notebook/mcp23s17-addressable-16-bit-io-expander-with-spi/
//
//
// MCP23S17 datasheet http://ww1.microchip.com/downloads/en/DeviceDoc/21952b.pdf
// uses MCP23S17 library version 0.4
 
#include "mbed.h"
#include "MCP23S17.h"
// Create SPI bus
SPI spi(p5, p6, p7);
// 
// Wiring Connections:
// mbed p5,p6,p7 are tied to MCP23S17 SI, SO, SCK pins
// mbed p20 to MCP23S17 CS
// MCP23S17 reset pin pulled high
// MCP23S17 GPA0 connected to GPB0 for loopback test
// A0, A1, A2 of the MCP23S17  are tied to ground on the breadboard, so the 8-bit address for writes is 0x40
// This is referred to as the opcode in the device datasheet
char Opcode = 0x40;
 
// Next create a MCP23S17
// mbed p20 is connected to ~chipSelect on the MCP23S17
MCP23S17 chip = MCP23S17(spi, p20, Opcode);
 
// Optional software reset - mbed p14 to MCP23S17 reset pin
// DigitalOut reset(p14);
 
int main() {
//  The MCP23S17 reset pin can just be pulled high, since it has a power on reset circuit.
//  The reset pin can be used for a software forced reset by pulling it low with an mbed GPIO pin.
//  But just leave it pulled high for this simple demo code.
//  After a power on reset, both IO ports default to input mode
//
//  Here is the optional code for a software reset
//  reset = 0;
//  wait_us(1);
//  reset = 1;
//
//  Set all 8 Port A bits to output direction
    chip.direction(PORT_A, 0x00);
//  Set all 8 Port B bits to input direction
    chip.direction(PORT_B, 0xFF);


    while (1) {
        int sw = chip.read(PORT_B) & 0x01;
        if (sw == 1) {
            chip.write(PORT_A, 0x01);
            wait(.1);
        } else {
            chip.write(PORT_A, 0x00);
            wait(.1);
        }
        
//        chip.write(PORT_A, 0x00);
//        wait(.5);
//        chip.write(PORT_A, 0x01);
//        wait(.5);
    }
}
 