// -------------------------------------------------------------------------------------------------------
// Render a 'keyboard' allowing the user to enter a password ..

int init_enter_password() {

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_INIT)
    Serial.println("  ");
    Serial.println("init_enter_password()");
  #endif

  renderEnterPassword();


  // Change modes and wait for user input ..

  status = STATUS_ENTERPASSWORD_LOOP;
  return loop_enter_password();

}


// -------------------------------------------------------------------------------------------------------
//  Process user input ..
//

int loop_enter_password() {

  while (status == STATUS_ENTERPASSWORD_LOOP) {

    yield();
    delay(100);
    
    sensorValue = analogRead(A0);
    
    #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_SENSOR_VALUE)
      if (sensorValue < DEBUG_PASSWORD_SENSOR_VALUE_THRESHOLD) { Serial.println(sensorValue); }
    #endif
    
    if (sensorValue > (BUTTON_1 - BUTTON_VARIANCE) && sensorValue < (BUTTON_1 + BUTTON_VARIANCE))     { status = loop_enter_password_button1_Click(); }     // Left 
    if (sensorValue > (BUTTON_2 - BUTTON_VARIANCE) && sensorValue < (BUTTON_2 + BUTTON_VARIANCE))     { status = loop_enter_password_button2_Click(); }     // Right
    if (sensorValue > (BUTTON_3 - BUTTON_VARIANCE) && sensorValue < (BUTTON_3 + BUTTON_VARIANCE))     { status = loop_enter_password_button3_Click(); }     // Up
    if (sensorValue > (BUTTON_4 - BUTTON_VARIANCE) && sensorValue < (BUTTON_4 + BUTTON_VARIANCE))     { status = loop_enter_password_button4_Click(); }     // Down
    if (sensorValue > (BUTTON_5 - BUTTON_VARIANCE) && sensorValue < (BUTTON_5 + BUTTON_VARIANCE))     { status = loop_enter_password_button5_Click(); }     // Enter
    
  }

  return status;
  
}


// -------------------------------------------------------------------------------------------------------
//  Left button clicked ..
//

int loop_enter_password_button1_Click() {

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_enter_password_button1_Click():");
    Serial.print("  password_highlightRow = ");
    Serial.println(password_highlightRow);
    Serial.print("  password_highlightCol = ");
    Serial.println(password_highlightCol);
    Serial.print("  password_topRow = ");
    Serial.println(password_topRow);
    Serial.print("  password_rowCount = ");
    Serial.println(password_rowCount);
  #endif
  
  if (password_highlightCol > 0) { 

    password_highlightCol--; 
    
  }

  renderEnterPassword();
  delay(100);

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_FINAL)
    Serial.println("  -- exit values");
    Serial.print("  password_highlightRow = ");
    Serial.println(password_highlightRow);
    Serial.print("  password_highlightCol = ");
    Serial.println(password_highlightCol);
    Serial.print("  password_topRow = ");
    Serial.println(password_topRow);
    Serial.print("  password_rowCount = ");
    Serial.println(password_rowCount);
    Serial.println("  return = STATUS_ENTERPASSWORD_LOOP");
  #endif
  
  return STATUS_ENTERPASSWORD_LOOP;
  
}


// -------------------------------------------------------------------------------------------------------
//  Right button clicked ..
//
 
int loop_enter_password_button2_Click() {

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_enter_password_button2_Click():");
    Serial.print("  password_highlightRow = ");
    Serial.println(password_highlightRow);
    Serial.print("  password_highlightCol = ");
    Serial.println(password_highlightCol);
    Serial.print("  password_topRow = ");
    Serial.println(password_topRow);
    Serial.print("  password_rowCount = ");
    Serial.println(password_rowCount);
  #endif

  if (password_highlightRow == PASSWORD_MENU_ROW && password_highlightCol < 2) {

    password_highlightCol++; 
    
  }
  else if (password_highlightRow == password_rowCount - 1) {

    if (password_highlightCol < password_charsOnLastRow - 1) {

      password_highlightCol++; 
        
    }
    
  }
  else if (password_highlightCol < PASSWORD_CHARS_PER_ROW - 1) {

    password_highlightCol++; 
      
  }

  renderEnterPassword();
  delay(100);

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_FINAL)
    Serial.println("  -- exit values");
    Serial.print("  password_highlightRow = ");
    Serial.println(password_highlightRow);
    Serial.print("  password_highlightCol = ");
    Serial.println(password_highlightCol);
    Serial.print("  password_topRow = ");
    Serial.println(password_topRow);
    Serial.print("  password_rowCount = ");
    Serial.println(password_rowCount);
    Serial.println("  return = STATUS_ENTERPASSWORD_LOOP");
  #endif

  return STATUS_ENTERPASSWORD_LOOP;

}


