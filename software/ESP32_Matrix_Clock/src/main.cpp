#include <Arduino.h>

#include "WiFiManager.h"
#include "LEDMatrix.h"

const uint8_t LEDMATRIX_CS_PIN = 5;

const int LEDMATRIX_SEGMENTS = 4;
const int LEDMATRIX_WIDTH = LEDMATRIX_SEGMENTS * 8;

// The LEDMatrixDriver class instance
LEDMatrix matrix(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);

void setup()
{
  Serial.begin(115200);
  Serial.print("Setup Begin\r\n");
  // init the display
  matrix.setEnabled(true);
  matrix.setIntensity(2); // 0 = low, 10 = high

  // matrix.drawBigString("11:13", strlen("11:13"), 0, 0);
  // matrix.drawBigString("1", strlen("1"), 0, 0);
  // matrix.drawBigChar('5', 0, 0);
  // matrix.drawBigChar('4', 6, 0);
  // matrix.drawBigChar('2', 12, 0);
  // matrix.drawBigChar('1', 18, 0);

  matrix.drawBigString("15:56", strlen("15:56"), 0, 0, 0);

  // matrix.drawSmallChar('5', 24, 0);
  // matrix.drawSmallChar('5', 28, 0);
  matrix.drawSmallString("58", strlen("58"), 24, 0, 1);
  matrix.display();
  // WiFiManager_Init();
  // WiFiManager_Scan();
  Serial.print("Setup End\r\n");
}

void loop()
{
  // String input = Serial.readStringUntil('\n'); // read user intput
  // Serial.print("Received: ");                  // echo
  // Serial.println(input);
  // WiFiManager_Run();
}

#if 0

#include <LEDMatrixDriver.hpp>

// This sketch draw marquee text on your LED matrix using the hardware SPI driver Library by Bartosz Bielawski.
// Example written 16.06.2017 by Marko Oette, www.oette.info

// Define the ChipSelect pin for the led matrix (Dont use the SS or MISO pin of your Arduino!)
// Other pins are Arduino specific SPI pins (MOSI=DIN, SCK=CLK of the LEDMatrix) see https://www.arduino.cc/en/Reference/SPI
const uint8_t LEDMATRIX_CS_PIN = 5;

// Number of 8x8 segments you are connecting
const int LEDMATRIX_SEGMENTS = 4;
const int LEDMATRIX_WIDTH = LEDMATRIX_SEGMENTS * 8;

// The LEDMatrixDriver class instance
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);

// Marquee text
// char text[] = "** LED MATRIX DEMO! ** (1234567890) ++ \"ABCDEFGHIJKLMNOPQRSTUVWXYZ\" ++ <$%/=?'.@,> --";
char text[] = "WIFI...";
// char text[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~";
//  Marquee speed (lower nubmers = faster)
const int ANIM_DELAY = 30;

void drawSprite(byte *sprite, int x, int y, int width, int height);
void drawString(char *text, int len, int x, int y);

void setup()
{
  // init the display
  lmd.setEnabled(true);
  lmd.setIntensity(2); // 0 = low, 10 = high

  int len = strlen(text);
  drawString(text, len, 0, 0);
  // In case you wonder why we don't have to call lmd.clear() in every loop: The font has a opaque (black) background...

  // Toggle display of the new framebuffer
  lmd.display();
}

int x = 0, y = 0; // start top left

