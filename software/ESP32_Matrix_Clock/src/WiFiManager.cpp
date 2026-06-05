

#include "WiFiManager.h"
#include "WebServerPages.h"
#include "Clock.h"

WiFiManager *WiFiManager::Instance()
{
   static WiFiManager sInstance;
   return &sInstance;
}

bool WiFiManager::IsConnected()
{
   return (WiFi.status() == WL_CONNECTED) ? true : false;
}

void WiFiManager::Loop()
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

void WiFiManager::SetState(WiFiState state)
{
   if (state != WiFiState::INIT_STATE)
   {
      mNewState = state;
   }
}

void WiFiManager::Init(const String &ApSsid, const String &ApPwd)
{
   mAPSSID = ApSsid;
   mAPPWD = ApPwd;
   if (Load())
   {
      Serial.print("Found ");
      Serial.println(mSTASSID);
      mNewState = WiFiState::CONNECTING_STATE;
   }
   else
   {
      mNewState = WiFiState::STANDBY_STATE;
   }
}

bool WiFiManager::Scan(const String &ssid)
{
   bool Found = false;
   Serial.println("开始扫描WiFi...");
   int n = WiFi.scanNetworks();
   if (n <= 0)
   {
      Serial.println("未扫描到WiFi网络");
   }
   else
   {
      Serial.print("扫描到 ");
      Serial.print(n);
      Serial.println(" 个WiFi：");
      for (int i = 0; i < n; i++)
      {
         Serial.print(i + 1);
         Serial.print(": ");
         Serial.print(WiFi.SSID(i));
         Serial.print(" | 信号强度：");
         Serial.print(WiFi.RSSI(i));
         Serial.print("dBm | 加密：");
         Serial.println(WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "开放" : "加密");
         if (WiFi.SSID(i) == ssid)
         {
            Found = true;
         }
      }
   }
   WiFi.scanDelete();
   return Found;
}

void WiFiManager::Connect(String ssid, String pwd)
{
   WiFi.mode(WIFI_STA);
   WiFi.setAutoConnect(true);
   WiFi.setAutoReconnect(true);

   Serial.print("连接WiFi：");
   Serial.print(ssid);
   Serial.print(",");
   Serial.println(pwd);
   WiFi.begin(ssid.c_str(), pwd.c_str());
   mCurState = WiFiState::CONNECTING_STATE;
}

void WiFiManager::Disconnect()
{
   if (WiFi.status() == WL_CONNECTED)
   {
      WiFi.disconnect();
   }
}

WiFiManager::WiFiManager()
{
}

WiFiManager::~WiFiManager()
{
}

void WiFiManager::Store(String ssid, String pwd)
{
   mStore.begin("WiFi", false);
   mStore.putString("ssid", ssid);
   mStore.putString("pwd", pwd);
   mStore.end();
   Serial.println("WiFi信息已保存到Flash");
}

bool WiFiManager::Load()
{
   mStore.begin("WiFi", false);
   String ssid = mStore.getString("ssid", "");
   String pwd = mStore.getString("pwd", "");
   mStore.end();
   if (ssid.isEmpty() || pwd.isEmpty())
   {
      Serial.println("Load WiFi信息失败");
      return false;
   }
   else
   {
      mSTASSID = ssid;
      mSTAPWD = pwd;
      Serial.println("Load WiFi信息成功");
      Serial.print(mSTASSID);
      Serial.print(",");
      Serial.println(mSTAPWD);
      // Load成功后判断当前是否有效
      return (Scan(mSTASSID));
   }
}

void WiFiManager::CreateAP(const String &ApSsid, const String &ApPwd)
{
   // 先断开连接，避免之前是STA连接中的状态
   WiFi.disconnect(true);
   IPAddress APIP(192, 168, 8, 1);
   WiFi.mode(WIFI_AP_STA);
   WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
   WiFi.softAP(ApSsid);

   Serial.print("热点已创建：");
   Serial.println(ApSsid);
   Serial.print("IP:");
   Serial.println(APIP);

   mWebServer.on("/", HTTP_GET, [this]()
                 { HandleIndex(); }); // 设置主页回调函数
   mWebServer.on("/configwifi", HTTP_GET, [this]()
                 { HandleConfigWiFi(); }); // 设置配置WiFi的回调函数
   mWebServer.on("/wifilist", HTTP_GET, [this]()
                 { HandleScanWiFi(); }); // 设置刷新请求的回调函数
   mWebServer.onNotFound([this]()
                         { HandleIndex(); }); // 设置无法响应的回调函数
   mWebServer.begin();                        // 启动WebServer
   Serial.println("WebServer已启动");

   mCurState = WiFiState::AP_SERVER_STATE;
}

