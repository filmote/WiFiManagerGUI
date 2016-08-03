#include <ESP8266WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <SSD1306.h>
#include <EEPROM.h>

#define WIFI_PORT                                   301


// -- Status / Mode -------------------------------------------------------------------------------------------------------
//
// The sketch is essentially a big state machine and is controlled by the following constants.  

#define STATUS_ACCESSPOINTS_INIT                    0
#define STATUS_ACCESSPOINTS_LOOP                    1
#define STATUS_ENTERPASSWORD_INIT                   2
#define STATUS_ENTERPASSWORD_LOOP                   3
#define STATUS_CONNECT_INIT                         4
#define STATUS_CONNECT_LOOP                         5
#define STATUS_APPLICATION_INIT                     6
#define STATUS_APPLICATION_LOOP                     7

// There are four main phases of the program :  
//
// (1) Selection of an access point
// (2) Entry of the password
// (3) Connect to nominated access point
// (4) Your application
//
// The flow of the state machine is depicted below - the methods are shown followed by brackets along the with relevant status 
// constant that causes the change of state.  AS mentioned, the application consists of four phases and each is typically 
// represented by a pair of methods - an xxx_init() and a xxx_loop() method.  Unlike the main setup() function of a sketch, which 
// is called exactly one during the operation of the sketch, the _init() function will be called each time the phase of operation
// is entered.  The _init() functions are used to initialise parameters and render the screen for that phase's operation whereas 
// the loop() method is used to handle user input (for example the clicking of buttons).
//
// The main loop of the sketch simply passes control to the respective _init() method of the current phase before it enters its 
// own _loop() method where processing continues until a status return code is set that forces the program to exit.  The diagram 
// details the valid return code for each loop.  Note that the first the specified return status code for each phase is that of the
// phase's initializer.  This is used to refresh the screen based on updates performed in the loop().  
//
//   setup()
//     | 
//     | 
//   loop() --+
//     |      |
//     |      |  // (1) Select an Access Point ..
//     |      |
//     ^      +------------------------------->    init_access_points() --------------------------------+-->   loop_access_points()  ---+------------------>--------------------+
//     |      |   STATUS_ACCESSPOINTS_INIT                                 STATUS_ACCESSPOINTS_LOOP     |                               |      STATUS_ACCESSPOINTS_INIT         |
//     |      |                                                                                         +---------------<---------------+                  or                   |
//     |      |                                                                                              STATUS_ACCESSPOINTS_LOOP          STATUS_ENTERPASSWORD_INIT        |
//     |      |                                                                                                                                                                 V
//     |      |                                                                                                                                                                 |
//     |      |                                                                                                                                                                 |
//     |      |  // (2) Enter the password ..                                                                                                                                   |
//     |      |                                                                                                                                                                 |
//     ^      +------------------------------>    init_enter_password()   ------------------------------+-->   loop_enter_password()  --+------------------>--------------------+
//     |      |   STATUS_ENTERPASSWORD_INIT                                 STATUS_ENTERPASSWORD_LOOP   |                               |      STATUS_ENTERPASSWORD_INIT        |
//     |      |                                                                                         +---------------<---------------+                  or                   |
//     |      |                                                                                             STATUS_ENTERPASSWORD_LOOP              STATUS_CONNECT_INIT          |
//     |      |                                                                                                                                                                 V
//     |      |                                                                                                                                                                 |
//     |      |                                                                                                                                                                 |
//     |      |  // (3) Attempt to connect to the accessoint ..                                                                                                                 |
//     |      |                                                                                                                                                                 |
//     ^      +---------------------------------->    init_connect()   ---------------------------------+----->   loop_connect()  ------+------------------>--------------------+
//     |      |      STATUS_CONNECT_INIT                                      STATUS_CONNECT_LOOP       |                               |        STATUS_CONNECT_INIT            |
//     |      |                                                                                         +---------------<---------------+                 or                    |
//     |      |                                                                                                STATUS_CONNECT_LOOP             STATUS_ACCESSPOINTS_INIT         |
//     |      |                                                                                                                                           or                    |
//     |      |                                                                                                                               STATUS_ENTERPASSWORD_INIT         V
//     |      |                                                                                                                                                                 |
//     |      |                                                                                                                                                                 |
//     |      |  // (4) Your application ..                                                                                                                                     |
//     |      |                                                                                                                                                                 |
//     |      +------------------------------------------------->            <<<<<<   You application logic.  >>>>>>           ---------------------->--------------------------+
//     |           STATUS_APPLICATION_INIT                                                                                                     STATUS_ACCESSPOINTS_INIT         |
//     |                                                                      In the sample code provided, this is                                        or                    |
//     |                                                                      implemented in the init_application()                            STATUS_ENTERPASSWORD_INIT        |
//     |                                                                         and loop_application() methods.                                          or                    V
//     |                                                                                                                                          STATUS_CONNECT_INIT           |
//     |                                                                                                                                                                        |                
//     +-------------------------------------------------------------------------------------------<----------------------------------------------------------------------------+
// 

