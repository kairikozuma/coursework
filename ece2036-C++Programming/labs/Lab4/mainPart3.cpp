// Lab 4 Part 3
// Kairi Kozuma
// ECE 2036B
#include "mbed.h"
#include "uLCD_4DGL.h"
#include "SDFileSystem.h"
#include "PinDetect.h"
#include <fstream>

// Push buttons
PinDetect pb1(p14);
PinDetect pb2(p15);
PinDetect pb3(p16);

// Display
uLCD_4DGL uLCD(p28, p27, p29); // serial tx, serial rx, reset pin;

// SD File system
SDFileSystem sd(p5, p6, p7, p8, "sd"); 

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

// Read OTP from sd card
std::string readOTP() {
  char myOTP[1000];
  FILE *fp = fopen("/sd/OTP.txt", "r"); 
  if(fp == NULL) {
    uLCD.printf("Open Error!!!\n");
    return NULL;
  } else {
    fscanf(fp, "%s", &myOTP);
    uLCD.printf("Successfully read OTP\n");
  }
  fclose(fp);
  std::string s(myOTP);
  return s;
}

// Get current receiver index and increment in file
int getCurrentReceiverIndex() {
  // Read current index from file
  int current;
  FILE *fpr = fopen("/sd/positionCipherReceiver.txt", "r"); 
  if(fpr == NULL) {
    uLCD.printf("Open Error!!!\n");
    return NULL;
  } else {
    fscanf(fpr, "%d", &current);
  }
  fclose(fpr);
  
  // Increment current receiver index and write to file
  FILE *fpw = fopen("/sd/positionCipherReceiver.txt", "w"); 
  if(fpw == NULL) {
    uLCD.printf("Open Error!!!\n");
    return NULL;
  } else {
    fprintf(fpw, "%i", current + 1);
  }
  fclose(fpw);
  return current; 
}

// Get the enrypted letter from the OTP and selected letter
char getDecrypted(char letterEncrypted, char letterOTP) {
  int num = (((int) letterEncrypted) - 65) - (((int) letterOTP) - 65);
  // Correct the number if num is negative
  num = num < 0 ? num + 26 : num;
  char decrypted = (char)(num + 65);
  return decrypted;
}

// Helper function to display current letter
// Letters to the left and right are also shown
void displayCurrentLetter(char letter) {
  char leftLet = letter != 'A' ? (char) (letter - 1) : 'Z';
  char centerLet = letter;
  char rightLet = letter != 'Z' ? (char) (letter + 1) : 'A';
  
  // Make text large
  uLCD.text_width(2);
  uLCD.text_height(2);  
  
  // Show letters
  uLCD.locate(2,3);
  uLCD.printf("%c", leftLet);
  uLCD.locate(4,3);
  uLCD.printf("%c", centerLet);
  uLCD.locate(6,3);
  uLCD.printf("%c", rightLet);
  
  // Delete the decrypted letter at the bottom
  uLCD.filled_rectangle(56,79,68,120,BLACK);
}

// Helper function that displays decrypted letter
void displayDecryptedLetter(char letter) {
    // Draw triangle pointing down from current letter    
    uLCD.triangle(56,79,68,79,62,87,GREEN);
  
    // Make text large
    uLCD.text_width(2);
    uLCD.text_height(2);  
  
    uLCD.locate(4, 6);
    uLCD.printf("%c", letter);
}

int main() {
  // Setup pushbuttons
  setupPushButtons();
  
  // Read OTP from file and store in string
  std::string otp = readOTP();
  
  // Declare variables to be used when decrypting
  bool finished = false;
  int myLetIndex = 0;
  char myLet = 'A';
  int otpIndex = getCurrentReceiverIndex();
  char otpLet;
  char myDecryptedLet;
  
  // Set initial screen
  uLCD.cls();
  uLCD.printf("Letter to decrypt:");
  uLCD.triangle(56,34,68,34,62,42,WHITE);
  displayCurrentLetter(myLet);
  
  // Loop through until code is done
  while(!finished) {
    // Left button, decrement letter
    if(!pb1) {
         myLetIndex = myLetIndex > 0 ? myLetIndex - 1 : 25;
         myLet = (char)(myLetIndex + 65);
         displayCurrentLetter(myLet);
         // Loop until button is released to prevent duplicate press
         while(!pb1) {
         }
         wait(0.1);
    // Middle button, select letter and decrypt
    } else if (!pb2) {
         otpLet = otp[otpIndex];
         myDecryptedLet = getDecrypted(myLet, otpLet);
         displayDecryptedLetter(myDecryptedLet); 
         otpIndex = getCurrentReceiverIndex();
         // Loop until button is released to prevent duplicate press
         while(!pb2) {
         }
         wait(0.1);
    // Right button, increment letter
    } else if (!pb3) {
         myLetIndex = myLetIndex < 25 ? myLetIndex + 1 : 0;
         myLet = (char)(myLetIndex + 65);
         displayCurrentLetter(myLet);
         // Loop until button is released to prevent duplicate press
         while(!pb3) {
         }
         wait(0.1);
    }
  }
}
