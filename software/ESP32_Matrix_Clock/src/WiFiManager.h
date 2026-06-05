#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <IPAddress.h>

class Clock;

class WiFiManager
{
public:
   static WiFiManager *Instance();

   enum class WiFiState
   {
      INIT_STATE,             // 初始化状态
      AP_SERVER_STATE,        // ap server
      AP_SERVER_CONFIG_STATE, // ap server 处理config
      CONNECTING_STATE,       // WIFI连接中状态
      STANDBY_STATE,          // 待命状态
   };

   // 获取WiFi连接状态
   bool IsConnected();
   // 循环处理
   void Loop();
   // 设置状态
   void SetState(WiFiState state);
   // 获取状态
   WiFiState GetState()
   {
      return mCurState;
   }

private:
   // 初始化WiFi，从Flash加载WiFi配置
   void Init(const String &ApSsid = "Matrix_LED_Clock", const String &ApPwd = "");
   // 扫描附近WiFi，查找是否有指定的WiFi
   bool Scan(const String &ssid);
   // 连接指定WiFi
   void Connect(String ssid, String pwd);
   // 断开WiFi
   void Disconnect();
   // 保存WiFi到Flash
   void Store(String ssid, String pwd);
   // 从Flash加载WiFi
   bool Load();
   // 创建热点，并开启Web服务器
   void CreateAP(const String &ApSsid, const String &ApPwd);

   // 处理每个状态的enter
   void StateEnter();
   // 处理每个状态exit
   void StateExit();
   // 处理每个状态的run
   void StateRun();

   void HandleIndex();
   void HandleConfigWiFi();
   void HandleScanWiFi();

   WiFiManager();
   ~WiFiManager();

private:
   Preferences mStore;
   String mSTASSID;
   String mSTAPWD;
   String mAPSSID;
   String mAPPWD;
   WebServer mWebServer;

   WiFiState mPreState = WiFiState::INIT_STATE;
   WiFiState mCurState = WiFiState::INIT_STATE;
   WiFiState mNewState = WiFiState::INIT_STATE;

   uint64_t mStartConnectMS = 0;
   uint64_t mStartCloseAPMS = 0;
   uint64_t mStartWiFiCheckMS = 0;

   uint32_t mConnectTimeout = 10000;
};

#endif // WIFI_MANAGER_H