// This is the font definition. You can use http://gurgleapps.com/tools/matrix to create your own font or sprites.
// If you like the font feel free to use it. I created it myself and donate it to the public domain.
byte font[95][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},                         // SPACE
    {0x10, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18}, // EXCL
    {0x28, 0x28, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00}, // QUOT
    {0x00, 0x0a, 0x7f, 0x14, 0x28, 0xfe, 0x50, 0x00}, // #
    {0x10, 0x38, 0x54, 0x70, 0x1c, 0x54, 0x38, 0x10}, // $
    {0x00, 0x60, 0x66, 0x08, 0x10, 0x66, 0x06, 0x00}, // %
    {0, 0, 0, 0, 0, 0, 0, 0},                         // &
    {0x00, 0x10, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00}, // '
    {0x02, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04}, // (
    {0x40, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20}, // )
    {0x00, 0x10, 0x54, 0x38, 0x10, 0x38, 0x54, 0x10}, // *
    {0x00, 0x08, 0x08, 0x08, 0x7f, 0x08, 0x08, 0x08}, // +
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08}, // COMMA
    {0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00}, // -
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06}, // DOT
    {0x00, 0x04, 0x04, 0x08, 0x10, 0x20, 0x40, 0x40}, // /
    {0x00, 0x38, 0x44, 0x4c, 0x54, 0x64, 0x44, 0x38}, // 0
    {0x04, 0x0c, 0x14, 0x24, 0x04, 0x04, 0x04, 0x04}, // 1
    {0x00, 0x30, 0x48, 0x04, 0x04, 0x38, 0x40, 0x7c}, // 2
    {0x00, 0x38, 0x04, 0x04, 0x18, 0x04, 0x44, 0x38}, // 3
    {0x00, 0x04, 0x0c, 0x14, 0x24, 0x7e, 0x04, 0x04}, // 4
    {0x00, 0x7c, 0x40, 0x40, 0x78, 0x04, 0x04, 0x38}, // 5
    {0x00, 0x38, 0x40, 0x40, 0x78, 0x44, 0x44, 0x38}, // 6
    {0x00, 0x7c, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10}, // 7
    {0x00, 0x3c, 0x44, 0x44, 0x38, 0x44, 0x44, 0x78}, // 8
    {0x00, 0x38, 0x44, 0x44, 0x3c, 0x04, 0x04, 0x78}, // 9
    {0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00}, // :
    {0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x08}, // ;
    {0x00, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10}, // <
    {0x00, 0x00, 0x7e, 0x00, 0x00, 0xfc, 0x00, 0x00}, // =
    {0x00, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08}, // >
    {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x00, 0x10}, // ?
    {0x00, 0x30, 0x48, 0xba, 0xba, 0x84, 0x78, 0x00}, // @
    {0x00, 0x1c, 0x22, 0x42, 0x42, 0x7e, 0x42, 0x42}, // A
    {0x00, 0x78, 0x44, 0x44, 0x78, 0x44, 0x44, 0x7c}, // B
    {0x00, 0x3c, 0x44, 0x40, 0x40, 0x40, 0x44, 0x7c}, // C
    {0x00, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x44, 0x78}, // D
    {0x00, 0x78, 0x40, 0x40, 0x70, 0x40, 0x40, 0x7c}, // E
    {0x00, 0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40}, // F
    {0x00, 0x3c, 0x40, 0x40, 0x5c, 0x44, 0x44, 0x78}, // G
    {0x00, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42}, // H
    {0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7e}, // I
    {0x00, 0x7e, 0x02, 0x02, 0x02, 0x02, 0x04, 0x38}, // J
    {0x00, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44}, // K
    {0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c}, // L
    {0x00, 0x82, 0xc6, 0xaa, 0x92, 0x82, 0x82, 0x82}, // M
    {0x00, 0x42, 0x42, 0x62, 0x52, 0x4a, 0x46, 0x42}, // N
    {0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x44, 0x38}, // O
    {0x00, 0x78, 0x44, 0x44, 0x48, 0x70, 0x40, 0x40}, // P
    {0x00, 0x3c, 0x42, 0x42, 0x52, 0x4a, 0x44, 0x3a}, // Q
    {0x00, 0x78, 0x44, 0x44, 0x78, 0x50, 0x48, 0x44}, // R
    {0x00, 0x38, 0x40, 0x40, 0x38, 0x04, 0x04, 0x78}, // S
    {0x00, 0x7e, 0x90, 0x10, 0x10, 0x10, 0x10, 0x10}, // T
    {0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3e}, // U
    {0x00, 0x42, 0x42, 0x42, 0x42, 0x44, 0x28, 0x10}, // V
    {0x80, 0x82, 0x82, 0x92, 0x92, 0x92, 0x94, 0x78}, // W
    {0x00, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42}, // X
    {0x00, 0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10}, // Y
    {0x00, 0x7c, 0x04, 0x08, 0x7c, 0x20, 0x40, 0xfe}, // Z
                                                      // (the font does not contain any lower case letters. you can add your own.)
}; // {}, //