int status = STATUS_ACCESSPOINTS_INIT;


// -- OLED Configuration --------------------------------------------------------------------------------------------------

// Refer to https://github.com/squix78/esp8266-oled-ssd1306 for details on Daniel Eichhorn's great library for using an
// SSD1306 display with the ESP8266.  The sketch only uses the basic library to render screens and ignores the advance UI 
// library that provides frames and overlays.
//
 
#define OLED_SDA                                    D1          // pin 14
#define OLED_SDC                                    D2          // pin 12
#define OLED_ADDR                                   0x3C


// -- DEBUG Constants -----------------------------------------------------------------------------------------------------
//
// The value form a hierarchy allowing you to turn off debugging all together or for a particular section of the code, as 
// shown in the sample below. The keywords #define and #undef can be used to quickly enable or disable, respectively, the 
// debugging from that point in the hierarchy down.
//
// +- DEBUG
//    +- DEBUG_EEPROM
//       +- DEBUG_EEPROM_READ
//       +- DEBUG_EEPROM_CLEAR_SETTINGS
//       +- DEBUG_EEPROM_WRITE_SETTINGS
//    +- DEBUG_ACCESSPOINTS
//       +- DEBUG_ACCESSPOINTS_SENSOR_VALUE
//       +- DEBUG_ACCESSPOINTS_SENSOR_VALUE_THRESHOLD
//       +- ... and so on.
//
// Examples:
//
// The following will enable all debug logging down to and including the EEPROM read functions.
//
//   #define DEBUG
//   #define DEBUG_EEPROM
//   #define DEBUG_EEPROM_READ
//
// The following will disable all debug logging, as the DEBUG definition is the parent of all other definitions.
//
//   #undef  DEBUG
//   #define DEBUG_EEPROM
//   #define DEBUG_EEPROM_READ
//   #define DEBUG_ACCESSPOINTS
//   #define DEBUG_ACCESSPOINTS_SENSOR_VALUE
//
// The following will disable debug logging within the acces point selection code.  The EEPROM code will log debug messages.
//
//   #define DEBUG
//   #define DEBUG_EEPROM
//   #define DEBUG_EEPROM_READ
//   #undef  DEBUG_ACCESSPOINTS
//   #define DEBUG_ACCESSPOINTS_SENSOR_VALUE
//
// When including logging within the code, all definitions in the hierarchy must be included.  This is shown in the example
// below.  Note also, the leading .println before the echoing of the method name.  Variables are indented two characters
// with the variable name followed by an equals sign and then the variable value.
//
//  #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS_VALUES_INIT)
//    Serial.println("");
//    Serial.println("loop_access_points_button5_Click():");
//    Serial.print("  accessPoint_highlightRow = ");
//    ..
//  #endif
//
    
#define DEBUG
#define DEBUG_EEPROM
#define DEBUG_EEPROM_READ
#undef  DEBUG_EEPROM_CLEAR_SETTINGS                         // Clear the EEPROM settings
#define DEBUG_EEPROM_WRITE_SETTINGS                         // Write test data into the EEPROM

