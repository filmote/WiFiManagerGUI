##WiFiManagerGUI
An ESP8266 / OLED display project for connecting to an Access Point using a GUI.  
<br /><br />

##Background
There are numerous articles, projects and libraries that set the ESP8266 into Access Point mode allowing you to connect and configure an SSID and password before restarting the device in Station mode.  These are great but what if you actually have a display attached to the device?  Why can't the device attempt to connect to previously saved settings and if this fails, allow the user to select an Access Point and enter a pasword using a GUI?

I have used a number of screens on the Arduino but wanted this project to be compatible with the small 128x64 pixel OLED displays that have become very popular and cheap to buy.  I also wanted to keep the input as simple as possible and settled on 5 buttons - 4 for movement (up, down, left and right) and one for selecting options. 

The inspiration for this project came from two separate GitHub projects :

* @tzapu for his amazing [WifiManager](https://github.com/tzapu/WiFiManager) project and 
* @squix78 for his [esp8266-oled-ssd1306](https://github.com/squix78/esp8266-oled-ssd1306) library for driving these little OLED displays.  
<br /><br />

##Dependencies

* [esp8266-oled-ssd1306](https://github.com/squix78/esp8266-oled-ssd1306) library ** Compatible with version 2.x only. ** An issue in version 3.x prevents the black text from being rendered on a white background.  This has been tracked as a bug in the latest version and will be fixed soon (hopefully!).  This project will operate with version 3.x however the rendering of the highlights suffers.

##In Operation

When the ESP8266 first starts it will attempt to connect to any existing SSID stored in the EEPROM using the credentials it retrieves.  If this connection fails, it will then prompt the user to select an Access Point, enter a password and then connect to the WiFi.  If a successful connection is made, control passes to your application otherwise the user is prompted to confirm the Access Point and / or password. 

Select an Access Point ..

![Select an Access Point](https://github.com/filmote/WiFiManagerGUI/blob/master/images/WiFiManagerGUI_1_sml.jpg)

Enter a Password ..

![Enter a Password](https://github.com/filmote/WiFiManagerGUI/blob/master/images/WiFiManagerGUI_2_sml.jpg)

Hopefully your connection will succeed, otherwise you may see this message ..

![Connect](https://github.com/filmote/WiFiManagerGUI/blob/master/images/WiFiManagerGUI_3_sml.jpg)  

<br /><br />

##My Prototype

I built my prototype using some bits and pieces following the schematic shown below.  The five buttons required to control the screen are connected to the ESP8266 via the ADC input freeing up the digital I/O pins for other tasks.  Resistor 5 is included to form a voltage divider and split the 3.3V into (approximately) 2.3V and 1.0V as the ADC will return a value between 0 and 1023 for voltage inputs between 0V and 1.0V.   Before you ask, I used the resistors I had available and they do not produce a nice spread of values.  One of my tasks is to work out the correct values and alter the constants in the application accordingly.

![Schematic](https://github.com/filmote/WiFiManagerGUI/blob/master/images/Schematic.png)  
<br /><br />

##About the Code

* the code is thoroughly documented the code - well as thoroughly as any developer does
* all constants have been pulled out into #define values where appropriate
* the code details a well defined state machine and shows how to control movement through the various connection screens and then into your own application (refer details in the code itself)
* the code includes detailed debugging which can be turned on or off in its entirety or in certain parts of the code only via a hierarchy #define structure (refer details in the code itself)  
<br /><br />

##Task List

The following is a list of tasks that need to be done, may be done and could possibly be done by myself or with the help of others.  I am not precious about the code and would prefer to see it completed than to sit idle ..

- [x] Publish initial code on GitHub
- [ ] More testing (obvious)
- [ ] Determine some appropriate resistor sizes that provide the broadest input values (between 0 - 1023, the resistors I used provide a narrow range between 110 and 330 resulting in some errors).
- [ ] Harden the handling of the input buttons - debounce, etc
- [ ] Support input via the digital inputs using a matrix
- [ ] Support displays of different resolutions  
- [ ] Support I2C and SPI communication to OLED
- [ ] Support three buttons (left, right and select only) operation

<br /><br />
##Confessions 
This is my first GitHub project.  Go easy on me.