// -------------------------------------------------------------------------------------------------------
//  Up button clicked ..
//

int loop_enter_password_button3_Click() {

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_enter_password_button3_Click():");
    Serial.print("  password_highlightRow = ");
    Serial.println(password_highlightRow);
    Serial.print("  password_highlightCol = ");
    Serial.println(password_highlightCol);
    Serial.print("  password_topRow = ");
    Serial.println(password_topRow);
    Serial.print("  password_rowCount = ");
    Serial.println(password_rowCount);
  #endif
  
  if (password_highlightRow == PASSWORD_MENU_ROW) { 

    password_highlightCol = 0;
    password_highlightRow = password_rowCount - 1; 
    password_topRow = (password_rowCount > PASSWORD_ROWS_VISIBLE ? password_highlightRow - PASSWORD_ROWS_VISIBLE + 1 : 0);
    
  }
  else if (password_highlightRow == 0) { 

    password_highlightRow = PASSWORD_MENU_ROW; 
    password_highlightCol = 0;
    accessPoint_topRow = 0;
    
  }
  else if (password_highlightRow == password_rowCount - 1) {

    password_highlightRow--; 
    
    if (password_highlightCol > password_charsOnLastRow - 1) {

      password_highlightCol = password_charsOnLastRow - 1;
      
    }

    if (password_highlightRow > 0) {

      password_topRow = password_highlightRow - 1;
      
    } 
        
  }
  else if (password_highlightRow > 0) { 

    password_highlightRow--; 

    if (password_highlightRow > 0) {

      password_topRow = password_highlightRow - 1;
      
    }
   
  }

  renderEnterPassword();
  delay(100);

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_FINAL)
    Serial.println("  -- exit values");
    Serial.print("  password_highlightRow = ");
    Serial.println(password_highlightRow);
    Serial.print("  password_highlightCol = ");
    Serial.println(password_highlightCol);
    Serial.print("  password_topRow = ");
    Serial.println(password_topRow);
    Serial.print("  password_rowCount = ");
    Serial.println(password_rowCount);
    Serial.println("  return = STATUS_ENTERPASSWORD_LOOP");
  #endif

  return STATUS_ENTERPASSWORD_LOOP;
  
}


// -------------------------------------------------------------------------------------------------------
//  Down button clicked ..
//

int loop_enter_password_button4_Click() {

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_enter_password_button4_Click():");
    Serial.print("  password_highlightRow = ");
    Serial.println(password_highlightRow);
    Serial.print("  password_highlightCol = ");
    Serial.println(password_highlightCol);
    Serial.print("  password_topRow = ");
    Serial.println(password_topRow);
    Serial.print("  password_rowCount = ");
    Serial.println(password_rowCount);
  #endif

  if (password_highlightRow == PASSWORD_MENU_ROW) {

    password_topRow = 0;
    password_highlightCol = 0;
    password_highlightRow = 0;
    
  }
  else if (password_highlightRow + 1 == password_rowCount) {

    password_highlightRow = PASSWORD_MENU_ROW; 
      
  }
  else if (password_highlightRow + 2 == password_rowCount) {  // About to move onto the last row.  It may not have PASSWORD_CHARS_PER_ROW characters on it.

    password_highlightRow++; 
    
    if (password_highlightCol > password_charsOnLastRow - 1) {

      password_highlightCol = password_charsOnLastRow - 1;
      
    }

    if (password_highlightRow >= password_topRow + PASSWORD_ROWS_VISIBLE) {

      password_topRow++;

    }
        
  }
  else if (password_highlightRow < password_rowCount) { 

    password_highlightRow++; 

    if (password_highlightRow >= password_topRow + PASSWORD_ROWS_VISIBLE) {

      password_topRow++;

    }

  }

  renderEnterPassword();
  delay(100);

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_FINAL)
    Serial.println("  -- exit values");
    Serial.print("  password_highlightRow = ");
    Serial.println(password_highlightRow);
    Serial.print("  password_highlightCol = ");
    Serial.println(password_highlightCol);
    Serial.print("  password_topRow = ");
    Serial.println(password_topRow);
    Serial.print("  password_rowCount = ");
    Serial.println(password_rowCount);
    Serial.println("  return = STATUS_ENTERPASSWORD_LOOP");
  #endif
  
  return STATUS_ENTERPASSWORD_LOOP;

}


