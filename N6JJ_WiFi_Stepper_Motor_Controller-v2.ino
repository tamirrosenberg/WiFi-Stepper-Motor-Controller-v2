/*************************************************************************************************************************************
Project: Tuning the Magnetic loop capacitor remotely using Wifi
Details: This project is based on the ESP8266 Micro Controller, DRV8255 stepper motor driver and bipolar stepper motor.
I built a Magnetic Loop Antenna, and my idea was to control the tuning capacitor remotely, using Wifi.

DRV8825 - https://www.pololu.com/product/2133
---------------------------------------------
The DRV8825 is a smart motor driver that can be used with normal steps and even with micro-steps, and that can be controlled by
the driver's pins M0, M1, M2. In my case I wanted to use the 1/32 micro-steps, and these are the speeds that you will see in the code.
NOTE: Please follow the directions in the following video on how to set the correct Voltage/Current on the motor driver BEFORE using it:
https://www.youtube.com/watch?v=89BHS9hfSUk - Setting the Current Limit on Pololu Stepper Motor Driver Carriers

Arduino IDE - https://www.arduino.cc/en/software
------------------------------------------------
When you are using the Arduino IDE, you will probalby, need to install some libraries.
Please follow the procedures in the README file on how to do that: https://github.com/tamirrosenberg/WiFi-Stepper-Motor-Controller-v2

License
-------
Are you allowed to use this code anywhere? Or change the code? 
Yes, as long that it's for YOUR OWN PERSONAL USE (NOT FOR SELLING IT)!

Design: Tamir Rosenberg, N6JJ
YouTube Channel: https://www.youtube.com/@reHamRadio
Website: re:Ham Radio website: https://rehamradio.com/
Please help and support this channel with Patreon: https://patreon.com/rehamradio
**************************************************************************************************************************************/

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <DoubleResetDetect.h> // https://github.com/jenscski/DoubleResetDetect
#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP
#define WEBSERVER_H
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESP8266mDNS.h>
#include <Updater.h>
#define U_PART U_FS

//Constants
#define LED D4

// maximum number of seconds between resets that
// counts as a double reset 
#define DRD_TIMEOUT 2.0

// address to the block in the RTC user memory
// change it if it collides with another usage of the address block
#define DRD_ADDRESS 0x00

DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);

//Parameters
String AP_ID = "reHamRadio-AP";
String AP_PW = "rehamradio";
const char* host = "rehamradio";