void loop()
{
#if 0
  // Draw the text to the current position
  int len = strlen(text);
  drawString(text, len, x, 0);
  // In case you wonder why we don't have to call lmd.clear() in every loop: The font has a opaque (black) background...

  // Toggle display of the new framebuffer
  lmd.display();

  // Wait to let the human read the display
  delay(ANIM_DELAY);

  // Advance to next coordinate
  if (--x < len * -8)
  {
    x = LEDMATRIX_WIDTH;
  }
#endif
}

/**
 * This function draws a string of the given length to the given position.
 */
void drawString(char *text, int len, int x, int y)
{
  for (int idx = 0; idx < len; idx++)
  {
    int c = text[idx] - 32;

    // stop if char is outside visible area
    if (x + idx * 8 > LEDMATRIX_WIDTH)
      return;

    // only draw if char is visible
    if (8 + x + idx * 8 > 0)
      drawSprite(font[c], x + idx * 8, y, 8, 8);
  }
}

/**
 * This draws a sprite to the given position using the width and height supplied (usually 8x8)
 */
void drawSprite(byte *sprite, int x, int y, int width, int height)
{
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;

  for (int iy = 0; iy < height; iy++)
  {
    for (int ix = 0; ix < width; ix++)
    {
      lmd.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask));

      // shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // reset column mask
    mask = B10000000;
  }
}
#endif

#if 0
#include <LEDMatrixDriver.hpp>

/*
 *  A very efficient and versatile, no-whitespace marquee example written by James Gohl 20.03.2019.
 *    - Using hardware SPI driver by Bartosz Bielawski.
 *
 *  This example features:
 *
 *    - Font character definitions are "bunched" together in a one-dimensional array
 *        and addressed using indexing.  The array is stored in program space
 *        freeing up RAM and saving a significant amount of space compared to
 *        other examples.
 *    - Whitespace in characters is ignored giving a cleaner appearance,
 *        character spacing is specified in nextCol().
 *    - Scrolling is performed at driver level; shifting the buffer contents
 *        left, while writing one column from the current character to the
 *        right-side of the display.
 *    - Timing is performed using elapsed milli seconds, to avoid blocking on other
 *        functions you might implement.
 *    - If you would like to draw your own fonts using my LEDMatrixPainter,
 *        you can get it here (https://github.com/jamesgohl/LEDMatrixPainter).
 *
 *  Wiring for Arduino UNO:
 *
 *    MAX7219 MODULE(S)        ARDUINO UNO
 *     +-------------+
 *     /             |
 *     \         CLK --------- SCK  (PIN 13)
 *     /          CS --------- CS   (PIN 9)
 *     \         DIN --------- MOSI (PIN 11)
 *     /         GND --------- GND
 *     \         VCC --------- 5V
 *     /             |
 *     +-------------+
 *
 *  Do NOT use the SS or MISO pin on your Arduino.
 *
 *  For other models, check here for SPI reference:
 *
 *    https://www.arduino.cc/en/Reference/SPI
 *
 */
// Set the Chip Select (CS) pin for the led matrix
const uint8_t LEDMATRIX_CS_PIN = 5;
// Set the number of modules you have daisy-chained together.
const uint8_t LEDMATRIX_SEGMENTS = 4;
// Width of entire display, where 8 is the number of pixels on a single module.
const uint16_t LEDMATRIX_WIDTH = LEDMATRIX_SEGMENTS * 8;