#define DEBUG_ACCESSPOINTS
#define DEBUG_ACCESSPOINTS_SENSOR_VALUE
#define DEBUG_ACCESSPOINTS_SENSOR_VALUE_THRESHOLD   700
#define DEBUG_ACCESSPOINTS_VALUES_INIT
#define DEBUG_ACCESSPOINTS_VALUES_FINAL
#undef  DEBUG_ACCESSPOINTS__NO_AP_FOUND__TEST_CASE

#define DEBUG_PASSWORD
#define DEBUG_PASSWORD_SENSOR_VALUE
#define DEBUG_PASSWORD_SENSOR_VALUE_THRESHOLD       700
#define DEBUG_PASSWORD_VALUES_INIT
#define DEBUG_PASSWORD_VALUES_FINAL

#define DEBUG_CONNECT
#define DEBUG_CONNECT_VALUES_INIT
#define DEBUG_CONNECT_VALUES_FINAL
#define DEBUG_CONNECT_SENSOR_VALUE
#define DEBUG_CONNECT_SENSOR_VALUE_THRESHOLD        700


// -- EEPROM Constants -----------------------------------------------------------------------------------------------------
//
// To ensure the EEPROM has been 'initialised' with valid SSID and password values, the byte at EEPROM_TEST_CHAR_POS in the
// EEPROM is tested to ensure it contains a known value, defined by the character EEPROM_TEST_CHAR_POS_VALUE.  SSID and 
// password values are written out with a single byte declaring the length of the value followed by the value itself.
//
// For example, the SSID value of 'Test' will bw written as the HEX values:    
//
//   04 54 65 73 74 .. .. .. ..
// 
// Where 04 is the length of the SSID and 54, 65, 73 and 74 reresent the ASCII values of the value.
//
// The SSID and password lengths are capped at EEPROM_SSID_MAX_LENGTH and EEPROM_PASSWORD_MAX_LENGTH characters respectively.
// When writing the values to the EEPROM, there is no need to zero out characters to the right of the value as the inclusion
// of the value length negates this need as retrieval will only read back the nominated number of characters.
//

#define EEPROM_TEST_CHAR_POS                        1
#define EEPROM_TEST_CHAR_POS_VALUE                  'a'
#define EEPROM_SSID_LENGTH_POS                      (EEPROM_TEST_CHAR_POS + 1)
#define EEPROM_SSID_START_POS                       (EEPROM_SSID_LENGTH_POS + 1)      
#define EEPROM_SSID_MAX_LENGTH                      32     
#define EEPROM_PASSWORD_LENGTH_POS                  (EEPROM_SSID_START_POS + EEPROM_SSID_MAX_LENGTH + 1) 
#define EEPROM_PASSWORD_START_POS                   (EEPROM_PASSWORD_LENGTH_POS + 1)
#define EEPROM_PASSWORD_MAX_LENGTH                  32     


// -- Button ADC Values ----------------------------------------------------------------------------------------------------
//
// To free up the digital input and output pins, the sketch uses five resistors connected in series to the ADC input.  Five 
// normally-open switches are wired between the resistors and connect the ADC input to ground when pressed between - resulting 
// in five different analogue values.  The values I have entered below work for the resistors I have used in my prototype 
// but will need to be changed to suit your sketch unless you *happen* to use the same values as me (unlikely).  To cater for
// the inaccuracy of the resistors / ADC, a variance threshold has been included.
//
// Example code:
//
//  sensorValue = analogRead(A0);
//
//  if (sensorValue > (BUTTON_3 - BUTTON_VARIANCE) && sensorValue < (BUTTON_3 + BUTTON_VARIANCE))     { status = loop_access_points_button3_Click(); }
//  if (sensorValue > (BUTTON_4 - BUTTON_VARIANCE) && sensorValue < (BUTTON_4 + BUTTON_VARIANCE))     { status = loop_access_points_button4_Click(); }
//  if (sensorValue > (BUTTON_5 - BUTTON_VARIANCE) && sensorValue < (BUTTON_5 + BUTTON_VARIANCE))     { status = loop_access_points_button5_Click(); }
//

#define BUTTON_1                                    110
#define BUTTON_2                                    185
#define BUTTON_3                                    245
#define BUTTON_4                                    290
#define BUTTON_5                                    330
#define BUTTON_VARIANCE                             25


