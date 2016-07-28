##WiFiManagerGUI
An ESP8266 / OLED display project for connecting to an Access Point using a GUI.

##Background
There are numerous articles, projects and libraries that set the ESP8266 into Access Point mode allowing you to connect and configure an SSID and password before restarting the device in Station mode.  These are great but what if you actually have a display attached to the device?  Why can't the device attempt to connect to previously saved settings and if this fails, allow the user to select an Access Point and enter a pasword using a GUI?

I have used a number of screens on the Arduino but wanted this project to be compatible with the small 128x64 pixel OLED displays that have become very popular and cheap to buy.

The inspiration for this project came from two separate GitHub projects :

* @tzapu for his amazing [WifiManager](https://github.com/tzapu/WiFiManager) project and 
* @squix78 for his [esp8266-oled-ssd1306](https://github.com/squix78/esp8266-oled-ssd1306) library for driving these little OLED displays.

##In Operation

Select an Access Point
![Select an Access Point](https://github.com/filmote/WiFiManagerGUI/blob/master/images/WiFiManagerGUI_1_sml.jpg)
![Enter a Password](https://github.com/filmote/WiFiManagerGUI/blob/master/images/WiFiManagerGUI_2_sml.jpg)
![Connect](https://github.com/filmote/WiFiManagerGUI/blob/master/images/WiFiManagerGUI_3_sml.jpg)

##Confessions 
This is my first GitHub project.  Go easy on me.