/*
 *  Initialise LEDMatrixDriver
 *    Since there are a lot of variants of led matrix modules out there,
 *    you can invert the column / row addressing per led segment by adding
 *    up the flags specified in argument 3, here are the options:
 *
 *        INVERT_SEGMENT_X
 *        INVERT_DISPLAY_X
 *        INVERT_Y
 *
 */
LEDMatrixDriver led(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);

/*
 *  Scroll Delay is performed using a tick count,
 *    this ensures scroll rate is closer to the specified delay value
 *    instead of using delay(), which adds to the processing delay.
 *
 *  Set SCROLL_DELAY to 0 to see the efficiency of this example.
 */
const uint16_t SCROLL_DELAY = 30;
// No need to alter this.
uint64_t marqueeDelayTimestamp = 0;

/*
 *  Font is derived from "5 by 7 Regular" by Peter Weigel with a few
 *  modifications to clean-up.
 *
 *  Stored in flash instead of RAM, for more info on this visit:
 *  //https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
 *
 *  The first byte per chatacter signifies the width in decimal form,
 *  the rest define the pixels for each column in the char. They are
 *  in bit form for easy editing.
 *
 *  We store a font_index[] since our characters are bunched together in a one-
 *  dimensional array, if you make any changes to a char width, all of the
 *  indices thereafter will need to be increased, use a simple for-loop to
 *  re-calculate and output to serial:
 *
 *    Serial.begin(9600);
 *    int cur_idx = 0;
 *    for(int i = 0; i < 94; i++)
 *    {
 *      Serial.print(cur_idx);
 *      Serial.print(",");
 *      Serial.flush();
 *      cur_idx += pgm_read_byte(&(font[cur_idx])) + 1;
 *    }
 *
 */