// -- Scroll Bar Rendering ------------------------------------------------------------------------------------------------
// 
// Constants used in the graphical rendering of the scroolbar component ..
//

#define SCROLLBAR_AREA_TOP                          0
#define SCROLLBAR_AREA_LEFT                         117
#define SCROLLBAR_AREA_WIDTH                        10
#define SCROLLBAR_AREA_HEIGHT                       63

#define SCROLLBAR_BORDER_TOP                        (SCROLLBAR_AREA_TOP)
#define SCROLLBAR_BORDER_LEFT                       (SCROLLBAR_AREA_LEFT + 2)
#define SCROLLBAR_BORDER_WIDTH                      (SCROLLBAR_AREA_WIDTH - 2)
#define SCROLLBAR_BORDER_HEIGHT                     (SCROLLBAR_AREA_HEIGHT)

#define SCROLLBAR_SLIDER_TOP                        (SCROLLBAR_AREA_TOP + 7)
#define SCROLLBAR_SLIDER_LEFT                       (SCROLLBAR_AREA_LEFT + 4)
#define SCROLLBAR_SLIDER_WIDTH                      (SCROLLBAR_AREA_WIDTH - 5)
#define SCROLLBAR_SLIDE_MAX_HEIGHT                  (SCROLLBAR_BORDER_HEIGHT - 13)

#define SCROLLBAR_INDICATOR_UP_TOP                  (SCROLLBAR_BORDER_TOP + 2)
#define SCROLLBAR_INDICATOR_DOWN_TOP                (SCROLLBAR_BORDER_HEIGHT - 4)


// -- Access Point Constants ----------------------------------------------------------------------------------------------
//
// Constants used to render the Access Point selection list and associated menu options.
//

#define ACCESSPOINT_HIGHLIGHT_ROW_LEFT              0
#define ACCESSPOINT_HIGHLIGHT_ROW_TOP               16
#define ACCESSPOINT_HIGHLIGHT_ROW_WIDTH             127
#define ACCESSPOINT_HIGHLIGHT_ROW_TEXT_TOP          15
#define ACCESSPOINT_ROW_HEIGHT                      11
#define ACCESSPOINT_ROW_MENU                        -1
#define ACCESSPOINT_ROWS_VISIBLE                    3
#define ACCESSPOINT_MAX 12

#define ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_LEFT     1
#define ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_TOP      53
#define ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_WIDTH    44
#define ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_HEIGHT   11
#define ACCESSPOINT_MENU_REFRESH_IMAGE_LEFT         (ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_LEFT + 1)
#define ACCESSPOINT_MENU_REFRESH_IMAGE_TOP          (ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_TOP + 3)
#define ACCESSPOINT_MENU_REFRESH_TEXT_LEFT          (ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_LEFT + 7)
#define ACCESSPOINT_MENU_REFRESH_TEXT_TOP           (ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_TOP - 1)

#define ACCESSPOINT_HEADING_TEXT_TOP                0
#define ACCESSPOINT_HEADING_TEXT_LEFT               1
#define ACCESSPOINT_PROGRESS_MESSAGE_TEXT_LEFT      1
#define ACCESSPOINT_PROGRESS_MESSAGE_TEXT_TOP       25
#define ACCESSPOINT_PROGRESS_MESSAGE_TEXT_LEFT      1
#define ACCESSPOINT_ERROR_MESSAGE_TEXT_TOP          26
#define ACCESSPOINT_ERROR_MESSAGE_TEXT_LEFT         13
#define ACCESSPOINT_ERROR_MESSAGE_IMAGE_TOP         28
#define ACCESSPOINT_ERROR_MESSAGE_IMAGE_LEFT        1

int accessPoint_highlightRow = 0;
int accessPoint_topRow = 0;
int accessPoint_count = 0;

String accessPoints[ACCESSPOINT_MAX];


// -- Password Entry Constants ----------------------------------------------------------------------------------------------
//
// Constants used to render the Password Entry dialogue and associated menu options.
//

