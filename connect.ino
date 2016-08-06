// -------------------------------------------------------------------------------------------------------
// Connect to WiFi network

int init_connect() {

  connect_line1_text = String("Connecting to " + truncateString(String(ssid), CONNECT_SSID_DISPLAY_TRUNCATE, false));
  connect_line2_text = String("Obtaining IP address ..");
  connect_line3_text = "";
  connect_line4_text = "";
  connect_line5_text = "";
  renderConnect(false);
    
  char charSSID[ssid.length() + 1];
  ssid.toCharArray(charSSID, ssid.length() + 1); 

  char charPassword[password.length() + 1];
  password.toCharArray(charPassword, password.length() + 1); 

  #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_VALUES_INIT)

    Serial.println("  ");
    Serial.println("init_connect() ");
    Serial.print("  SSID (String) = ");
    Serial.println(ssid);
    Serial.print("  password (String) = ");
    Serial.println(password);
    Serial.print("  SSID (char[]) = ");
    Serial.println(charSSID);
    Serial.print("  password (char[]) = ");
    Serial.println(charPassword);

  #endif


  // Attempt to connect to the nominated access point ..
   
  WiFi.begin(charSSID, charPassword);
  wifiStatus = WiFi.status();

  #if defined(DEBUG) && defined(DEBUG_CONNECT)
    Serial.println("  status = " + getStatusDesc(wifiStatus));
  #endif

  while (wifiStatus != WL_CONNECTED && wifiStatus != WL_CONNECT_FAILED && wifiStatus != WL_NO_SSID_AVAIL && wifiStatus != WL_IDLE_STATUS) {

    yield();
    delay(180);


    // Create a status bar of (up to) 40 dots ..

    if (connect_line5_text.length() < 40) {
      connect_line5_text = String(connect_line5_text + ".");
      renderConnect(false);
      yield();
    }
    
    wifiStatus = WiFi.status();
    #if defined(DEBUG) && defined(DEBUG_CONNECT)
      Serial.println("  status = " + getStatusDesc(wifiStatus));
    #endif

  }


  // Was the connection successful ?  If so, render the details and progress to the 'actual' application ..
  
  if (wifiStatus == WL_CONNECTED) {

    connect_line2_text = String("IP : " + IpAddress2String(WiFi.localIP()));
    connect_line3_text = "WiFi status: Connected";
    connect_line5_text = "";
    renderConnect(false);
  
    
    // Start the server
    server.begin();
    connect_line4_text = "Server status: Started";
    renderConnect(false);


    // Write SSID and password to EEPROM ..
    
    writeSSIDandPassword(ssid, password);

    #if defined(DEBUG) && defined(DEBUG_EEPROM)
      dumpEEPROMValues();
    #endif


    #if defined(CONNECT_DIM_AFTER_CONNECTION)
      connect_dimAfterConnetionDelay = CONNECT_DIM_AFTER_CONNECTION_DELAY;
    #endif
    
    return STATUS_APPLICATION_INIT;
    
  }
  else {


    // If the connection failed, render an error message ..
    
    wifiStatus = WL_CONNECT_FAILED;
    WiFi.disconnect();
    connect_line2_text = "";
    connect_line3_text = "   WiFi connection failed.";
    connect_line5_text = "";
    connect_highlightCol = CONNECT_MENU_OPTION_BACK;
    renderConnect(true);

    status = STATUS_CONNECT_LOOP;
    loop_connect();
    
  }

}


// -------------------------------------------------------------------------------------------------------
//  Process user input ..
//

int loop_connect() {

  while (status == STATUS_CONNECT_LOOP) {

    yield();
    sensorValue = analogRead(A0);

    #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_SENSOR_VALUE)
      if (sensorValue < DEBUG_CONNECT_SENSOR_VALUE_THRESHOLD) { Serial.println(sensorValue); }
    #endif
    
    if (sensorValue > (BUTTON_1 - BUTTON_VARIANCE) && sensorValue < (BUTTON_1 + BUTTON_VARIANCE))     { status = loop_connect_button1_Click(); }
    if (sensorValue > (BUTTON_2 - BUTTON_VARIANCE) && sensorValue < (BUTTON_2 + BUTTON_VARIANCE))     { status = loop_connect_button2_Click(); }
    if (sensorValue > (BUTTON_5 - BUTTON_VARIANCE) && sensorValue < (BUTTON_5 + BUTTON_VARIANCE))     { status = loop_connect_button5_Click(); }

    yield();
    
  }

  return status;
  
}


// -------------------------------------------------------------------------------------------------------
//  Left button clicked ..
//

int loop_connect_button1_Click() {
 
  #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_connect_button1_Click():");
  #endif
  
  if (connect_highlightCol > CONNECT_MENU_OPTION_BACK) {

    connect_highlightCol--;
    
    renderConnect(wifiStatus == WL_CONNECTED);
    delay(100);
    
  }
  
  #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_VALUES_FINAL)
    Serial.println("  -- exit values");
    Serial.println("  return = STATUS_CONNECT_LOOP");
  #endif
    
  return STATUS_CONNECT_LOOP;
  
}


// -------------------------------------------------------------------------------------------------------
//  Right button clicked ..
//

