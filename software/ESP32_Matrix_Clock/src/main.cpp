#include <Arduino.h>
#include "WiFiManager.h"

void setup()
{
  Serial.begin(115200);
  Serial.print("Setup Begin\r\n");
  WiFiManager_Init();
  WiFiManager_Scan();
  Serial.print("Setup End\r\n");
}

void loop()
{
  // String input = Serial.readStringUntil('\n'); // read user intput
  // Serial.print("Received: ");                  // echo
  // Serial.println(input);
  WiFiManager_Run();
}
