// -------------------------------------------------------------------------------------------------------
// Retrieve and render the detected access points.

int init_access_points() {

  #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS)
    Serial.println("  ");
    Serial.println("init_access_points()");
    Serial.println("  Start scan");
  #endif

  
  // WiFi.scanNetworks will return the number of networks found ..

  renderAccessPoints_Retrieving();
  int n = WiFi.scanNetworks();


  // Forece the 'No Access Points found' test case?
  
  #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS__NO_AP_FOUND__TEST_CASE)
    n = 0;
  #endif
  
  #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS)
    Serial.println("  Scan done");
  #endif

  if (n == 0) {

    #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS)
      Serial.println("  ** No networks found !");
    #endif

    accessPoint_highlightRow = ACCESSPOINT_ROW_MENU;
    
  }
  else {
    
    #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS)
    
      Serial.print("  ");
      Serial.print(n);
      Serial.println(" networks found.");
      
      for (int i = 0; i < n; i++) {
        
        Serial.print("    ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
        delay(10);
        
      }
    #endif

    accessPoint_count = (n > ACCESSPOINT_MAX ? ACCESSPOINT_MAX : n);
    hasAPNameBeenVerified = true;
    
    for (int i = 0; i <= accessPoint_count; i++)  { 
      
       accessPoints[i] = WiFi.SSID(i); 

       // Should the current SSID be highlighted?
       
       if (ssid != "" && WiFi.SSID(i) == ssid) {
        
          accessPoint_highlightRow = i;

          // Make sure the highlighted row is visible ..
          
          if (accessPoint_highlightRow == 0) {    // highlighted row is the top one.

            accessPoint_topRow = 0;
            
          }
          else if (accessPoint_highlightRow < accessPoint_count - ACCESSPOINT_ROWS_VISIBLE) {   // highlighed row is not the top row and is not on the last 'page', set top to highlighed row minus one.

             accessPoint_topRow = accessPoint_highlightRow - 1;
             
          }
          else {  // highlighed row is on the last 'page', ensure that the top row is set to display ACCESSPOINT_ROWS_VISIBLE rows.

            accessPoint_topRow = accessPoint_count - ACCESSPOINT_ROWS_VISIBLE;
            
          }

       }
       
    }
  
  }

  renderAccessPoints();


  // Change modes and wait for user input ..

  status = STATUS_ACCESSPOINTS_LOOP;
  return loop_access_points();

}


// -------------------------------------------------------------------------------------------------------
//  Process user input ..
//

int loop_access_points() {

  while (status == STATUS_ACCESSPOINTS_LOOP) {

    yield();

    #if defined(ADC_INPUT)
    
      sensorValue = analogRead(A0);

      #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS_SENSOR_VALUE)
        if (sensorValue < DEBUG_ACCESSPOINTS_SENSOR_VALUE_THRESHOLD) { Serial.println(sensorValue); }
      #endif
    
      if (sensorValue > (BUTTON_3 - BUTTON_VARIANCE) && sensorValue < (BUTTON_3 + BUTTON_VARIANCE))     { status = loop_access_points_button3_Click(); }
      if (sensorValue > (BUTTON_4 - BUTTON_VARIANCE) && sensorValue < (BUTTON_4 + BUTTON_VARIANCE))     { status = loop_access_points_button4_Click(); }
      if (sensorValue > (BUTTON_5 - BUTTON_VARIANCE) && sensorValue < (BUTTON_5 + BUTTON_VARIANCE))     { status = loop_access_points_button5_Click(); }

    #endif

    #if defined(KY040_INPUT)
    
      long newPosition = myEnc.read();
      
      if (abs(newPosition - oldPosition) >= KY040_MINIMUM_RESOLUTION) {

        if (newPosition > oldPosition)                                                                  { status = loop_access_points_button3_Click(); }
        if (newPosition < oldPosition)                                                                  { status = loop_access_points_button4_Click(); }
    
        oldPosition = newPosition;

      }

      // Software debounce of button press ..
      
      if (isButtonPressed && millis() - lastUpdateMillis > 50) {

        Serial.println("click");
        isButtonPressed = false;
        lastUpdateMillis = millis();                                                                      status = loop_access_points_button5_Click();

        myEnc.write(0);    
        newPosition = 0;
        oldPosition = 0;
        
    }

    #endif
    
    yield();
    
  }

  return status;
  
}


// -------------------------------------------------------------------------------------------------------
//  Up button clicked ..
//

int loop_access_points_button3_Click() {

  #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_access_points_button3_Click():");
    Serial.print("  accessPoint_highlightRow = ");
    Serial.println(accessPoint_highlightRow);
    Serial.print("  accessPoint_topRow = ");
    Serial.println(accessPoint_topRow);
    Serial.print("  accessPoint_count = ");
    Serial.println(accessPoint_count);
  #endif
    
  if (accessPoint_highlightRow == ACCESSPOINT_ROW_MENU) {   

    accessPoint_highlightRow = accessPoint_count - 1; 
    accessPoint_topRow = (accessPoint_count > ACCESSPOINT_ROWS_VISIBLE ? accessPoint_highlightRow - ACCESSPOINT_ROWS_VISIBLE + 1 : 0);
    
  }
  else if (accessPoint_highlightRow == 0) { 

    accessPoint_highlightRow = ACCESSPOINT_ROW_MENU; 
    accessPoint_topRow = 0;
    
  }
  else if (accessPoint_highlightRow > 0) { 

    accessPoint_highlightRow--; 

    if (accessPoint_highlightRow > 0) {

      accessPoint_topRow = accessPoint_highlightRow - 1;
      
    }
   
  }

  renderAccessPoints();
  delay(100);

  #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS_VALUES_FINAL)
    Serial.println("  -- exit values");
    Serial.print("  accessPoint_highlightRow = ");
    Serial.println(accessPoint_highlightRow);
    Serial.print("  accessPoint_topRow = ");
    Serial.println(accessPoint_topRow);
    Serial.print("  accessPoint_count = ");
    Serial.println(accessPoint_count);
    Serial.println("  return = STATUS_ACCESSPOINTS_LOOP");
  #endif
  
  return STATUS_ACCESSPOINTS_LOOP;
  
}


// -------------------------------------------------------------------------------------------------------
//  Down button clicked ..
//

int loop_access_points_button4_Click() {

  #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_access_points_button3_Click():");
    Serial.print("  accessPoint_highlightRow = ");
    Serial.println(accessPoint_highlightRow);
    Serial.print("  accessPoint_topRow = ");
    Serial.println(accessPoint_topRow);
    Serial.print("  accessPoint_count = ");
    Serial.println(accessPoint_count);
  #endif
    
  if (accessPoint_highlightRow == ACCESSPOINT_ROW_MENU) {

    accessPoint_topRow = 0;
    accessPoint_highlightRow = 0;
    
  }
  else if (accessPoint_highlightRow + 1 == accessPoint_count) {

    accessPoint_highlightRow = ACCESSPOINT_ROW_MENU; 
      
  }
  else if (accessPoint_highlightRow < accessPoint_count) { 

    accessPoint_highlightRow++; 

    if (accessPoint_highlightRow >= accessPoint_topRow + ACCESSPOINT_ROWS_VISIBLE) {

      accessPoint_topRow++;

    }

  }

  renderAccessPoints();
  delay(100);

  #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS_VALUES_FINAL)
    Serial.println("  -- exit values");
    Serial.print("  accessPoint_highlightRow = ");
    Serial.println(accessPoint_highlightRow);
    Serial.print("  accessPoint_topRow = ");
    Serial.println(accessPoint_topRow);
    Serial.print("  accessPoint_count = ");
    Serial.println(accessPoint_count);
    Serial.println("  return = STATUS_ACCESSPOINTS_LOOP");
  #endif
  
  return STATUS_ACCESSPOINTS_LOOP;

}


// -------------------------------------------------------------------------------------------------------
//  Select button clicked ..
//

int loop_access_points_button5_Click() {
  
  #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_access_points_button5_Click():");
    Serial.print("  accessPoint_highlightRow = ");
    Serial.println(accessPoint_highlightRow);
    Serial.print("  accessPoint_topRow = ");
    Serial.println(accessPoint_topRow);
    Serial.print("  accessPoint_count = ");
    Serial.println(accessPoint_count);
  #endif

  if (accessPoint_highlightRow == ACCESSPOINT_ROW_MENU) {

    accessPoint_highlightRow = 0;
    accessPoint_topRow = 0;
  
    #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS_VALUES_FINAL)
      Serial.println("  -- exit values");
      Serial.print("  accessPoint_highlightRow = ");
      Serial.println(accessPoint_highlightRow);
      Serial.print("  accessPoint_topRow = ");
      Serial.println(accessPoint_topRow);
      Serial.print("  accessPoint_count = ");
      Serial.println(accessPoint_count);
      Serial.println("  return = STATUS_ACCESSPOINTS_INIT");
    #endif
  
    return STATUS_ACCESSPOINTS_INIT;
    
  }
  else {

    ssid = accessPoints[accessPoint_highlightRow];
    accessPoint_highlightRow = 0;
    accessPoint_topRow = 0;

    #if defined(DEBUG) && defined(DEBUG_ACCESSPOINTS) && defined(DEBUG_ACCESSPOINTS_VALUES_FINAL)
      Serial.print("  -- exit values");
      Serial.print("  accessPoint_highlightRow = ");
      Serial.println(accessPoint_highlightRow);
      Serial.print("  accessPoint_topRow = ");
      Serial.println(accessPoint_topRow);
      Serial.print("  accessPoint_count = ");
      Serial.println(accessPoint_count);
      Serial.println("  return = STATUS_ENTERPASSWORD_INIT");
    #endif
    
    return STATUS_ENTERPASSWORD_INIT;

  }
  
}

