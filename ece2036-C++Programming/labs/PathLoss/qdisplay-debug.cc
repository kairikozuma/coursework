// Interface to Qt graphics library
// George F. Riley, Georgia Tech, Spring 2004
// ECE3090, Spring 2004

#include <iostream>
#include <fstream>
#include <pthread.h>

#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>

#include "qdisplay.h"

#define nil 0

using namespace std;

int QDisplay::windowCount = 0;

// // QApp methods
// QApp::QApp(int argc, char** argv) 
//     : QApplication(argc, argv)
// {
//   // Connect application "lastwindowclosed" signal
//   connect(this, SIGNAL(lastWindowClosed()), SLOT(quit()));
// }

// void QApp::Run()
// {
//   exec();
// }

// Constructors
QDisplay::QDisplay(QApplication& a)
    : ready(true), closed(false), qTimer(nil), image(nil), app(a),
    updateRate(0), white(nil), whitew(0), whiteh(0)
{
  // Create and connect the timer
  qTimer = new QTimer();
  connect(qTimer, SIGNAL(timeout()), SLOT(timerDone()));  

  // Connect application "lastwindowclosed" signal
  connect(&app, SIGNAL(lastWindowClosed()), SLOT(mainClosed()));
  //updateRate = 10;
}

QDisplay::~QDisplay()
{ // Destructor
}

// Member functions
bool QDisplay::Load(const char* fn)
{ // Load an image from a file
  image = new QImage();
  if (!image->load(fn))
    { // load failed
      delete image;
      image = nil;
      return false;
    }
  //show();
  resize(image->width(), image->height()); // Size the main window to image
  Update();
  windowCount++;
  return true;
}

bool QDisplay::Save(const  char* fn)
{
  if (!image) return false; // no image to save
  image->save(fn);
  return true;
#ifdef OLD_WAY_NO_LONGER_USED
  ofstream fs(fn);
  if (!fs) return false;
  fs << "P5" << endl;
  fs << "# CREATOR George F. RILEY" << endl;
  fs << "# GENERATOR: QDisplay::Save" << endl;
  fs << image->width() << " " << image->height() << endl;
  fs << image->numColors() << endl;
  unsigned char* b = image->bits();
  for (int y = 0; y < image->height(); ++y)
    {
      for (int x = 0; x < image->width(); ++x)
        fs << b[y*image->width() + x];
    }
  fs.close();
  return true;
#endif
}

void QDisplay::BlankImage(int w, int h, int d)
{  // Create a blank image of the specified color depth
  // If 8 bit, we need a grayscale colormap
  if (d == 8)
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
      image = nil;
    }
  
  // Create the image
  if (d == 8)
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
  
  //show();
  resize(image->width(), image->height()); // Size the main window to image
  Update(); // And draw on screen
  windowCount++;
}

  
int  QDisplay::Depth()
{ // Return image depth (bits per pixel), either 1, 8, or 32.
  // Note, the 32 bit images are actually 24 bits, with 8 bits of filler
  // for word alignment
  if (!image) return 0; // No image loaded
  int d = image->depth();
  if (d == 24) d = 32;
  return d;             // Bits per pixel
}

int  QDisplay::Width()
{ // Return image width (pixels)
  if (!image) return 0;
  return image->width();
}

int  QDisplay::Height()
{ // Return image height (pixels)
  if (!image) return 0;
  return image->height();
}
  
unsigned char* QDisplay::ImageData()
{ // Get a pointer to the image data
  if(!image) return nil; // No image loaded
  //cout << "Image data has " << image->numBytes() << " bytes " << endl;
  return image->bits();
}

void  QDisplay::Update()
{
  cout << "::Update" << endl;
  if (closed) cout << " closed";
  if (ready) cout << " ready " << endl;
  if (closed) return; // No more updates after window closed
  while(!ready && !closed)
    {
      //pthread_yield();
      app.processEvents(); // Wait for 25fps elapsed 
    }
  
  cout << "updateReate is " << updateRate << endl;
  
  if (updateRate)
    { // Non-zero update rate specified
      ready = false;
      qTimer->setInterval(1000/updateRate); // 40ms is 25 frames per second
      qTimer->start(1000/updateRate);  
      cout << "StartedTimer" << endl;
    }
  // Update image here
  update();                                  // Force re-paint event later
  //repaint();                                  // Force re-paint event now
 app.processEvents();
  ready = false;
}

void  QDisplay::Update(int x, int y, int w, int h)
{
  if (closed) return; // No more updates after window closed
  while(!ready && !closed) app.processEvents(); // Wait for 25fps elapsed 
  if (updateRate)
    { // Non-zero update rate specified
      ready = false;
      qTimer->setInterval(1000/updateRate);
      qTimer->start(1000/updateRate);  // 40ms is 25 frames per second
    }
  // Update the pixel
  if (image)
    {
      //repaint(x, y, 1, 1);
      update(x, y, w, h);
    }
  app.processEvents();
}

bool QDisplay::IsClosed()
{
  return closed;
}

void QDisplay::UpdateRate(int r)
{
  updateRate = r;
  cout << "Updat rate set to " << r << endl;
  qTimer->start(1000/updateRate);  

}

void QDisplay::Show()
{
  show();
}

  
// Inherited from QMainWindow
void QDisplay::paintEvent(QPaintEvent* pe)
{
  if (image)
    {
      QRect r = pe->rect();
      QPainter p(this);
      cout << "Updating image a x " << r.x()
           << " y " << r.y()
           << " w " << r.width()
           << " h " << r.height()
           << endl;
      p.drawImage(r, *image, r);
    }
}

// Slots
void QDisplay::timerDone()
{
  ready = true; // Enough time elapsed for another frame update
  cout << "Timer is done, ready set true" << endl;
}

void QDisplay::mainClosed()
{
  closed = true; // Main window has closed
}

