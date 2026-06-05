#ifndef WEB_SERVER_PAGES_H
#define WEB_SERVER_PAGES_H

#include <pgmspace.h>
#include <WString.h>
const String gWebServerPage PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='en'>

<head>
   <meta charset='UTF-8'>
   <meta name='viewport'
      content='width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=no'>
   <title>点阵时钟配置WIFI</title>
   <style>
      * {
         margin: 0;
         padding: 0
      }

      body {
         font-size: 16px;
         margin: 0 15px;
         z-index: 0
      }

      .flexcenter {
         display: flex;
         align-content: center;
         align-items: center
      }

      .button {
         width: 40%;
         border-radius: 5px;
         height: 40px;
         background: none;
         border: 1px solid green;
         font-size: 20px;
         justify-content: space-evenly;
         color: green
      }

      .button:active {
         background: #ccc
      }

      .wifititle {
         display: flex;
         margin: 0 15px;
         justify-content: space-between;
         font-size: 22px;
         color: #333
      }

      .wifilist {
         display: flex;
         flex-direction: column;
         align-items: center;
         justify-content: space-between;
         font-size: 25px;
         color: #ccc
      }

      divw {
         height: 30px;
         border-bottom: 1px solid#ccc;
         width: 100%;
         background: none;
         margin-top: 5px
      }

      divw:active {
         background: #50bfff
      }

      .title {
         justify-content: center;
         height: 10%;
         font-size: 35px;
         color: #725af3;
         letter-spacing: 5px;
         text-shadow: 0px 12px 10px#9d91bf
      }

      .content-input {
         height: 20%;
         max-height: 20%;
         display: flex;
         flex-direction: column
      }

      .input-text {
         display: flex;
         flex-direction: row;
         align-items: flex-end;
         justify-content: space-evenly;
         font-size: 20px;
         color: #ccc
      }

      .input {
         width: 76%;
         line-height: 30px;
         font-size: 25px;
         margin-top: 15px;
         border-top: 0;
         border-left: 0;
         border-right: 0;
         border-bottom: 1px solid#ccc
      }

      .context-button {
         margin-top: 15px;
         margin-bottom: 15px;
         justify-content: space-around
      }

      .loading-svg-container {
         width: 50px;
         height: 50px
      }

      .path {
         stroke: #409eff;
         stroke-width: 2;
         stroke-dasharray: 95, 126;
         stroke-dashoffset: 0;
         animation: loading-dash 1.5s ease-in-out infinite
      }

      @keyframes loading-dash {
         0% {
            stroke-dasharray: 1, 126;
            stroke-dashoffset: 0
         }

         50% {
            stroke-dasharray: 95, 126;
            stroke-dashoffset: -31px
         }

         to {
            stroke-dasharray: 6, 120;
            stroke-dashoffset: -120px
         }
      }

      .loading {
         width: 40%;
         height: 15%;
         position: absolute;
         background: #fff;
         display: flex;
         align-items: center;
         justify-content: space-around;
         top: 30%;
         left: 30%;
         flex-direction: column;
         color: #F4606C;
         border-radius: 20px;
         overflow: hidden;
         font-size: 14px;
         z-index: 999
      }

      .box {
         width: 100%;
         height: 100%;
         position: fixed;
         background: #cccccc;
         opacity: 0.8;
         margin-left: -15px;
         display: none
      }
   </style>
</head>

<body onload='initData()'>
   <div class='box'>
      <div class='loading'><svg viewBox='0 0 50 50' class='loading-svg-container'>
            <circle cx='25' cy='25' r='20' fill='none' class='path'></circle>
         </svg><span id='tip'></span></div>
   </div>
   <div class='flexcenter title'>欢迎使用点阵时钟</div>
   <div class='content-input'>
      <div class='input-text'>SSID：<input class='input' id='ssid' type='text' value='' /></div>
      <div class='input-text'>密码：<input class='input' id='pwd' type='password' value='' /></div>
   </div>
   <div class='flexcenter context-button'><button class='flexcenter button'
         onclick='connwifi()'>提&nbsp;&nbsp;交</button><button class='flexcenter button'
         onclick='reset()'>重&nbsp;&nbsp;置</button></div>
   <div class='wifititle'><span>wifi列表</span><span style='color: blue' onclick='getWifiList()'>刷新</span></div>
   <div class='wifilist' id='wifilist'></div>
