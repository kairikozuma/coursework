// Define the "gtDisplay" graphics API
// George F. Riley, Georgia Tech, Spring 2015

#ifndef __GTDISPLAY_H__
#define __GTDISPLAY_H__

#include <stdint.h>

// Use an "opaque pointer to point to a generic window.
typedef void* gtWindow; 


// Initialize must be called before any of the other API's
void gtDisplayInit(int argc, char** argv);

// Create a blank (White background) window of specified width, height, and depth
gtWindow gtCreateBlankWindow(int width, int height, int pixelDepth);

// Create a window with the specified image preloaded.
// The window width, height, and depth will be as needed for the specified image.
gtWindow gtLoadImage(const char* name);

// Save a window to an image file.
bool     gtSaveImage(gtWindow, const char* name);

// Get a pointer to the image data (8 bit gray scale) for the specfied window.
uint8_t* gtGet8BitPixels(gtWindow);

// Get a pointer to the image data (32bit RGB) for the specified window
uint32_t* gtGet32BitPixels(gtWindow);

// Redraw the specified window
void gtUpdate(gtWindow);

// Update a partial rectangular region on the window.
// The x and y are the upper left corner, w and h are the
// width and height of the rectangular region.
void gtUpdatePartial(gtWindow, int x, int y, int w, int h);

// Get the window geormetry 
int  gtGetWidth(gtWindow);
int  gtGetHeight(gtWindow);
int  gtGetDepth(gtWindow);

// Set maximum update, frames per second
void gtUpdateRate(gtWindow, int);

// Run the graphics engine drawing as necessary.
void gtRun();

// 32bit color management
uint32_t gtMakeRGB(uint8_t, uint8_t, uint8_t);  // From red/green/blue
uint8_t  gtGetRed(uint32_t);
uint8_t  gtGetGreen(uint32_t);
uint8_t  gtGetBlue(uint32_t);

#endif