//Variables
const int enablePin = 0;                            // ESP8266 digital pin 0
const int stepPin = 4;                              // ESP8266 digital pin 4
const int dirPin = 5;                               // ESP8266 digital pin 5
bool tuneStatus = false;
String sliderValue = "2000";
int sliderRange = 4400;
const char* PARAM_INPUT = "value";

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
<body style='background-color:#77a6f7; color:white;'>
    <title>WiFi Stepper Motor Controller</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin: 0px auto;
            padding-top: 30px;
        }
        .button {
            padding: 10px 20px;
            padding-left: 10%;
            padding-right: 10%;
            font-size: 24px;
            text-align: center;
            outline: none;
            color: #fff;
            background-color: #2f4468;
            border: none;
            border-radius: 5px;
            box-shadow: 0 6px #999;
            cursor: pointer;
            -webkit-touch-callout: none;
            -webkit-user-select: none;
            -khtml-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
        }
        .button:hover {
            background-color: #1f2e45
        }
        .button:active {
            background-color: #1f2e45;
            box-shadow: 0 4px #666;
            transform: translateY(2px);
        }
        .slider {
            -webkit-appearance: none;
            margin: 0 auto;
            border-radius: 25px;
            width: 60%;
            height: 25px;
            background: #FFD65C;
            outline: none;
            -webkit-transition: .2s;
            transition: opacity .2s;
        }
        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 65px;
            height: 35px;
            border-radius: 25%;
            background: #2f4468;
            cursor: pointer;
        }
        .slider::-webkit-slider-thumb:hover {
            background-color: #1f2e45
        }
        .slider::-moz-range-thumb {
            width: 65px;
            height: 35px;
            border-radius: 25%;
            background: #2f4468;
            cursor: pointer;
        }
        .slider::-moz-range-thumb:hover {
            background-color: #1f2e45
        }
    </style>
    </head>
    <body>
        <h2><center>WiFi Stepper Motor Controller</center></h2>
        <hr>
        <p id="tuneStatus">Connecting...</p>
        <p>slow<input type="range" direction="ltr" onchange="updateSpeedSlider(this)" id="speedSlider" min="0"
                max="4000" value="" step="1" class="slider">fast</p>
        <br>
        <button class="button" onmousedown="toggleCheckbox('scanToggle');" ontouchstart="toggleCheckbox('scanToggle');">
            SCAN </button>
        <br><br><br>
        <button class="button" onmousedown="toggleCheckbox('leftOn');" ontouchstart="toggleCheckbox('leftOn');"
            onmouseup="toggleCheckbox('leftOff');" ontouchend="toggleCheckbox('leftOff');"><<<<</button>      
        <button class="button" onmousedown="toggleCheckbox('rightOn');" ontouchstart="toggleCheckbox('rightOn');"
            onmouseup="toggleCheckbox('rightOff');" ontouchend="toggleCheckbox('rightOff');">>>>></button>
        <br><br><hr>
        <font size="1"><center>Created by N6JJ - Community Version (v0.3.1-beta) 2023</center></font>
        <font size="1"><center><a href='https://www.youtube.com/c/reHamRadio' target='_blank'>re:HamRadio on YouTube</a></center></font>
        <font size="1"><center><a href='https://rehamradio.com' target='_blank'>re:HamRadio Website</a></center></font>
        <font size="1"><center><a href='https://github.com/tamirrosenberg/WiFi-Stepper-Motor-Controller-v2' target='_blank'>The project on GitHub</a></center></font>
        <font size="1"><center><a href='http://rehamradio.local/update'>Firmware Update</a></center></font>
        <script>
            function toggleCheckbox(x) {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/" + x, true);
                xhr.send();
            }
            function updateSpeedSlider(element) {
                var sliderValue = document.getElementById("speedSlider").value;
                //document.getElementById("textSliderValue").innerHTML = sliderValue;
                console.log(sliderValue);
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/slider?value=" + sliderValue, true);
                xhr.send();
            }
            function checkAndUpdateStatus() {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("tuneStatus").innerHTML =
                            this.responseText;
                    }
                };
                xhttp.open("GET", "/tuneStatus", true);
                xhttp.send();
            }
            setInterval(checkAndUpdateStatus, 75);
    
        </script>
    </body>
</html>)rawliteral";

AsyncWebServer server(80);
size_t content_len;

void handleRoot(AsyncWebServerRequest *request) {
  request->redirect("/update");
}

void handleUpdate(AsyncWebServerRequest *request) {
  const char* html = "<body style='background-color:#77a6f7; color:white; align=left; font-family: Arial;'><h3>Firmware Update:</h3><form method='POST' action='/fwUpdate' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form><font size='2'><a href='http://rehamradio.local'>Back</a></font></body><hr><font size='1'><center>Created by N6JJ - Community Version (v0.3.1-beta) 2023</center></font>";
  request->send(200, "text/html", html);
}

void handleFWUpdate(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
  if (!index){
    Serial.println("Updating firmware...");
    content_len = request->contentLength();
    // if filename includes spiffs, update the spiffs partition
    int cmd = (filename.indexOf("spiffs") > -1) ? U_PART : U_FLASH;

    Update.runAsync(true);
    if (!Update.begin(content_len, cmd)) {
      Update.printError(Serial);
    }
    request->send(200, "text/html", "<head><meta http-equiv='refresh' content='10;URL=/'/></head><body style='background-color:#77a6f7; color:white; align=left; font-family: Arial;'>Uploaded! Rebooting, please wait...</body>");
  }

  if (Update.write(data, len) != len) {
    Update.printError(Serial);
    Serial.printf("Progress: %d%%\n", (Update.progress()*100)/Update.size());
  }

  if (final) {
    if (!Update.end(true)){
      Update.printError(Serial);
    } else {
      Serial.println("Firmware updated!");
      Serial.flush();
      ESP.restart();
    }
  }
}