static const uint8_t font[515] PROGMEM = {
    2, 0b00000000, 0b00000000,                                     /* 032 =   */
    2, 0b01101111, 0b01101111,                                     /* 033 = ! */
    3, 0b00000011, 0b00000000, 0b00000011,                         /* 034 = " */
    5, 0b00010100, 0b01111111, 0b00010100, 0b01111111, 0b00010100, /* 035 = # */
    5, 0b00100100, 0b00101010, 0b01111111, 0b00101010, 0b00010010, /* 036 = $ */
    5, 0b00100011, 0b00010011, 0b00001000, 0b01100100, 0b01100010, /* 037 = % */
    5, 0b00110100, 0b01001010, 0b01001010, 0b00110100, 0b01010000, /* 038 = & */
    1, 0b00000011,                                                 /* 039 = ' */
    3, 0b00011100, 0b00100010, 0b01000001,                         /* 040 = ( */
    3, 0b01000001, 0b00100010, 0b00011100,                         /* 041 = ) */
    3, 0b00000101, 0b00000010, 0b00000101,                         /* 042 = * */
    5, 0b00001000, 0b00001000, 0b00111110, 0b00001000, 0b00001000, /* 043 = + */
    2, 0b11100000, 0b01100000,                                     /* 044 = , */
    5, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, /* 045 = - */
    2, 0b01100000, 0b01100000,                                     /* 046 = . */
    5, 0b01000000, 0b00110000, 0b00001000, 0b00000110, 0b00000001, /* 047 = / */
    5, 0b00111110, 0b01010001, 0b01001001, 0b01000101, 0b00111110, /* 048 = 0 */
    3, 0b01000010, 0b01111111, 0b01000000,                         /* 049 = 1 */
    5, 0b01000010, 0b01100001, 0b01010001, 0b01001001, 0b01000110, /* 050 = 2 */
    5, 0b00100001, 0b01000001, 0b01000101, 0b01001011, 0b00110001, /* 051 = 3 */
    5, 0b00011000, 0b00010100, 0b00010010, 0b01111111, 0b00010000, /* 052 = 4 */
    5, 0b00100111, 0b01000101, 0b01000101, 0b01000101, 0b00111001, /* 053 = 5 */
    5, 0b00111100, 0b01001010, 0b01001001, 0b01001001, 0b00110000, /* 054 = 6 */
    5, 0b00000001, 0b00000001, 0b01111001, 0b00000101, 0b00000011, /* 055 = 7 */
    5, 0b00110110, 0b01001001, 0b01001001, 0b01001001, 0b00110110, /* 056 = 8 */
    5, 0b00000110, 0b01001001, 0b01001001, 0b00101001, 0b00011110, /* 057 = 9 */
    2, 0b00110110, 0b00110110,                                     /* 058 = : */
    2, 0b01110110, 0b00110110,                                     /* 059 = ; */
    4, 0b00001000, 0b00010100, 0b00100010, 0b01000001,             /* 060 = < */
    5, 0b00010100, 0b00010100, 0b00010100, 0b00010100, 0b00010100, /* 061 = = */
    4, 0b01000001, 0b00100010, 0b00010100, 0b00001000,             /* 062 = > */
    5, 0b00000010, 0b00000001, 0b01010001, 0b00001001, 0b00000110, /* 063 = ? */
    5, 0b00111110, 0b01000001, 0b01011101, 0b01010101, 0b01011110, /* 064 = @ */
    5, 0b01111110, 0b00001001, 0b00001001, 0b00001001, 0b01111110, /* 065 = A */
    5, 0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b00110110, /* 066 = B */
    5, 0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00100010, /* 067 = C */
    5, 0b01111111, 0b01000001, 0b01000001, 0b01000001, 0b00111110, /* 068 = D */
    5, 0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b01000001, /* 069 = E */
    5, 0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00000001, /* 070 = F */
    5, 0b00111110, 0b01000001, 0b01001001, 0b01001001, 0b01111010, /* 071 = G */
    5, 0b01111111, 0b00001000, 0b00001000, 0b00001000, 0b01111111, /* 072 = H */
    5, 0b01000001, 0b01000001, 0b01111111, 0b01000001, 0b01000001, /* 073 = I */
    5, 0b00100000, 0b01000001, 0b01000001, 0b00111111, 0b00000001, /* 074 = J */
    5, 0b01111111, 0b00001000, 0b00010100, 0b00100010, 0b01000001, /* 075 = K */
    5, 0b01111111, 0b01000000, 0b01000000, 0b01000000, 0b01000000, /* 076 = L */
    5, 0b01111111, 0b00000010, 0b00000100, 0b00000010, 0b01111111, /* 077 = M */
    5, 0b01111111, 0b00000110, 0b00001000, 0b00110000, 0b01111111, /* 078 = N */
    5, 0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00111110, /* 079 = O */
    5, 0b01111111, 0b00010001, 0b00010001, 0b00010001, 0b00001110, /* 080 = P */
    5, 0b00111110, 0b01000001, 0b01010001, 0b00100001, 0b01011110, /* 081 = Q */
    5, 0b01111111, 0b00001001, 0b00011001, 0b00101001, 0b01000110, /* 082 = R */
    5, 0b00100110, 0b01001001, 0b01001001, 0b01001001, 0b00110010, /* 083 = S */
    5, 0b00000001, 0b00000001, 0b01111111, 0b00000001, 0b00000001, /* 084 = T */
    5, 0b00111111, 0b01000000, 0b01000000, 0b01000000, 0b00111111, /* 085 = U */
    5, 0b00011111, 0b00100000, 0b01000000, 0b00100000, 0b00011111, /* 086 = V */
    5, 0b00111111, 0b01000000, 0b00110000, 0b01000000, 0b00111111, /* 087 = W */
    5, 0b01100011, 0b00010100, 0b00001000, 0b00010100, 0b01100011, /* 088 = X */
    5, 0b00000111, 0b00001000, 0b01110000, 0b00001000, 0b00000111, /* 089 = Y */
    5, 0b01100001, 0b01010001, 0b01001001, 0b01000101, 0b01000011, /* 090 = Z */
    3, 0b01111111, 0b01000001, 0b01000001,                         /* 091 = [ */
    5, 0b00000001, 0b00000110, 0b00001000, 0b00110000, 0b01000000, /* 092 = \ */
    3, 0b01000001, 0b01000001, 0b01111111,                         /* 093 = ] */
    5, 0b00000100, 0b00000010, 0b00000001, 0b00000010, 0b00000100, /* 094 = ^ */
    5, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, /* 095 = _ */
    1, 0b00000011,                                                 /* 096 = ' */
    5, 0b00100000, 0b01010100, 0b01010100, 0b01010100, 0b01111000, /* 097 = a */
    5, 0b01111111, 0b00101000, 0b01000100, 0b01000100, 0b00111000, /* 098 = b */
    5, 0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b00101000, /* 099 = c */
    5, 0b00111000, 0b01000100, 0b01000100, 0b00101000, 0b01111111, /* 100 = d */
    5, 0b00111000, 0b01010100, 0b01010100, 0b01010100, 0b00011000, /* 101 = e */
    5, 0b00000100, 0b01111110, 0b00000101, 0b00000001, 0b00000010, /* 102 = f */
    5, 0b00011000, 0b10100100, 0b10100100, 0b10100100, 0b01111100, /* 103 = g */
    5, 0b01111111, 0b00000100, 0b00000100, 0b00000100, 0b01111000, /* 104 = h */
    3, 0b01000100, 0b01111101, 0b01000000,                         /* 105 = i */
    4, 0b01000000, 0b10000000, 0b10000100, 0b01111101,             /* 106 = j */
    5, 0b01111111, 0b00010000, 0b00010000, 0b00101000, 0b01000100, /* 107 = k */
    3, 0b01000001, 0b01111111, 0b01000000,                         /* 108 = l */
    5, 0b01111100, 0b00000100, 0b01111100, 0b00000100, 0b01111000, /* 109 = m */
    5, 0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b01111000, /* 110 = n */
    5, 0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b00111000, /* 111 = o */
    5, 0b11111100, 0b00100100, 0b00100100, 0b00100100, 0b00011000, /* 112 = p */
    5, 0b00011000, 0b00100100, 0b00100100, 0b00100100, 0b11111100, /* 113 = q */
    5, 0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b00001000, /* 114 = r */
    5, 0b01001000, 0b01010100, 0b01010100, 0b01010100, 0b00100000, /* 115 = s */
    5, 0b00000100, 0b00111110, 0b01000100, 0b01000000, 0b00100000, /* 116 = t */
    5, 0b00111100, 0b01000000, 0b01000000, 0b00100000, 0b01111100, /* 117 = u */
    5, 0b00011100, 0b00100000, 0b01000000, 0b00100000, 0b00011100, /* 118 = v */
    5, 0b00111100, 0b01000000, 0b00110000, 0b01000000, 0b00111100, /* 119 = w */
    5, 0b01000100, 0b00101000, 0b00010000, 0b00101000, 0b01000100, /* 120 = x */
    5, 0b00000100, 0b01001000, 0b00110000, 0b00001000, 0b00000100, /* 121 = y */
    5, 0b01000100, 0b01100100, 0b01010100, 0b01001100, 0b01000100, /* 122 = z */
    3, 0b00001000, 0b00110110, 0b01000001,                         /* 123 = { */
    1, 0b01111111,                                                 /* 124 = | */
    3, 0b01000001, 0b00110110, 0b00001000,                         /* 125 = } */
    5, 0b00011000, 0b00000100, 0b00001000, 0b00010000, 0b00001100  /* 126 = ~ */
};

