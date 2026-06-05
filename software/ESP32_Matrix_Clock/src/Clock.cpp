#include "Clock.h"

Clock::Clock() : mLEDMatrix(new LEDMatrix(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN)),
                 mNTPClient(new NTPClient(mNTPUDP, "cn.ntp.org.cn", 8 * 60 * 60, 60 * 60000))
{
   SetEnabled(false);
}

Clock::~Clock()
{
}

void Clock::SetEnabled(bool enabled)
{
   mLEDMatrix->setEnabled(enabled);
}

void Clock::SetIntensity(int level)
{
   mLEDMatrix->setIntensity(level);
}

void Clock::Clear()
{
   mLEDMatrix->clear();
}

void Clock::Loop()
{
   if (mNewState != mCurState)
   {
      StateExit();
      mPreState = mCurState;
      mCurState = mNewState;
      StateEnter();
   }
   StateRun();
}

void Clock::Init()
{
   SetEnabled(true);
   SetIntensity(0);
   // TODO：检查begin的返回值
   mRTC.begin();
   ShowInit();
}

void Clock::WiFiNTP()
{
   mNewState = Clock::ClockState::NTPCAL_STATE;
}

void Clock::WiFiConfig()
{
   mNewState = Clock::ClockState::WIFI_CONFIG_STATE;
}

void Clock::WiFiCheck()
{
   if (WiFiManager::Instance()->IsConnected())
   {
      Serial.println("WiFi已连接");
   }
   else
   {
      Serial.println("WiFi未连接，尝试重连");
      WiFiManager::Instance()->SetState(WiFiManager::WiFiState::CONNECTING_STATE);
   }
   mNewState = Clock::ClockState::WIFI_CHECK_STATE;
}

void Clock::ShowInit()
{
   mLEDMatrix->clear();
   mLEDMatrix->drawSmallString("Init...", strlen("Init..."), 2, 0, 1);
   mLEDMatrix->display();
}

void Clock::ShowOK()
{
   mLEDMatrix->clear();
   mLEDMatrix->drawSmallString("OK", strlen("OK"), 2, 0, 1);
   mLEDMatrix->display();
}

void Clock::ShowNoOK()
{
   mLEDMatrix->clear();
   mLEDMatrix->drawSmallString("No OK", strlen("No OK"), 2, 0, 1);
   mLEDMatrix->display();
}

void Clock::ShowAPConfig()
{
   mLEDMatrix->clear();
   mLEDMatrix->drawSmallString("Conf...", strlen("Conf..."), 2, 0, 1);
   mLEDMatrix->display();
}

void Clock::ShowConnectingWiFi()
{
   mLEDMatrix->clear();
   mLEDMatrix->drawSmallString("WiFi...", strlen("WiFi..."), 2, 0, 1);
   mLEDMatrix->display();
}

void Clock::ShowNTPCalibration()
{
   mLEDMatrix->clear();
   mLEDMatrix->drawSmallString("Cali...", strlen("Cali..."), 2, 0, 1);
   mLEDMatrix->display();
}

