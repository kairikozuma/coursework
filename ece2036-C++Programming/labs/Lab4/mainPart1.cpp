// Lab 4 Part 1
// Kairi Kozuma
// ECE 2036B
#include "mbed.h"
#include "uLCD_4DGL.h"
#include "SDFileSystem.h"
#include "TMP36.h"
#include "PinDetect.h"
#include <iostream>
#include <fstream>

// Push buttons
PinDetect pb1(p14);
PinDetect pb2(p15);
PinDetect pb3(p16);

// Display
uLCD_4DGL uLCD(p28, p27, p29); // serial tx, serial rx, reset pin;

// SD File system
SDFileSystem sd(p5, p6, p7, p8, "sd"); 

// Temperature Sensor
TMP36 myTempSensor(p20);

// Setup push buttons
void setupPushButtons() {
  //setup push buttons
  pb1.mode(PullUp);
  pb2.mode(PullUp);
  pb3.mode(PullUp);

  // Delay for initial pullup to take effect
  wait(.01);
  
  // Start sampling pb inputs using interrupts
  pb1.setSampleFrequency();
  pb2.setSampleFrequency();
  pb3.setSampleFrequency();
}


// Generate random number using the temperature
int generateRandom() {
  // Obtain thermal random number
  float tempC1 = myTempSensor.read();
  wait(0.02);
  float tempC2 = myTempSensor.read();
  int trand = (1000 * fabs(tempC1 - tempC2));
    
  // Obtain final random number  
  int random = rand();
  int key = (trand + random) % 26;
  return key;
}

// Create Cipher Sender and Receiver
void createCipherSR() {  
  // positionCipherSender.txt
  FILE *fps = fopen("/sd/positionCipherSender.txt", "w");
  if(fps == NULL) {
    uLCD.printf("Error Open\n");
  } else {
    fprintf(fps, "0");
  } 
  
  // positionCipherReceiver.txt
  FILE *fpr = fopen("/sd/positionCipherReceiver.txt", "w");
  if(fpr == NULL) {
    uLCD.printf("Error Open\n");
  } else {
    fprintf(fpr, "0");
  }
  fclose(fps);
  fclose(fpr); 
}

// Create One Time Pad
string generateOTP() {
  std::string otp;
  for (int i = 0; i < 1000; i++) {
      otp += (char)(generateRandom() + 65);
  }
  return otp;
}

// Write generated string to sd card
void writeToSD(std::string otp) {
  // Open file and write otp to file
  FILE *fp = fopen("/sd/OTP.txt", "w");
  if(fp == NULL) {
    uLCD.printf("Error Open\n");
  } else {
    fprintf(fp, "%s", otp);
  }
  fclose(fp); 
}

// method executed when in the DOWNLOAD_OTP_STATE
bool downloadOTPState(std::string otp) {
  // Clear screen
  uLCD.cls();
  
   // Prompt user to write OTP to SD
  uLCD.printf("Do you want to\ndownload the OTP?");
  uLCD.locate(0,13);
  uLCD.printf("  YES         NO  ");
  
  while(1) {
      if(!pb1) {
         uLCD.locate(7,5);
         uLCD.printf("YES");
         writeToSD(otp);
         return true;
      } else if (!pb3) {
         uLCD.locate(7,5);
         uLCD.printf("NO");
         return false;
      }
  }
}

// method called when in the COPY_OTP_STATE
bool copyOTPstate(std::string otp) {
  // Clear screen
  uLCD.cls();
  
  // Prompt user to write OTP to another SD
  uLCD.printf("Do you want to\ncopy the OTP to\nanother SD card?");
  uLCD.locate(0,13);
  uLCD.printf("  YES         NO  ");
  
  while(1) {
      if(!pb1) {
         uLCD.locate(7,5);
         uLCD.printf("YES");
         writeToSD(otp);
         return true;
      } else if (!pb3) {
         uLCD.locate(7,5);
         uLCD.printf("NO");
         return false;
      }
  }
}

int main() {  
  // Randomize seed
  srand(time(NULL));
  
  // Create positionCipherSender.txt and positionCipherReceiver.txt
  createCipherSR();
  
  // Setup pushbuttons
  setupPushButtons();

  // Notify user that OTP is being generated
  uLCD.printf("Generating OTP\n\nPlease wait ...");

  // Generate One Time Pad
  std::string otp = generateOTP();
  
  // Clear screen after generation complete
  uLCD.cls();
  
  int currentState = 0;
  static const int DOWNLOAD_OTP_STATE = 0;
  static const int COPY_OTP_STATE = 1;
  
  // Condition to end program
  bool endProgram = false;
  
  while(!endProgram) {
      switch(currentState) {
        case DOWNLOAD_OTP_STATE:
          downloadOTPState(otp);
          currentState = COPY_OTP_STATE;
          break;
        case COPY_OTP_STATE:
          copyOTPstate(otp);
          endProgram = true;
          break;
        default:
          break;
      }
      wait(1);
  }
  uLCD.cls();
  uLCD.printf("You have ended the program"); 
}