void printProgress(size_t prg, size_t sz) {
  Serial.printf("Progress: %d%%\n", (prg*100)/content_len);
}

boolean fwPage() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {request->redirect("/update");});
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){handleUpdate(request);});
  server.on("/fwUpdate", HTTP_POST,
    [](AsyncWebServerRequest *request) {},
    [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
                  size_t len, bool final) {handleFWUpdate(request, filename, index, data, len, final);}
  );
  server.onNotFound([](AsyncWebServerRequest *request){request->send(404);});
  server.begin();
  return fwPage;
}

void setup() {
  //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System..."));

  // WiFiManager
  WiFiManager wm;
   
  bool res;
  res = wm.autoConnect(AP_ID.c_str(),AP_PW.c_str()); // password protected access point
  
  if (drd.detect())
     {
         Serial.println("** Double reset boot **");
         WiFi.disconnect();
         wm.startConfigPortal(AP_ID.c_str(),AP_PW.c_str());
         delay(1000);
         ESP.restart();
     }
  
  if(!res) {
     Serial.println("Failed to connect");
  } 
  else {    
     Serial.println("Connected!");
  }
  
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);                           // step pin
  pinMode(dirPin, OUTPUT);                            // direction pin
  pinMode(enablePin, OUTPUT);                         // enable pin
  pinMode(LED, OUTPUT);                               // onboard LED indicator
  digitalWrite(enablePin, HIGH);                      // start with driver disable
  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html",index_html);
  });

  // Button Left On
  server.on("/leftOn", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("Left On");
    digitalWrite(dirPin, HIGH);                         // set direction to one side
    tuneStatus = true;
    request->send(200, "text/plain", "ok");
  });

  // Button Left Off
  server.on("/leftOff", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("Left Off");
    tuneStatus = false;
    request->send(200, "text/plain", "ok");
  });

  // Button Right On
  server.on("/rightOn", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("Right On");
    digitalWrite(dirPin, LOW);                         // set direction to one side
    tuneStatus = true;
    request->send(200, "text/plain", "ok");
  });

  // Button Right Off
  server.on("/rightOff", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("Right Off");
    tuneStatus = false;
    request->send(200, "text/plain", "ok");
  });

  // Button Scan Toggle
  server.on("/scanToggle", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("Scan Toggle");
    if (tuneStatus == false) {
      tuneStatus = true;
    }
    else {
      tuneStatus = false;
    }
    Serial.println({tuneStatus});
    request->send(200, "text/plain", "ok");
  });

  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.on("/tuneStatus", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String statusValue = "Idle";
    if(tuneStatus){
      statusValue = "Tuning...";
    }
    request->send(200, "text/plain", statusValue);
  });
  
  MDNS.begin(host);
  server.onNotFound(notFound);
  server.begin();
  fwPage();
  MDNS.addService("http", "tcp", 80);
  Serial.printf("The application is ready at http://%s.local\n", host);
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Hmm... something is wrong... :(");
}

void loop() {
  // -- Main Tuning Function --
  if (tuneStatus) {
    digitalWrite(LED, LOW);                           // turn LED indicator OFF
    digitalWrite(enablePin, LOW);
    digitalWrite(stepPin, HIGH);                           // perform a step
    delayMicroseconds(sliderRange-sliderValue.toInt());    // wait for the steps operation
    digitalWrite(stepPin, LOW);                            // stop the step 
    delayMicroseconds(sliderRange-sliderValue.toInt());    // wait between steps
  }
  else {
    digitalWrite(LED, HIGH);
    digitalWrite(enablePin, HIGH);
  }
  MDNS.update();
}