static const uint16_t font_index[95] PROGMEM = {
    0, 3, 6, 10, 16, 22, 28, 34, 36, 40, 44, 48, 54, 57, 63, 66, 72, 78, 82, 88, 94,
    100, 106, 112, 118, 124, 130, 133, 136, 141, 147, 152, 158, 164, 170, 176,
    182, 188, 194, 200, 206, 212, 218, 224, 230, 236, 242, 248, 254, 260, 266,
    272, 278, 284, 290, 296, 302, 308, 314, 320, 324, 330, 334, 340, 346, 348,
    354, 360, 366, 372, 378, 384, 390, 396, 400, 405, 411, 415, 421, 427, 433,
    439, 445, 451, 457, 463, 469, 475, 481, 487, 493, 499, 503, 505, 509};

/*
 *  Text to scroll, this is hard-coded but if you want to have dynamic
 *    text from Serial input, you should Set scrollWhitespace =
 *    LEDMATRIX_WIDTH , wait for this to return to zero (so the previous
 *    text scrolls away), then set textIndex and colIndex to 0.
 */
// char text[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~";
char text[] = "12:34";

// Current text and column indices.
uint16_t textIndex = 0;
uint8_t colIndex = 0;

/*
 *  Used to set whitespace to the given number of columns,
 *    if > 0 in every loop this value will decrease by 1 and draw nothing while scrolling.
 *    At the end of each character we set this in nextCol() for spacing.
 *    At the end of the text we set this to LEDMATRIX_WIDTH.
 */