void WiFiManager::StateEnter()
{
   switch (mCurState)
   {
   case WiFiState::INIT_STATE:
   {
   }
   break;
   case WiFiState::AP_SERVER_STATE:
   {
      if (mPreState != WiFiState::AP_SERVER_CONFIG_STATE)
      {
         CreateAP(mAPSSID, mAPPWD);
      }
   }
   break;
   case WiFiState::AP_SERVER_CONFIG_STATE:
   {
      Serial.println("Start connect");
      mStartConnectMS = millis();
   }
   break;
   case WiFiState::CONNECTING_STATE:
   {
      Connect(mSTASSID, mSTAPWD);
      mStartConnectMS = millis();
   }
   break;
   default:
      break;
   }
}

void WiFiManager::StateExit()
{
   switch (mCurState)
   {
   case WiFiState::AP_SERVER_STATE:
   {
   }
   break;
   case WiFiState::AP_SERVER_CONFIG_STATE:
   {
   }
   break;
   case WiFiState::CONNECTING_STATE:
   {
   }
   break;
   case WiFiState::STANDBY_STATE:
   {
   }
   break;
   default:
      break;
   }
}

void WiFiManager::StateRun()
{
   switch (mCurState)
   {
   case WiFiState::INIT_STATE:
   {
      Init();
   }
   break;
   case WiFiState::AP_SERVER_STATE:
   {
      mWebServer.handleClient();
   }
   break;
   case WiFiState::AP_SERVER_CONFIG_STATE:
   {
      mWebServer.handleClient();
      if (IsConnected())
      {
         Serial.print("WIFI Connected:");
         Serial.println(WiFi.localIP());
         mStartCloseAPMS = millis();
         // mWebServer.send(200, "text/html", "WiFi连接成功");
         Store(mSTASSID, mSTAPWD);
         mNewState = WiFiState::STANDBY_STATE;
      }
      else if ((millis() - mStartConnectMS) > mConnectTimeout)
      {
         Serial.println("Connect failed");
         // mWebServer.send(200, "text/html", "WiFi连接失败,请检查密码后重试");
         mNewState = WiFiState::AP_SERVER_STATE;
         WiFi.disconnect();
      }
   }
   break;
   case WiFiState::CONNECTING_STATE:
   {
      // 执行连接是否成功的检查
      if (IsConnected())
      {
         Serial.println("连接成功");
         mNewState = WiFiState::STANDBY_STATE;
      }
      else if ((millis() - mStartConnectMS) > mConnectTimeout)
      {
         WiFi.disconnect(true);
         mNewState = WiFiState::STANDBY_STATE;
      }
   }
   break;
   case WiFiState::STANDBY_STATE:
   {
      if (WiFi.getMode() == WIFI_AP_STA)
      {
         // 如果前一个状态是AP模式，则需要等待1秒后关闭热点
         if ((millis() - mStartCloseAPMS) > 1000)
         {
            Serial.println("关闭AP热点");
            WiFi.softAPdisconnect(true);
            WiFi.mode(WIFI_STA);
            mWebServer.close();
         }
         else
         {
            mWebServer.handleClient();
         }
      }
   }
   default:
      break;
   }
}

void WiFiManager::HandleIndex()
{
   mWebServer.send(200, "text/html", gWebServerPage);
}

void WiFiManager::HandleConfigWiFi()
{
   mSTASSID = mWebServer.arg("ssid").c_str();
   mSTAPWD = mWebServer.arg("pwd");
   Serial.print(mSTASSID);
   Serial.print(",");
   Serial.println(mSTAPWD);
   WiFi.begin(mWebServer.arg("ssid").c_str(), mWebServer.arg("pwd").c_str(), 1);
   mNewState = WiFiState::AP_SERVER_CONFIG_STATE;
}

void WiFiManager::HandleScanWiFi()
{
   int n = WiFi.scanNetworks();
   if (n > 0)
   {
      char wifilist[640] = {0};
      Serial.println("sacn wifi.");
      for (int i = 0; i < 20; ++i)
      {
         sprintf(wifilist, "%s%s%s", wifilist, WiFi.SSID(i).c_str(), ",");
      }
      Serial.print(wifilist);
      mWebServer.send(200, "text/html", wifilist);
   }
   else
   {
      Serial.println("no any wifi.");
      mWebServer.send(200, "text/html", ".nodata");
   }
   WiFi.scanDelete();
}