</body>
<script>
   function initData() {
      setTimeout(function () { getWifiList() }, 1000)
   }
   function showloading(msg) {
      document.getElementsByClassName('box')[0].style.display = 'table';
      document.getElementById('tip').innerHTML = msg
   }
   function hideloading() {
      document.getElementsByClassName('box')[0].style.display = 'none';
      document.getElementById('tip').innerHTML = ''
   }
   function getWifiList() {
      document.getElementById('tip').innerText = '';
      showloading('搜索wifi中...');
      doget('/wifilist', function (result) {
         if (result.indexOf('.nodata') !== -1) {
            showloading('找不到有效wifi信息');
            setTimeout(function () { hideloading() }, 2000);
            return
         }
         hideloading();
         var arr = result.split(',');
         var html = '';
         for (item of arr) {
            if (item === '') {
               continue
            }
            html += '<divw onclick=\"setSsid(event)\">' + item + '</divw>'
         }
         document.getElementById('wifilist').innerHTML = html
      })
   }
   function setSsid(event) {
      document.getElementById('tip').innerText = '';
      document.getElementById('ssid').value = event.currentTarget.innerText
   }
   function reset() {
      document.getElementById('tip').innerText = '';
      document.getElementById('ssid').value = '';
      document.getElementById('pwd').value = ''
   }
   function connwifi() {
      var ssid = document.getElementById('ssid').value;
      if (ssid === '' || ssid === null || typeof (ssid) === 'undefined') {
         showloading('SSID不能为空'); 
         setTimeout(function () { hideloading() }, 1000); 
         return
      }
      var pwd = document.getElementById('pwd').value;
      var url = '/configwifi?ssid=' + ssid + '&pwd=' + pwd;
      showloading('wifi连接中...');
      doget(url, function (result) {
         showloading(result);
         setTimeout(function () { hideloading() }, 2000)
      })
   }
   function doget(url, callback) {
      var ajax = new XMLHttpRequest();
      ajax.open('get', url);
      ajax.send();
      ajax.onreadystatechange = function () { callback(ajax.response) }
   }
</script>
</html>

)rawliteral";
#if 0
const String g_WebPage1FirstPart PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
   <meta charset="UTF-8">
   <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <meta http-equiv="content-type" content="text/html; charset=UTF-8" />
   <title>矩阵时钟配网</title>
   <style>
      #content,
      .input_card a,
      .input_card h1 {
         text-align: center
      }

      body,
      html {
         margin: 0;
         padding: 0;
         width: 100%;
         height: 100%;
         display: table
      }

      #content {
         font-family: 'Source Sans Pro', sans-serif;
         /* -webkit-background-size: cover;
         -moz-background-size: cover;
         -o-background-size: cover; */
         background-size: cover;
         display: table-cell;
         vertical-align: middle
      }

      .input_card {
         padding: 40px;
         width: 274px;
         background-color: #F7F7F7;
         margin: 0 auto 10px;
         border-radius: 20px;
         box-shadow: 8px 8px 15px rgba(0, 0, 0, .3);
         overflow: hidden
      }

      .input_card h1 {
         font-weight: 400;
         font-size: 2.3em;
         color: #1383c6
      }

      .input_card h1 span {
         color: #f26721
      }

      .input_card img {
         width: 70%;
         height: 70%
      }

      .input_card input[type=submit] {
         width: 100%;
         display: block;
         margin-bottom: 10px;
         position: relative
      }

      .input_card input[type=text],
      input[type=password] {
         height: 44px;
         font-size: 16px;
         width: 100%;
         margin-bottom: 10px;
         /* -webkit-appearance: none; */
         background: #fff;
         border: 1px solid #d9d9d9;
         border-top: 1px solid silver;
         border-radius: 10px;
         padding: 0 8px;
         box-sizing: border-box;
         /* -moz-box-sizing: border-box */
      }

      .input_card input[type=text]:hover,
      input[type=password]:hover {
         border: 1px solid #b9b9b9;
         border-top: 1px solid #a0a0a0;
         /* -moz-box-shadow: inset 0 1px 2px rgba(0, 0, 0, .1);
         -webkit-box-shadow: inset 0 1px 2px rgba(0, 0, 0, .1); */
         box-shadow: inset 0 1px 2px rgba(0, 0, 0, .1)
      }

      .input_submit {
         font-size: 14px;
         font-family: Arial, sans-serif;
         font-weight: 700;
         height: 36px;
         padding: 0 8px
      }

      .input_submit {
         /* -webkit-appearance: none;
         -moz-appearance: none; */
         appearance: none;
         border: 0;
         border-radius : 15px; 
         color: #fff;
         text-shadow: 0 1px rgba(0, 0, 0, .1);
         background-color: #4d90fe
      }

      .input_submit:disabled {
         opacity: .6
      }

      .input_submit:hover {
         border: 0;
         text-shadow: 0 1px rgba(0, 0, 0, .3);
         background-color: #357ae8
      }

      .input_card a {
         text-decoration: none;
         color: #666;
         font-weight: 400;
         display: inline-block;
         opacity: .6;
         transition: opacity ease .5s
      }

      .input_card a:hover {
         opacity: 1
      }

   </style>
</head>

<body style="background-color: #e5e9f2">
   <div id="content">
      <form name='input' action='/configwifi' method='POST' onsubmit="doSubmit(event)">
         <div class="input_card">
            <h1>WiFi配置</h1>
            <input type="text" name="ssid" placeholder="请输入 WiFi 名称" id="ssid" list="data-list" ;>
            <datalist id="data-list">
)rawliteral";

const String g_WebPage1SecondPart PROGMEM = R"rawliteral(
</datalist>
            <input type="password" name="password" placeholder="请输入 WiFi 密码" id="password" ;>
            <input type="submit" class="input_submit" value="确 定 提 交" id="login" ; disabled;>
         </div>
      </form>
   </div>
   <script type="text/javascript">
      function doSubmit(event) {
         var ssidInput = document.getElementById("ssid").value;
         if (ssidInput == "") {
            event.preventDefault();
            alert("请选择WiFi！");
            return;
         }
         var passwordInput = document.getElementById("password").value;
         if (passwordInput == "") {
            event.preventDefault();
            alert("请输入WiFi密码！");
            return;
         }
      }
   </script>
</body>

</html>

)rawliteral";

#endif
#endif // WEB_SERVER_PAGES_H