#define PASSWORD_ROW_1_TOP                          15
#define PASSWORD_ROW_2_TOP                          27
#define PASSWORD_ROW_3_TOP                          39
#define PASSWORD_HIGHLIGHT_SIZE                     10
#define PASSWORD_HIGHLIGHT_LEFT                     1
#define PASSWORD_CHAR_SPACING                       12
#define PASSWORD_CHAR_LEFT                          (PASSWORD_HIGHLIGHT_LEFT + 1)
#define PASSWORD_CHARS_PER_ROW                      9
#define PASSWORD_ROWS_VISIBLE                       3

#define PASSWORD_KEY_LABEL_LEFT                     1
#define PASSWORD_KEY_LABEL_TOP                      0
#define PASSWORD_KEY_BORDER_LEFT                    33
#define PASSWORD_KEY_BORDER_TOP                     0
#define PASSWORD_KEY_BORDER_WIDTH                   82
#define PASSWORD_KEY_BORDER_HEIGHT                  12

#define PASSWORD_MENU_ROW                           -1
#define PASSWORD_MENU_OPTION_BACK                   0
#define PASSWORD_MENU_OPTION_DELETE                 1
#define PASSWORD_MENU_OPTION_CONNECT                2

#define PASSWORD_MENU_BACK_HIGHLIGHT_LEFT           1
#define PASSWORD_MENU_BACK_HIGHLIGHT_TOP            53
#define PASSWORD_MENU_BACK_HIGHLIGHT_WIDTH          31
#define PASSWORD_MENU_BACK_HIGHLIGHT_HEIGHT         11
#define PASSWORD_MENU_BACK_IMAGE_LEFT               (PASSWORD_MENU_BACK_HIGHLIGHT_LEFT + 1)
#define PASSWORD_MENU_BACK_IMAGE_TOP                (PASSWORD_MENU_BACK_HIGHLIGHT_TOP + 2)
#define PASSWORD_MENU_BACK_TEXT_LEFT                (PASSWORD_MENU_BACK_HIGHLIGHT_LEFT + 7)
#define PASSWORD_MENU_BACK_TEXT_TOP                 (PASSWORD_MENU_BACK_HIGHLIGHT_TOP - 1)

#define PASSWORD_MENU_DELETE_HIGHLIGHT_LEFT         (PASSWORD_MENU_BACK_HIGHLIGHT_LEFT + 34)
#define PASSWORD_MENU_DELETE_HIGHLIGHT_TOP          (PASSWORD_MENU_BACK_HIGHLIGHT_TOP)
#define PASSWORD_MENU_DELETE_HIGHLIGHT_WIDTH        32
#define PASSWORD_MENU_DELETE_HIGHLIGHT_HEIGHT       (PASSWORD_MENU_BACK_HIGHLIGHT_HEIGHT)
#define PASSWORD_MENU_DELETE_TEXT_LEFT              (PASSWORD_MENU_DELETE_HIGHLIGHT_LEFT + 1)
#define PASSWORD_MENU_DELETE_TEXT_TOP               (PASSWORD_MENU_DELETE_HIGHLIGHT_TOP - 1)

#define PASSWORD_MENU_CONNECT_HIGHLIGHT_LEFT        (PASSWORD_MENU_DELETE_HIGHLIGHT_LEFT + 35)
#define PASSWORD_MENU_CONNECT_HIGHLIGHT_TOP         (PASSWORD_MENU_DELETE_HIGHLIGHT_TOP)
#define PASSWORD_MENU_CONNECT_HIGHLIGHT_WIDTH       43
#define PASSWORD_MENU_CONNECT_HIGHLIGHT_HEIGHT      (PASSWORD_MENU_BACK_HIGHLIGHT_HEIGHT)
#define PASSWORD_MENU_CONNECT_TEXT_LEFT             (PASSWORD_MENU_CONNECT_HIGHLIGHT_LEFT + 1)
#define PASSWORD_MENU_CONNECT_TEXT_TOP              (PASSWORD_MENU_CONNECT_HIGHLIGHT_TOP - 1)

