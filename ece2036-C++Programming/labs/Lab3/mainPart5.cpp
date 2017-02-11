// Part 5
#include "mbed.h"
#include "SDFileSystem.h"
#include "uLCD_4DGL.h"
#include <iostream>
#include <fstream>

SDFileSystem sd(p5, p6, p7, p8, "sd");
uLCD_4DGL uLCD(p28, p27, p29); // serial tx, serial rx, reset pin;

int main() {
  uLCD.printf("Hello Micro SD Card!\n");
  mkdir("/sd/mydir", 0777);

  // Open file
  FILE *fp = fopen("/sd/mydir/sdtest.txt", "w");
  if(fp == NULL) {
    uLCD.printf("Error Open\n");
  } else {
    // Write to file
    fprintf(fp, "I love ECE2036");
  }
  fclose(fp);

  // Open file to be written to uLCD
  std::string message;
  ifstream fileReader("/sd/mydir/sdtest.txt", ios::in);
  // Store the line in message from fileReader
  std::getline(fileReader, message);
  uLCD.printf(message.c_str());
}