int loop_connect_button2_Click() {
 
  #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_connect_button2_Click():");
  #endif
  
  if (connect_highlightCol < CONNECT_MENU_OPTION_RETRY) {

    connect_highlightCol++;    
    renderConnect(wifiStatus == WL_CONNECTED);
    delay(100);
    
  }

  #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_VALUES_FINAL)
    Serial.println("  -- exit values");
    Serial.println("  return = STATUS_CONNECT_LOOP");
  #endif
    
  return STATUS_CONNECT_LOOP;
  
}


// -------------------------------------------------------------------------------------------------------
//  Select button clicked ..
//

int loop_connect_button5_Click() {
 
  #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_connect_button5_Click():");
  #endif
   
  if (connect_highlightCol == CONNECT_MENU_OPTION_BACK) {

    Serial.println("loop_connect_button5_Click(), return status : STATUS_ENTERPASSWORD_INIT");
    delay(100);
    
    #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_VALUES_FINAL)
      Serial.println("  -- exit values");
      Serial.println("  return = STATUS_ENTERPASSWORD_INIT");
    #endif


    // If we know the AP name is 'valid' (it was selected but it may have dropped out of range) then 
    // the user has most likely to have entered the password wronng.  Return them to the password
    // entry screen.  If the AP has not been virified then return the user to the AP selection screes,
    
    return (hasAPNameBeenVerified ? STATUS_ENTERPASSWORD_INIT : STATUS_ACCESSPOINTS_INIT);

  }
  
  if (connect_highlightCol == CONNECT_MENU_OPTION_RETRY) {

    Serial.println("loop_connect_button5_Click(), return status : STATUS_ENTERPASSWORD_INIT");
    delay(100);
    wifiStatus = -1;
        
    #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_VALUES_FINAL)
      Serial.println("  -- exit values");
      Serial.println("  return = STATUS_CONNECT_INIT");
    #endif
    
    return STATUS_CONNECT_INIT;

  }
  
}


// -------------------------------------------------------------------------------------------------------
//  Render an IP address in standard xxx.xxx.xxx.xxx format ..
//

String IpAddress2String(const IPAddress& ipAddress) {
  
  return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3])  ;

}


// -------------------------------------------------------------------------------------------------------
//  Connect to the WiFi silently.  
//
//  This is used in your application if the connection is dropped for any reason.
//
//  Returns WL_CONNECTED or WL_CONNECT_FAILED

int connectSilently() {
  
  char charSSID[ssid.length() + 1];
  ssid.toCharArray(charSSID, ssid.length() + 1); 

  char charPassword[password.length() + 1];
  password.toCharArray(charPassword, password.length() + 1); 

  #if defined(DEBUG) && defined(DEBUG_CONNECT) && defined(DEBUG_CONNECT_VALUES_INIT)

    Serial.println("  ");
    Serial.println("connectSilently() ");
    Serial.print("  SSID (String) = ");
    Serial.println(ssid);
    Serial.print("  password (String) = ");
    Serial.println(password);
    Serial.print("  SSID (char[]) = ");
    Serial.println(charSSID);
    Serial.print("  password (char[]) = ");
    Serial.println(charPassword);

  #endif

  
  // Attempt to connect to the nominated access point ..
   
  WiFi.begin(charSSID, charPassword);
  wifiStatus = WiFi.status();

  #if defined(DEBUG) && defined(DEBUG_CONNECT)
    Serial.println("  status = " + getStatusDesc(wifiStatus));
  #endif

  while (wifiStatus != WL_CONNECTED && wifiStatus != WL_CONNECT_FAILED && wifiStatus != WL_NO_SSID_AVAIL && wifiStatus != WL_IDLE_STATUS) {

    yield();
    delay(180);
    
    wifiStatus = WiFi.status();
    #if defined(DEBUG) && defined(DEBUG_CONNECT)
      Serial.println("  status = " + getStatusDesc(wifiStatus));
    #endif

  }


  // Was the connection successful ?  If so, render the details and progress to the 'actual' application ..
  
  if (wifiStatus == WL_CONNECTED) {
  
    
    // Start the server ..
    
    server.begin();
    return WL_CONNECTED;
    
  }
  else {

    return WL_CONNECT_FAILED;
    
  }

}


// -------------------------------------------------------------------------------------------------------
//  Convert WiFi status enum to text ..
//

#if defined(DEBUG) && defined(DEBUG_CONNECT)
String getStatusDesc(int status) {

  if (status == WL_NO_SHIELD)         return "WL_NO_SHIELD";
  if (status == WL_IDLE_STATUS)       return "WL_IDLE_STATUS";
  if (status == WL_NO_SSID_AVAIL)     return "WL_NO_SSID_AVAIL";
  if (status == WL_SCAN_COMPLETED)    return "WL_SCAN_COMPLETED";
  if (status == WL_CONNECTED)         return "WL_CONNECTED";
  if (status == WL_CONNECT_FAILED)    return "WL_CONNECT_FAILED";
  if (status == WL_CONNECTION_LOST)   return "WL_CONNECTION_LOST";

  return "UNKNOWN";

}
#endif