#define PASSWORD_MENU_DIVIDER1_LEFT                 (PASSWORD_MENU_BACK_HIGHLIGHT_LEFT + PASSWORD_MENU_BACK_HIGHLIGHT_WIDTH + 1)
#define PASSWORD_MENU_DIVIDER2_LEFT                 (PASSWORD_MENU_DIVIDER1_LEFT + PASSWORD_MENU_DELETE_HIGHLIGHT_WIDTH + 3)
#define PASSWORD_MENU_DIVIDERS_TOP                  (PASSWORD_MENU_BACK_HIGHLIGHT_TOP)
#define PASSWORD_MENU_DIVIDERS_HEIGHT               (PASSWORD_MENU_BACK_HIGHLIGHT_HEIGHT)

char password_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890`~!@#$%^&*()-_=+[]{};:'<>,./?";

int password_topRow = 0;
int password_chars_per_row = 9;
int password_rowCount = (((sizeof(password_chars) - 1) % password_chars_per_row) == 0 ? (sizeof(password_chars) - 1) / password_chars_per_row : ((sizeof(password_chars) - 1) / password_chars_per_row) + 1);
int password_charsOnLastRow = ((sizeof(password_chars) - 1) % password_chars_per_row == 0 ? PASSWORD_CHARS_PER_ROW : (sizeof(password_chars) - 1) % password_chars_per_row);
int password_highlightRow = 0;
int password_highlightCol = 0;


// -- Connection Constants ------------------------------------------------------------------------------------------------
//
// Constants used to render the Connection screen and associated menu options.
//

#define CONNECT_MENU_OPTION_BACK                    0
#define CONNECT_MENU_OPTION_RETRY                   1

#define CONNECT_MENU_BACK_HIGHLIGHT_LEFT            1
#define CONNECT_MENU_BACK_HIGHLIGHT_TOP             53
#define CONNECT_MENU_BACK_HIGHLIGHT_WIDTH           31
#define CONNECT_MENU_BACK_HIGHLIGHT_HEIGHT          11
#define CONNECT_MENU_BACK_IMAGE_LEFT                (CONNECT_MENU_BACK_HIGHLIGHT_LEFT + 1)
#define CONNECT_MENU_BACK_IMAGE_TOP                 (CONNECT_MENU_BACK_HIGHLIGHT_TOP + 2)
#define CONNECT_MENU_BACK_TEXT_LEFT                 (CONNECT_MENU_BACK_HIGHLIGHT_LEFT + 7)
#define CONNECT_MENU_BACK_TEXT_TOP                  (CONNECT_MENU_BACK_HIGHLIGHT_TOP - 1)

#define CONNECT_MENU_RETRY_HIGHLIGHT_LEFT           (CONNECT_MENU_BACK_HIGHLIGHT_LEFT + 34)
#define CONNECT_MENU_RETRY_HIGHLIGHT_TOP            (CONNECT_MENU_BACK_HIGHLIGHT_TOP)
#define CONNECT_MENU_RETRY_HIGHLIGHT_WIDTH          26
#define CONNECT_MENU_RETRY_HIGHLIGHT_HEIGHT         (CONNECT_MENU_BACK_HIGHLIGHT_HEIGHT)
#define CONNECT_MENU_RETRY_TEXT_LEFT                (CONNECT_MENU_RETRY_HIGHLIGHT_LEFT + 1)
#define CONNECT_MENU_RETRY_TEXT_TOP                 (CONNECT_MENU_RETRY_HIGHLIGHT_TOP - 1)

#define CONNECT_MENU_DIVIDER1_LEFT                  (CONNECT_MENU_BACK_HIGHLIGHT_LEFT + CONNECT_MENU_BACK_HIGHLIGHT_WIDTH + 1)
#define CONNECT_MENU_DIVIDERS_TOP                   (CONNECT_MENU_BACK_HIGHLIGHT_TOP)
#define CONNECT_MENU_DIVIDERS_HEIGHT                (CONNECT_MENU_BACK_HIGHLIGHT_HEIGHT)

#define CONNECT_DIM_AFTER_CONNECTION                             // Once connected, should the screen be dimmed after a few seconds.
#define CONNECT_DIM_AFTER_CONNECTION_DELAY          30           // This is approximately 3 seconds but is *totally* dependant on the code in your application

#define CONNECT_RETRY_AUTOMATICALLY                             // Should the application re-attempt to connect if the signal is lost?
#define CONNECT_RETRY_ATTEMPTS                      3

String connect_line1_text = "";
String connect_line2_text = "";
String connect_line3_text = "";
String connect_line4_text = "";
String connect_line5_text = "";
String connect_line6_text = "";

int connect_highlightCol = 0;

#if defined(CONNECT_DIM_AFTER_CONNECTION)
  int connect_dimAfterConnetionDelay = 0;
#endif


// -- Global Variables ----------------------------------------------------------------------------------------------
//

int wifiStatus = 0;
String ssid = "";
String password = "";
int sensorValue = 0;

bool hasAPNameBeenVerified = false;               // Has the AP name been verified (ie. selected from a list) ?



SSD1306          display(OLED_ADDR, OLED_SDA, OLED_SDC);    // For I2C

WiFiServer server(WIFI_PORT); 


// -- Initialization -----------------------------------------------------------------------------------------------
//

void setup() {

  #if defined(DEBUG)
    Serial.begin(115200);
  #endif

  EEPROM.begin(512);

  #if defined(DEBUG) && defined(DEBUG_EEPROM)
    dumpEEPROMValues();
  #endif

  delay(10);

  #if defined(DEBUG)
    Serial.println();
    Serial.println("setup()");
  #endif


  // Clear the EEPROM test character ..

  #if defined(DEBUG) && defined(DEBUG_EEPROM_CLEAR_SETTINGS)
    Serial.println("  Clearing EEPROM data");
    clearTestChar();
  #endif


  // Populate the EEPROM with an SSID and password for testing ..

  #if defined(DEBUG) && defined(DEBUG_EEPROM_WRITE_SETTINGS)
    Serial.println("  Writing test data to EEPROM");
    writeSSIDandPassword("Telstra06AE", "143803385");  
    EEPROM.commit();
  #endif
            
  #if defined(DEBUG) && defined(DEBUG_EEPROM)
    dumpEEPROMValues();
  #endif


  // If a SSID and password have been saved, retrieve them and attempt connection ..
   
  if (validEEPROMSettings()) {

    ssid = readSSIDValue();
    password = readPasswordValue();

    #if defined(DEBUG) && defined(DEBUG_EEPROM)
      Serial.println("  Saved SSID and password found ");
      Serial.print("    SSID = ");
      Serial.println(ssid);
      Serial.print("    Password = ");
      Serial.println(password);
    #endif
    
    status = STATUS_CONNECT_INIT;
    
  }
  #if defined(DEBUG) && defined(DEBUG_EEPROM)
    else {
      Serial.println("  No saved SSID or password found");
    }
  #endif

  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected ..

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  display.init();
  display.displayOn();
  
}


// -- Main Loop -----------------------------------------------------------------------------------------------
//

void loop() {

  #if defined(DEBUG) 
  
    if (status != STATUS_APPLICATION_INIT) {
      
      Serial.println("  ");
      Serial.println("loop()");
      Serial.print("  status = ");
      
      if (status == STATUS_ACCESSPOINTS_INIT)      { Serial.println("STATUS_ACCESSPOINTS_INIT"); }
      if (status == STATUS_ENTERPASSWORD_INIT)     { Serial.println("STATUS_ENTERPASSWORD_INIT"); }
      if (status == STATUS_CONNECT_INIT)           { Serial.println("STATUS_CONNECT_INIT"); }
      if (status == STATUS_APPLICATION_INIT)       { Serial.println("STATUS_APPLICATION_INIT"); }

    }
    
  #endif
    
  if (status == STATUS_ACCESSPOINTS_INIT)        { status = init_access_points(); }
  if (status == STATUS_ENTERPASSWORD_INIT)       { status = init_enter_password(); }
  if (status == STATUS_CONNECT_INIT)             { status = init_connect(); }
  if (status == STATUS_APPLICATION_INIT)         { status = init_application(); }

  yield();

}