// -------------------------------------------------------------------------------------------------------
//  Select button clicked ..
//

int loop_enter_password_button5_Click() {

  #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_INIT)
    Serial.println("  ");
    Serial.println("loop_enter_password_button5_Click():");
    Serial.print("  password_highlightRow = ");
    Serial.println(password_highlightRow);
    Serial.print("  password_highlightCol = ");
    Serial.println(password_highlightCol);
    Serial.print("  password_topRow = ");
    Serial.println(password_topRow);
    Serial.print("  password_rowCount = ");
    Serial.println(password_rowCount);
    Serial.print("  password = ");
    Serial.println(password);
  #endif  

  if (password_highlightRow == PASSWORD_MENU_ROW) {

    if (password_highlightCol == PASSWORD_MENU_OPTION_BACK) {

        #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_FINAL)
          Serial.println("  -- exit values");
          Serial.print("  password_highlightRow = ");
          Serial.println(password_highlightRow);
          Serial.print("  password_highlightCol = ");
          Serial.println(password_highlightCol);
          Serial.print("  password_topRow = ");
          Serial.println(password_topRow);
          Serial.print("  password_rowCount = ");
          Serial.println(password_rowCount);
          Serial.print("  password = ");
          Serial.println(password);
          Serial.println("  return = STATUS_ACCESSPOINTS_INIT");
        #endif
        
        return STATUS_ACCESSPOINTS_INIT;

    }
    
    if (password_highlightCol == PASSWORD_MENU_OPTION_DELETE) {

      if (password.length() > 0) { 
        
        password = password.substring(0, password.length() - 1); 
        renderEnterPassword();
        delay(100);

        #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_FINAL)
          Serial.println("  -- exit values");
          Serial.print("  password_highlightRow = ");
          Serial.println(password_highlightRow);
          Serial.print("  password_highlightCol = ");
          Serial.println(password_highlightCol);
          Serial.print("  password_topRow = ");
          Serial.println(password_topRow);
          Serial.print("  password_rowCount = ");
          Serial.println(password_rowCount);
          Serial.print("  password = ");
          Serial.println(password);
          Serial.println("  return = STATUS_ENTERPASSWORD_INIT");
        #endif

        return STATUS_ENTERPASSWORD_INIT;
        
      }

    }

    if (password_highlightCol == PASSWORD_MENU_OPTION_CONNECT) {

      #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_FINAL)
        Serial.println("  -- exit values");
        Serial.print("  password_highlightRow = ");
        Serial.println(password_highlightRow);
        Serial.print("  password_highlightCol = ");
        Serial.println(password_highlightCol);
        Serial.print("  password_topRow = ");
        Serial.println(password_topRow);
        Serial.print("  password_rowCount = ");
        Serial.println(password_rowCount);
        Serial.print("  password = ");
        Serial.println(password);
        Serial.println("  return = STATUS_CONNECT_INIT");
      #endif

      return STATUS_CONNECT_INIT;

    }
    
  }
  else {

    password = password + password_chars[(password_highlightRow * PASSWORD_CHARS_PER_ROW) + password_highlightCol];
    renderEnterPassword();
    delay(100);

    #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(DEBUG_PASSWORD_VALUES_FINAL)
      Serial.println("  -- exit values");
      Serial.print("  password_highlightRow = ");
      Serial.println(password_highlightRow);
      Serial.print("  password_highlightCol = ");
      Serial.println(password_highlightCol);
      Serial.print("  password_topRow = ");
      Serial.println(password_topRow);
      Serial.print("  password_rowCount = ");
      Serial.println(password_rowCount);
      Serial.print("  password = ");
      Serial.println(password);
      Serial.println("  return = STATUS_ENTERPASSWORD_INIT");
    #endif

    return STATUS_ENTERPASSWORD_INIT;

  }
  
}
