#ifndef CLOCK_H
#define CLOCK_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <RTClib.h>

#include "WiFiManager.h"
#include "LEDMatrix.h"

class Clock
{
public:
   Clock();
   ~Clock();

   void SetEnabled(bool enabled);

   void SetIntensity(int level);

   void Clear();

   void Loop();

   void Init();

   void WiFiNTP();

   void WiFiConfig();

   void WiFiCheck();

private:
   enum class ClockState
   {
      INIT_STATE,         // 初始化状态
      WIFI_CONFIG_STATE,  // WIFI配网状态
      TIME_DISPLAY_STATE, // 显示时间状态
      NTPCAL_STATE,       // NTP校准状态
      WIFI_CHECK_STATE,   // WIFI检查状态
      ERROR_STATE,        // 出错状态
   };

   void ShowInit();
   void ShowOK();
   void ShowNoOK();
   void ShowAPConfig();
   void ShowConnectingWiFi();
   void ShowNTPCalibration();
   void UpdateTime();
   bool NTPCalibration(bool force = false);
   // 处理每个状态的enter
   void StateEnter();
   // 处理每个状态exit
   void StateExit();
   // 处理每个状态的run
   void StateRun();

private:
   const uint8_t LEDMATRIX_CS_PIN = 5;
   const int LEDMATRIX_SEGMENTS = 4;
   const int LEDMATRIX_WIDTH = LEDMATRIX_SEGMENTS * 8;

   WiFiUDP mNTPUDP;
   LEDMatrix *mLEDMatrix = NULL;
   NTPClient *mNTPClient = NULL;
   RTC_DS3231 mRTC;

   ClockState mCurState = ClockState::INIT_STATE;
   ClockState mPreState = ClockState::INIT_STATE;
   ClockState mNewState = ClockState::INIT_STATE;

   uint64_t mNTPCalLastMS = 0;
   uint64_t mLastUpdateTimeMS = 0;
   uint64_t mStartWiFiCheckMS = 0;

   bool mNTPCaliResult = false;
};
#endif // CLOCK_H