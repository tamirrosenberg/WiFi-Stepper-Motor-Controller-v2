# WiFi-Stepper-Motor-Controller-v2
Control a Stepper Motor remotely using WiFI (ESP8266/DRV8825)

## NOTE: In this project, you will find a new WiFi Stepper Motor Controller App for Magnetic Loop Antenna, that is replacing the old Blynk App!!! (as Blynk upgraded to v2 and removed v1 from the AppStore and the PlayStore). This project is replacing the old project that is now DEPRECATED: https://github.com/tamirrosenberg/WiFi-Stepper-Motor-Controller/edit/main/README.md

## Please help and support this project and the YoutTube channel with Pateron: https://patreon.com/rehamradio
## Thank You!

About the project
-----------------
Tuning the Magnetic loop antenna, by controling the capacitor remotely, using Wifi. 
I built a Magnetic Loop Antenna, and my idea was to control the tuning capacitor remotely.
And it's working great! :)

For more details, please visit the re:Ham Radio website: https://rehamradio.com/  
Visit my YouTube channel to see this project in action:  
https://www.youtube.com/@reHamRadio - The YouTube Channel  
https://youtu.be/9yk-2Kybxgk - New WiFi Stepper Motor Controller App for Magnetic Loop Antenna (Blynk replacement!)  
https://youtu.be/Orhog4gqOqk - Build a Magnetic Loop Antenna with Remote Wireless Tuning - Practical Guidance  
https://youtu.be/6eGOkw7WbA4 - The original project with the Blynk App - DEPRECATED!!!


This project is based on:
-------------------------
* ESP8266 Micro Controller
* DRV8255 Stepper Motor Driver
* Bipolar stepper motor (Nema 17)
  
  
DRV8825 - Motor Driver
----------------------
The DRV8825 is a smart motor driver that can be used with normal steps and even with micro-steps, and that can be controlled by
the driver's pins M0, M1, M2. In my case I wanted to use the 1/32 micro-steps, and these are the speeds that you will see in the code.
More details about this driver can be found here: https://www.pololu.com/product/2133.  
NOTE: Please follow the directions in the following video on how to set the correct Voltage/Current on the motor driver before using it:
https://www.youtube.com/watch?v=89BHS9hfSUk - Setting the Current Limit on Pololu Stepper Motor Driver Carriers

Arduino IDE
-----------
The Arduino IDE can be download from: https://www.arduino.cc/en/software.  
When you are using the Arduino IDE, you will probalby need to install the esp8266 libraries, so please follow this procedure:
1. Go to the "File" --> "Preferences"
2. In the "Additional Boards Manager URLs:" insert "http://arduino.esp8266.com/stable/package_esp8266com_index.json"
3. Confirm with "Ok"

Now, let's install and choose the board. From the Arduino IDE menu:
1. Go to "Tools" --> "Boards" --> "Boards Manager"
2. Search for board "8266", and install it
3. Go to "Tools" --> "Boards" --> "esp8266" and choose "NodeMCU 1.0 (ESP-12E Module)"

Now, we need to install few libraries that are in use and needed in the code, it's easy.  
Please go to each of these links, and download the files to your computer by using the Green button called "Code" --> "Download ZIP":
1. https://github.com/tzapu/WiFiManager
2. https://github.com/jenscski/DoubleResetDetect
3. https://github.com/me-no-dev/ESPAsyncTCP
4. https://github.com/me-no-dev/ESPAsyncWebServer

Once you have all these files locally, go back to the Arduino IDE:
1. Go to "Sketch" --> "Include Library" --> "Add .ZIP Library..."
2. Add each of the files above, one by one
3. I told you it's easy ;)

Now you are ready to install the code into the esp8266 micro-controller:
1. Copy & Paste the code into the main code screen of the Arduino IDE  
2. Click "Upload" (An icon of an arrow)

You just finished to installed the app and it's ready for use! YAY!!!


License
-------
Are you allowed to use this code anywhere? Or change the code?
Yes! You are free to do that as long that it's for YOUR OWN PERSONAL USE (NOT FOR SELLING IT)!
Please feel free to change that to your own project use case.


Design: Tamir Rosenberg, N6JJ  
YouTube Channel Name: re: Ham Radio  
YouTube Channel link: https://www.youtube.com/@reHamRadio

And again, please consider to help and support this project and the YouTube channel with Pateron:  
https://patreon.com/rehamradio

## Thank You!
