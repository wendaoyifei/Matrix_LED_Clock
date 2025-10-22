#ifndef WEB_SERVER_PAGES_H
#define WEB_SERVER_PAGES_H

#include <pgmspace.h>
#include <WString.h>

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

#endif // WEB_SERVER_PAGES_H
