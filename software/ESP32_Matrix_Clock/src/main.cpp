#include <Arduino.h>

#include <SPI.h>

// max7219 registers
#define MAX7219_REG_NOOP 0x0
#define MAX7219_REG_DIGIT0 0x1
#define MAX7219_REG_DIGIT1 0x2
#define MAX7219_REG_DIGIT2 0x3
#define MAX7219_REG_DIGIT3 0x4
#define MAX7219_REG_DIGIT4 0x5
#define MAX7219_REG_DIGIT5 0x6
#define MAX7219_REG_DIGIT6 0x7
#define MAX7219_REG_DIGIT7 0x8
#define MAX7219_REG_DECODEMODE 0x9
#define MAX7219_REG_INTENSITY 0xA
#define MAX7219_REG_SCANLIMIT 0xB
#define MAX7219_REG_SHUTDOWN 0xC
#define MAX7219_REG_DISPLAYTEST 0xF

// spi pins
#define VSPI_CLK 18
#define VSPI_CS 5
#define VSPI_MOSI 23
#define VSPI_MISO 19

// LED Maxtrix
#define LED_MAXTRIX_COUNT 4

int Count = 0;

void SPI_SendByte(const byte reg, const byte data)
{
  digitalWrite(VSPI_CS, LOW);
  for (byte device = 0; device < LED_MAXTRIX_COUNT; device++)
  {
    // sendByte(device, reg, data);
    SPI.transfer(reg);
    SPI.transfer(data);
  }
  digitalWrite(VSPI_CS, HIGH);
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Setup Begin\r\n");
  pinMode(VSPI_CS, OUTPUT);
  SPI.begin(VSPI_CLK, VSPI_MISO, VSPI_MOSI, VSPI_CS);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI_SendByte(MAX7219_REG_SCANLIMIT, 7);   // show all 8 digits
  SPI_SendByte(MAX7219_REG_DECODEMODE, 0);  // using an led matrix (not digits)
  SPI_SendByte(MAX7219_REG_DISPLAYTEST, 0); // no display test
  SPI_SendByte(MAX7219_REG_INTENSITY, 0);   // character intensity: range: 0 to 15
  SPI_SendByte(MAX7219_REG_SHUTDOWN, 1);    // not in shutdown mode (ie. start it up)

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if ((i % 2) == 0)
      {
        SPI_SendByte(i + 1, 0xAA);
      }
      else
      {
        SPI_SendByte(i + 1, 0x55);
      }
    }
  }

  Serial.print("Setup End\r\n");
}

void loop()
{
  String input = Serial.readStringUntil('\n'); // read user intput
  Serial.print("Received: ");                  // echo
  Serial.println(input);
}
