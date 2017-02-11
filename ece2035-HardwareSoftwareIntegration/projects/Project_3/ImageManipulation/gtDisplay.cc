// Implement the "gtDisplay" graphics API
// George F. Riley, Georgia Tech, Spring 2015

#include <iostream>

#include <qwidget.h>
#include <qapplication.h>

#include "gtDisplay.h"
#include "gtWidget.h"

using namespace std;

QApplication* pApp;

// Implement the gtDisplay API
void gtDisplayInit(int argc, char** argv)
{
  pApp = new QApplication(argc, argv);
}

gtWindow gtCreateBlankWindow(int w, int h, int d)
{  // Create a blank window of specified width, height and depth
  GTWidget* widget = new GTWidget;
  widget->BlankImage(w, h, d);
  widget->show();
  widget->closed = 0;
  return (gtWindow) widget;
}

// Get a pointer to the image data (8 bit gray scale) for the specfied window.
uint8_t* gtGet8BitPixels(gtWindow gtw)
{
  GTWidget* w = (GTWidget*)gtw;
  if (!w->image) return 0; // No image
  return (uint8_t*)w->image->bits();  // Pointer to the image data
}

// Get a pointer to the image data (32bit RGB) for the specified window
uint32_t* gtGet32BitPixels(gtWindow gtw)
{
  GTWidget* w = (GTWidget*)gtw;
  if (!w->image) return 0; // No image
  return (uint32_t*)w->image->bits();  // Pointer to the image data
}

void gtUpdate(gtWindow gtw)
{
  GTWidget* w = (GTWidget*)gtw;
  w->Update();
}


void gtUpdateRate(gtWindow gtw, int framesPerSecond)
{
  GTWidget* w = (GTWidget*)gtw;
  w->UpdateRate(framesPerSecond);
}

gtWindow gtLoadImage(const char* name)
{
  QImage* image = new QImage();
  if (!image->load(name))
    { // Loca failed return false
      cout << "Load failed for " << name << endl;
      delete image;
      return 0;
    }
  // Create the window
  GTWidget* widget = new GTWidget;
  widget->image = image;
  widget->resize(image->width(), image->height());
  widget->Update();
  widget->Show();
  //cout << "Width is " << image->width() << " height " << image->height() << endl;
  return widget;
}

bool gtSaveImage(gtWindow gtw, const char* fn)
{
  GTWidget* w = (GTWidget*)gtw;
  if (!w->image) return false; // no image to save
  return (w->image->save(fn));
  }


int gtGetWidth(gtWindow gtw)
{
  GTWidget* w = (GTWidget*)gtw;
  return w->Width();
}

int gtGetHeight(gtWindow gtw)
{
  GTWidget* w = (GTWidget*)gtw;
  return w->Height();
}

int gtGetDepth(gtWindow gtw)
{
  GTWidget* w = (GTWidget*)gtw;
  return w->Depth();
}

void gtRun()
{ // Just call app.exec to process events
  pApp->exec();
}


// 32 bit color management
uint32_t gtMakeRGB(uint8_t r, uint8_t g, uint8_t b)  // From red/green/blue
{
  uint32_t ret = r << 16 | g << 8 | b;
  return ret;
}

uint8_t  gtGetRed(uint32_t rgb)
{
  return (uint8_t)((rgb >> 16) & 0xff);
}

uint8_t  gtGetGreen(uint32_t rgb)
{
  return (uint8_t)((rgb >> 8) & 0xff);
}

uint8_t  gtGetBlue(uint32_t rgb)
{
  return (uint8_t)((rgb >> 0) & 0xff);
}