void Clock::UpdateTime()
{
   char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

   if ((millis() - mLastUpdateTimeMS) >= 1000)
   {
      mLastUpdateTimeMS = millis();
      NTPCalibration();
      DateTime now = mRTC.now();

      // Serial.print(now.year(), DEC);
      // Serial.print('/');
      // Serial.print(now.month(), DEC);
      // Serial.print('/');
      // Serial.print(now.day(), DEC);
      // Serial.print(" (");
      // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      // Serial.print(") ");
      // Serial.print(now.hour(), DEC);
      // Serial.print(':');
      // Serial.print(now.minute(), DEC);
      // Serial.print(':');
      // Serial.print(now.second(), DEC);
      // Serial.println();
      mLEDMatrix->clear();
      char hour_min[5];
      sprintf(hour_min, "%02d:%02d", now.hour(), now.minute());
      mLEDMatrix->drawBigString(hour_min, 5, 0, 0, 0);
      char sec[2];
      sprintf(sec, "%02d", now.second());
      mLEDMatrix->drawSmallChar(sec[0], 24, 0);
      mLEDMatrix->drawSmallChar(sec[1], 28, 0);
      mLEDMatrix->display();

#if 0
      unsigned long epoch = mNTPClient->getEpochTime();
      time_t t = epoch;
      struct tm *timeinfo = gmtime(&t);
      mktime(timeinfo);
#if 0
         Serial.print("日期时间：");
         Serial.print(timeinfo->tm_year + 1900);
         Serial.print("-");
         Serial.print(timeinfo->tm_mon + 1);
         Serial.print("-");
         Serial.print(timeinfo->tm_mday);
         Serial.print(" ");
         Serial.print(timeinfo->tm_hour);
         Serial.print(":");
         Serial.print(timeinfo->tm_min);
         Serial.print(":");
         Serial.println(timeinfo->tm_sec);
#endif
      mLEDMatrix->clear();
      char hour_min[5];
      sprintf(hour_min, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
      mLEDMatrix->drawBigString(hour_min, 5, 0, 0, 0);
      char sec[2];
      sprintf(sec, "%02d", timeinfo->tm_sec);
      mLEDMatrix->drawSmallChar(sec[0], 24, 0);
      mLEDMatrix->drawSmallChar(sec[1], 28, 0);
      mLEDMatrix->display();
#endif
   }
}

bool Clock::NTPCalibration(bool force)
{
   if (WiFiManager::Instance()->IsConnected() && (mNTPClient->update(force)))
   {
      // 时间更新成功，写入RTC
      unsigned long epoch = mNTPClient->getEpochTime();
      time_t t = epoch;
      struct tm *timeinfo = gmtime(&t);
      mktime(timeinfo);
      Serial.print("NTP日期时间：");
      Serial.print(timeinfo->tm_year + 1900);
      Serial.print("-");
      Serial.print(timeinfo->tm_mon + 1);
      Serial.print("-");
      Serial.print(timeinfo->tm_mday);
      Serial.print(" ");
      Serial.print(timeinfo->tm_hour);
      Serial.print(":");
      Serial.print(timeinfo->tm_min);
      Serial.print(":");
      Serial.println(timeinfo->tm_sec);
      mRTC.adjust(DateTime(timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
                           timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec));
      return true;
   }
   return false;
}

void Clock::StateEnter()
{
   switch (mCurState)
   {
   case ClockState::INIT_STATE:
   {
   }
   break;
   case ClockState::WIFI_CONFIG_STATE:
   {
      ShowAPConfig();
      WiFiManager::Instance()->SetState(WiFiManager::WiFiState::AP_SERVER_STATE);
   }
   break;
   case ClockState::TIME_DISPLAY_STATE:
   {
      NTPCalibration();
      DateTime now = mRTC.now();

      mLEDMatrix->clear();
      char hour_min[5];
      sprintf(hour_min, "%02d:%02d", now.hour(), now.minute());
      mLEDMatrix->drawBigString(hour_min, 5, 0, 0, 0);
      char sec[2];
      sprintf(sec, "%02d", now.second());
      mLEDMatrix->drawSmallChar(sec[0], 24, 0);
      mLEDMatrix->drawSmallChar(sec[1], 28, 0);
      mLEDMatrix->display();
      mLastUpdateTimeMS = millis();
   }
   break;
   case ClockState::NTPCAL_STATE:
   {
      ShowNTPCalibration();
      mNTPCaliResult = NTPCalibration(true);
      if (mNTPCaliResult)
      {
         ShowOK();
      }
      else
      {
         ShowNoOK();
      }
   }
   break;
   case ClockState::WIFI_CHECK_STATE:
   {
      mStartWiFiCheckMS = millis();
   }
   case ClockState::ERROR_STATE:
   {
   }
   break;
   default:
      break;
   }
}

void Clock::StateExit()
{
   switch (mCurState)
   {
   case ClockState::INIT_STATE:
   {
   }
   break;
   case ClockState::WIFI_CONFIG_STATE:
   {
   }
   break;
   case ClockState::TIME_DISPLAY_STATE:
   {
   }
   break;
   case ClockState::ERROR_STATE:
   {
   }
   break;
   case ClockState::WIFI_CHECK_STATE:
   {
   }
   break;
   default:
      break;
   }
}

void Clock::StateRun()
{
   switch (mCurState)
   {
   case ClockState::INIT_STATE:
   {
      WiFiManager::Instance()->Loop();
      WiFiManager::WiFiState state = WiFiManager::Instance()->GetState();
      switch (state)
      {
      case WiFiManager::WiFiState::STANDBY_STATE:
      {
         mNewState = ClockState::WIFI_CHECK_STATE;
      }
      break;
      case WiFiManager::WiFiState::CONNECTING_STATE:
      {
         ShowConnectingWiFi();
      }
      break;
      default:
         break;
      }
   }
   break;
   case ClockState::WIFI_CONFIG_STATE:
   {
      WiFiManager::Instance()->Loop();
      WiFiManager::WiFiState state = WiFiManager::Instance()->GetState();
      switch (state)
      {
      case WiFiManager::WiFiState::STANDBY_STATE:
      {
         mNewState = ClockState::WIFI_CHECK_STATE;
      }
      break;
      case WiFiManager::WiFiState::AP_SERVER_CONFIG_STATE:
      {
         ShowConnectingWiFi();
      }
      break;
      case WiFiManager::WiFiState::AP_SERVER_STATE:
      {
         ShowAPConfig();
      }
      default:
         break;
      }
   }
   break;
   case ClockState::NTPCAL_STATE:
   {
      if (millis() - mNTPCalLastMS > 1000)
      {
         mNewState = ClockState::TIME_DISPLAY_STATE;
      }
   }
   break;
   case ClockState::TIME_DISPLAY_STATE:
   {
      UpdateTime();
   }
   break;
   case ClockState::WIFI_CHECK_STATE:
   {
      WiFiManager::Instance()->Loop();
      WiFiManager::WiFiState state = WiFiManager::Instance()->GetState();
      switch (state)
      {
      case WiFiManager::WiFiState::STANDBY_STATE:
      {
         if ((millis() - mStartWiFiCheckMS) > 1000)
         {
            mNewState = ClockState::TIME_DISPLAY_STATE;
         }
         if (WiFiManager::Instance()->IsConnected())
         {
            ShowOK();
         }
         else
         {
            ShowNoOK();
         }
      }
      break;
      case WiFiManager::WiFiState::CONNECTING_STATE:
      {
         ShowConnectingWiFi();
      }
      break;
      default:
         break;
      }
   }
   case ClockState::ERROR_STATE:
   {
   }
   break;
   default:
      break;
   }
}

// for (int i = 0; i < 5; i++)
// {
//    delay(100);
// }
// for (int i = 0; i < 5; i++)
// {
//    matrix.scroll(LEDMatrixDriver::scrollDirection::scrollDown, 28, 2 + i, 3, 5 - i, 1);
//    matrix.display();
//    delay(100);
// }
// number++;
// if (number == 10)
// {
//    number = 0;
// }

// matrix.drawSmallChar(number + 0x30, 28, 0);
// matrix.display();

// matrix.drawBigString("11:13", strlen("11:13"), 0, 0);
// matrix.drawBigString("1", strlen("1"), 0, 0);
// matrix.drawBigChar('5', 0, 0);
// matrix.drawBigChar('4', 6, 0);
// matrix.drawBigChar('2', 12, 0);
// matrix.drawBigChar('1', 18, 0);

// matrix.drawBigString("15:56", strlen("15:56"), 0, 0, 0);
//  matrix.drawSmallChar('5', 24, 0);
//  matrix.drawSmallChar('5', 28, 0);
// matrix.drawSmallString("58", strlen("50"), 24, 0, 1);
// matrix.display();