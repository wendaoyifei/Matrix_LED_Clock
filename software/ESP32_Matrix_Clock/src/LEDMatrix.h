#ifndef LED_MATRIX_H
#define LED_MATRIX_H

// #define USE_ADAFRUIT_GFX
#include <LEDMatrixDriver.hpp>

class LEDMatrix : public LEDMatrixDriver
{
public:
   LEDMatrix(uint8_t N, uint8_t ssPin);
#ifdef USE_ADAFRUIT_GFX
   virtual
#endif
       ~LEDMatrix();

#ifndef USE_ADAFRUIT_GFX
   void drawBigString(const char *string, int len, int x, int y, uint8_t whitespace);
   uint8_t drawBigChar(char c, int x, int y);
   void drawSmallString(const char *string, int len, int x, int y, uint8_t whitespace);
   uint8_t drawSmallChar(char c, int x, int y);
#endif

private:
};

#endif // LED_MATRIX_H
