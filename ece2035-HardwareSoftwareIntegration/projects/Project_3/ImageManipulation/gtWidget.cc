// Implement the GTWidget class
// George F. Riley, Georgia Tech, Spring 2015

#include <QWidget>
#include <QApplication>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>

#include <iostream>
#include <pthread.h>

#include "gtWidget.h"


using namespace std;

extern QApplication* pApp;


// Implement the GTWidget class
GTWidget::GTWidget()
{ // need more here
  qTimer = 0;
  image = 0;
  updateRate = 0;
  ready = true;
  qTimer = new QTimer();
  connect(qTimer, SIGNAL(timeout()), SLOT(timerDone()));  

  // Connect application "lastwindowclosed" signal
  //connect(pApp, SIGNAL(lastWindowClosed()), SLOT(mainClosed()));

}

GTWidget::~GTWidget()
{ // Clean up memory here
  
}


void GTWidget::BlankImage(int w, int h, int depth)
// code later
{  // Create a blank image of the specified color depth
  // If 8 bit, we need a grayscale colormap
  if (depth == 8)
    {
      if (colorTable.empty())
        {
          for (int i = 0; i < 256; ++i)
            {
              colorTable.push_back(QColor(i, i, i).rgb());
            }
        }
    }
  
  if (image)
    { // Delete any existing
      delete image;
      image = 0;
    }
  
  // Create the image
  if (depth == 8)
    {
      image = new QImage(w, h, QImage::Format_Indexed8);
      image->setColorTable(colorTable);
      image->fill(0xff); // All white
    }
  else
    {
      image = new QImage(w, h, QImage::Format_RGB32);
      image->fill(0x00ffffff); // All white
    }
  
  show();
  resize(image->width(), image->height()); // Size the main window to image
  Update(); // And draw on screen
}

void  GTWidget::Update()
{
  if (closed) return; // No more updates after window closed
  //cout << "Hello frmo GTW::Update ready " << ready << " closed " << closed << endl;
  while(!ready && !closed) 
    {
      //cout << "Yielding" << endl;
      //pthread_yield_np();
      pthread_yield();
      pApp->processEvents(); // Wait for 25fps elapsed 
    }
  if (updateRate)
    { // Non-zero update rate specified
      ready = false;
      qTimer->setInterval(1000/updateRate); // 40ms is 25 frames per second
      qTimer->start(TRUE);  
    }
  // Update image here
  update();                                  // Force re-paint event later
  //repaint();                                  // Force re-paint event now
  pApp->processEvents();
}

void GTWidget::timerDone()
{
  ready = true;
}

// Inherited from QMainWindow
void GTWidget::paintEvent(QPaintEvent* pe)
{
  if (image)
    {
      QRect r = pe->rect();
      QPainter p(this);
      p.drawImage(r, *image, r);
    }
}

void GTWidget::UpdateRate(int r)
{
  updateRate = r;
}

// Image geometry
int  GTWidget::Depth()
{ // Return image depth (bits per pixel), either 1, 8, or 32.
  // Note, the 32 bit images are actually 24 bits, with 8 bits of filler
  // for word alignment
  if (!image) return 0; // No image loaded
  int d = image->depth();
  if (d == 24) d = 32;
  return d;             // Bits per pixel
}

int  GTWidget::Width()
{ // Return image width (pixels)
  if (!image) return 0;
  return image->width();
}

int  GTWidget::Height()
{ // Return image height (pixels)
  if (!image) return 0;
  return image->height();
}
  
// Load an image file
bool GTWidget::Load(const char* fn)
{ // Load an image from a file
  image = new QImage();
  if (!image->load(fn))
    { // load failed
      delete image;
      image = 0;
      return false;
    }
  //show();
  resize(image->width(), image->height()); // Size the main window to image
  Update();
  return true;
}

void GTWidget::Show(bool s)
{
  if (s) show();
}

