#include <WiFi.h>
#include <WebServer.h>
#include "WiFiManager.h"
#include "Debug.h"
#include "WebServerPages.h"

static const char *s_APSSID = "Maxtrix LED Clock";
static IPAddress s_APIP(192, 168, 4, 1);
static IPAddress s_APGateway(192, 168, 4, 1);
static IPAddress s_APNetMask(255, 255, 255, 0);
static String s_WiFiHTMLOptions;
static WebServer s_WebServer;
static String s_WiFiSSID;
static String s_WiFiPass;

static bool StartAP(void);
static bool StartWebServer(void);
static void WebServerHandleRoot(void);
static void WebServerHandleNotFound(void);
static void WebServerHandleConfigWiFi(void);

bool WiFiManager_Init(void)
{
   bool result = StartAP();
   if (result)
   {
      result = StartWebServer();
   }
   return result;
}

void WiFiManager_Run(void)
{
   s_WebServer.handleClient();
}

bool WiFiManager_Scan(void)
{
   bool result = false;
   Debugln("开始扫描WiFi");
   int n = WiFi.scanNetworks();
   if (n)
   {
      Debug("扫描到");
      Debug(n);
      Debugln("个WIFI");
      s_WiFiHTMLOptions = "";
      for (size_t i = 0; i < n; i++)
      {
         int32_t rssi = WiFi.RSSI(i);
         String signalStrength;
         if (rssi >= -35)
         {
            signalStrength = " (信号极强)";
         }
         else if (rssi >= -50)
         {
            signalStrength = " (信号强)";
         }
         else if (rssi >= -70)
         {
            signalStrength = " (信号中)";
         }
         else
         {
            signalStrength = " (信号弱)";
         }
         // s_WiFiHTMLOptions += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + signalStrength + "</option>";
         s_WiFiHTMLOptions += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
         Debug("WiFi的名称(SSID):");
         Debugln(WiFi.SSID(i));
      }
      result = true;
   }
   else
   {
      Debugln("没有扫描到可用的WiFi");
   }
   return result;
}

static bool StartAP(void)
{
   Debugln("开启AP模式...");
   WiFi.mode(WIFI_MODE_APSTA);
   WiFi.softAPConfig(s_APIP, s_APGateway, s_APNetMask);
   if (!WiFi.softAP(s_APSSID))
   {
      Debugln("AP模式启动失败");
      return false;
   }
   else
   {
      Debugln("AP模式启动成功");
   }
   Debugln("AP模式启动成功");
   Debug("AP IP地址: ");
   Debugln(WiFi.softAPIP());
   return true;
}

static bool StartWebServer(void)
{
   // 当浏览器请求服务器根目录(网站首页)时调用自定义函数handleRoot处理，设置主页回调函数，必须添加第二个参数HTTP_GET，否则无法强制门户
   s_WebServer.on("/", HTTP_GET, WebServerHandleRoot);
   // 当浏览器请求服务器/configwifi(表单字段)目录时调用自定义函数handleConfigWifi处理
   s_WebServer.on("/configwifi", HTTP_POST, WebServerHandleConfigWiFi);
   // 当浏览器请求的网络资源无法在服务器找到时调用自定义函数handleNotFound处理
   s_WebServer.onNotFound(WebServerHandleNotFound);
   s_WebServer.begin();
   Debugln("服务器启动成功！");
   return true;
}

static void WebServerHandleRoot(void)
{
   s_WebServer.send(200, "text/html", g_WebPage1FirstPart + s_WiFiHTMLOptions + g_WebPage1SecondPart);
}

static void WebServerHandleNotFound(void)
{
   WebServerHandleRoot();
}

static void WebServerHandleConfigWiFi(void)
{
   // 判断是否有WiFi名称
   if (s_WebServer.hasArg("ssid"))
   {
      Debug("获得WiFi名称:");
      s_WiFiSSID = s_WebServer.arg("ssid");
      Debugln(s_WiFiSSID);
   }
   else
   {
      Debugln("错误, 没有发现WiFi名称");
      s_WebServer.send(200, "text/html", "<meta charset='UTF-8'>错误, 没有发现WiFi名称");
      return;
   }
   // 判断是否有WiFi密码
   if (s_WebServer.hasArg("password"))
   {
      Debug("获得WiFi密码:");
      s_WiFiPass = s_WebServer.arg("password");
      Debugln(s_WiFiPass);
   }
   else
   {
      Debugln("错误, 没有发现WiFi密码");
      s_WebServer.send(200, "text/html", "<meta charset='UTF-8'>错误, 没有发现WiFi密码");
      return;
   }

   // 将信息存入nvs中
   // 获得了所需要的一切信息，给客户端回复
   s_WebServer.send(200, "text/html", "<meta charset='UTF-8'><style type='text/css'>body {font-size: 2rem;}</style><br/><br/>WiFi: " + s_WiFiSSID + "<br/>密码: " + s_WiFiPass + "<br/>已取得相关信息,正在尝试连接,请手动关闭此页面。");
}