uint16_t scrollWhitespace = 0;

void drawString(char *text, int len, int x, int y, uint8_t whitespace)
{
  int currentX = 0;
  for (int charIdx = 0; charIdx < len; charIdx++)
  {
    int asc = text[charIdx] - 32;
    uint16_t fontIdx = pgm_read_word(&(font_index[asc]));
    uint8_t fontWidth = pgm_read_byte(&(font[fontIdx]));
    for (int w = 0; w < (fontWidth + whitespace); w++)
    {
      uint8_t col = 0x00;
      if (w < fontWidth)
      {
        col = pgm_read_byte(&(font[w + fontIdx + 1]));
      }
      led.setColumn(x + currentX, col);
      currentX++;
      if (currentX > LEDMATRIX_WIDTH)
      {
        return;
      }
    }
  }
}

void nextChar()
{
  if (text[++textIndex] == '\0')
  {
    textIndex = 0;
    // Set this to the number of pixels you want drawn between text loops.
    scrollWhitespace = LEDMATRIX_WIDTH;
  }
}

void nextCol(uint8_t w)
{
  if (++colIndex == w)
  {
    // Character spacing, consider increasing this when scrolling is faster.
    scrollWhitespace = 2;
    colIndex = 0;
    nextChar();
  }
}

void writeCol()
{
  if (scrollWhitespace > 0)
  {
    // Deal with whitespace - scroll but don't set LEDs.
    scrollWhitespace--;
    return;
  }

  uint8_t asc = text[textIndex] - 32;

  // We need to use pgm_read_byte and pgm_read_word to read data we put into PROGMEM.
  uint16_t idx = pgm_read_word(&(font_index[asc]));
  uint8_t w = pgm_read_byte(&(font[idx]));
  uint8_t col = pgm_read_byte(&(font[colIndex + idx + 1]));

  led.setColumn(LEDMATRIX_WIDTH - 1, col);

  nextCol(w);
}

void marquee()
{
  // millis() will reset back to zero eventually so we need to handle this.
  if (millis() < 1)
    marqueeDelayTimestamp = 0;
  if (millis() < marqueeDelayTimestamp)
    return;

  marqueeDelayTimestamp = millis() + SCROLL_DELAY;

  // Shift everything left by one led column.
  led.scroll(LEDMatrixDriver::scrollDirection::scrollLeft);

  // Write the next column of leds to the right.
  writeCol();

  // The driver is buffering so we need write all changes.
  led.display();
}

void setup()
{

  led.setEnabled(true);

  // LED brightness (0 - 15).
  led.setIntensity(2);

  drawString(text, strlen(text), 0, 0, 1);
  led.scroll(LEDMatrixDriver::scrollDirection::scrollDown);
  led.display();
}

void loop()
{

  // marquee();
  // Put additional functions in here. If they require a delay, don't use Delay().
  // Instead, include the same delay mechanism but use a different delay timestamp
  // variable, this prevents each function from blocking the next for smoother
  // processing.
}

#endif