// Define the GTWidget

#include <stdint.h>
#include <QWidget>

class GTWidget : public QWidget 
{
Q_OBJECT
public:
  GTWidget();
  virtual ~GTWidget();        // Destructor
  bool Load(const char*);    // Load image from a file
  void BlankImage(int, int, int); // Create a blank grayscale image, w h d
  int   Depth();              // Get image depth
  int   Width();              // Get image width
  int   Height();             // Get image height
  uint8_t* gtGet8BitPixels(); // Get the 8-bit pixel array
  uint32_t* get32BitPixels(); // Get the 32-bit pixel array
  void  Show(bool s = true);  // Set window visibility
  void  Update();             // Update the entire screen image
  void  Update(const QRect&qq, bool noPaint = false);
  void  UpdateRate(int);      // Set update rate (frames/sec) 0 = infinite
  // Inherited from QMainWindow
  void paintEvent(QPaintEvent*);

public:
  bool           ready;       // True if timer expired and ready for update
  bool           closed;      // If main window closed
  QTimer*        qTimer;      // Ready timer, to enforce 25fps update rate
  QImage*        image;       // Current image
  int            updateRate;  // Update rate (frames per sec)
  QVector<QRgb>  colorTable;  // Grayscale color table

public slots:
  void timerDone();
};
