// Part 1
#include "mbed.h" 
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(p28, p27, p29); // create a global lcd object
int main() {
    uLCD.printf("\nWhat’s up mBED!\n"); 
}