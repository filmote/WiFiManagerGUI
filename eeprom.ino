// -------------------------------------------------------------------------------------------------------
// Test to see if valid SSID and password settings have been saved previously (test to see if the byte 
// at position EEPROM_TEST_CHAR_POS equals the EEPROM_TEST_CHAR_POS_VALUE)..

bool validEEPROMSettings() {

  char testChar = EEPROM.read(EEPROM_TEST_CHAR_POS);
  
  #if defined(DEBUG) && defined(DEBUG_EEPROM)
    Serial.println(" ");
    Serial.println("validEEPROMSettings()"); 
    Serial.print("  testChar = ");
    Serial.println(testChar, DEC); 
    Serial.print("  (testChar == EEPROM_TEST_CHAR_POS_VALUE) = ");
    Serial.println(testChar == EEPROM_TEST_CHAR_POS_VALUE);
  #endif
   
  return (testChar == EEPROM_TEST_CHAR_POS_VALUE);
  
}


// -------------------------------------------------------------------------------------------------------
// Update the EEPROM_TEST_CHAR_POS byte with the value of EEPROM_TEST_CHAR_POS_VALUE to indicate that 
// the SSID and password have been saveed previously.

void writeTestChar() {

  EEPROM.write(EEPROM_TEST_CHAR_POS, EEPROM_TEST_CHAR_POS_VALUE);

  #if defined(DEBUG) && defined(DEBUG_EEPROM)

    char testChar = EEPROM.read(EEPROM_TEST_CHAR_POS);

    Serial.println(" ");
    Serial.println("writeTestChar()"); 
    Serial.print("  testChar = "); 
    Serial.println(EEPROM.read(EEPROM_TEST_CHAR_POS), DEC);
    Serial.print("  (testChar == EEPROM_TEST_CHAR_POS_VALUE) = ");
    Serial.println(testChar == EEPROM_TEST_CHAR_POS_VALUE ? "true" : "false");    
  #endif

}


// -------------------------------------------------------------------------------------------------------
// Clear the EEPROM_TEST_CHAR_POS byte that is used to indicate that a valid SSID and password have been 
// saveed previously.

void clearTestChar() {

  EEPROM.write(EEPROM_TEST_CHAR_POS, 0);

  #if defined(DEBUG) && defined(DEBUG_EEPROM) 
    Serial.println(" ");
    Serial.println("clearTestChar()"); 
    Serial.print("  testChar = "); 
    Serial.println(EEPROM.read(EEPROM_TEST_CHAR_POS), DEC);
  #endif

}


// -------------------------------------------------------------------------------------------------------
// Read the length of a previously saved SSID value.  The length is stored BEFORE the actual value in 
// byte EEPROM_SSID_LENGTH_POS.

int readSSIDLength() {

  #if defined(DEBUG) && defined(DEBUG_EEPROM)
    Serial.println(" ");
    Serial.println("readSSIDLength()"); 
    Serial.print("  length = "); 
    Serial.println(EEPROM.read(EEPROM_SSID_LENGTH_POS));
  #endif
  
  return (EEPROM.read(EEPROM_SSID_LENGTH_POS) > EEPROM_SSID_MAX_LENGTH ? EEPROM_SSID_MAX_LENGTH : EEPROM.read(EEPROM_SSID_LENGTH_POS));
  
}


// -------------------------------------------------------------------------------------------------------
// Read a previously saved SSID value from position EEPROM_SSID_START_POS.  The length of the value is
// stored immediately before the value in position EEPROM_SSID_LENGTH_POS.

String readSSIDValue() {
  
  String value = "";
  int ssidLength = readSSIDLength();
  
  for (int i = EEPROM_SSID_START_POS; i < EEPROM_SSID_START_POS + ssidLength; i++) {

    value += char(EEPROM.read(i));
    
  }

  #if defined(DEBUG) && defined(DEBUG_EEPROM)
    Serial.println(" ");
    Serial.println("readSSIDValue()"); 
    Serial.print("  value = "); 
    Serial.println(value);
  #endif

  return value;
  
}


// -------------------------------------------------------------------------------------------------------
// Write the SSID value starting at position EEPROM_SSID_START_POS.  
//
// Note: Function truncates SSID values in excess of EEPROM_SSID_MAX_LENGTH.

void writeSSIDValue(String value) {
    
  EEPROM.write(EEPROM_SSID_LENGTH_POS, (value.length() > EEPROM_SSID_MAX_LENGTH ? EEPROM_SSID_MAX_LENGTH : value.length()));

  for (int i = EEPROM_SSID_START_POS; i < EEPROM_SSID_START_POS + (value.length() > EEPROM_SSID_MAX_LENGTH ? EEPROM_SSID_MAX_LENGTH : value.length()); i++) {

    EEPROM.write(i, value[i - EEPROM_SSID_START_POS]);
    
  }
  
  #if defined(DEBUG) && defined(DEBUG_EEPROM)

    String testValue = "";
    int ssidLength = value.length();
  
    for (int i = EEPROM_SSID_START_POS; i < EEPROM_SSID_START_POS + ssidLength; i++) {

      testValue += char(EEPROM.read(i));
    
    }
    
    Serial.println(" ");
    Serial.println("writeSSIDValue()"); 
    Serial.print("  length = ");
    Serial.println(EEPROM.read(EEPROM_SSID_LENGTH_POS));
    Serial.print("  value = "); 
    Serial.println(testValue);
    
  #endif
  
}


// -------------------------------------------------------------------------------------------------------
// Read the length of a previously saved password value.  The length is stored BEFORE the actual value in 
// byte EEPROM_PASSWORD_LENGTH_POS.

int readPasswordLength() {
  
  #if defined(DEBUG) && defined(DEBUG_EEPROM)
    Serial.println(" ");
    Serial.println("readPasswordLength()"); 
    Serial.print("  length = "); 
    Serial.println(EEPROM.read(EEPROM_PASSWORD_LENGTH_POS));
  #endif
  
  return (EEPROM.read(EEPROM_PASSWORD_LENGTH_POS) > EEPROM_PASSWORD_MAX_LENGTH ? EEPROM_PASSWORD_MAX_LENGTH : EEPROM.read(EEPROM_PASSWORD_LENGTH_POS));
  
}


// -------------------------------------------------------------------------------------------------------
// Read a previously saved SSID value from position EEPROM_PASSWORD_START_POS.  The length of the value is
// stored immediately before the value in position EEPROM_PASSWORD_LENGTH_POS.

String readPasswordValue() {
  
  String value = "";
  int passwordLength = readPasswordLength();
  
  for (int i = EEPROM_PASSWORD_START_POS; i < EEPROM_PASSWORD_START_POS + passwordLength; i++) {

    value += char(EEPROM.read(i));
    
  }
  
  #if defined(DEBUG) && defined(DEBUG_EEPROM)
    Serial.println(" ");
    Serial.println("readPasswordValue()"); 
    Serial.print("  value = "); 
    Serial.println(value);
  #endif
  
  return value;
  
}


// -------------------------------------------------------------------------------------------------------
// Write the password value starting at position EEPROM_PASSWORD_START_POS.  
//
// Note: Function truncates passwords in excess of EEPROM_PASSWORD_MAX_LENGTH.

void writePasswordValue(String value) {
  
  EEPROM.write(EEPROM_PASSWORD_LENGTH_POS, (value.length() > EEPROM_PASSWORD_MAX_LENGTH ? EEPROM_PASSWORD_MAX_LENGTH : value.length()));

  for (int i = EEPROM_PASSWORD_START_POS; i < EEPROM_PASSWORD_START_POS + (value.length() > EEPROM_PASSWORD_MAX_LENGTH ? EEPROM_PASSWORD_MAX_LENGTH : value.length()); i++) {

    EEPROM.write(i, value[i - EEPROM_PASSWORD_START_POS]);
    
  }
    
  #if defined(DEBUG) && defined(DEBUG_EEPROM)

    String testValue = "";
    int passwordLength = (value.length() > EEPROM_PASSWORD_MAX_LENGTH ? EEPROM_PASSWORD_MAX_LENGTH : value.length());
  
    for (int i = EEPROM_PASSWORD_START_POS; i < EEPROM_PASSWORD_START_POS + passwordLength; i++) {

      testValue += char(EEPROM.read(i));
    
    }
    
    Serial.println(" ");
    Serial.println("writePasswordValue()"); 
    Serial.print("  length = ");
    Serial.println(EEPROM.read(EEPROM_PASSWORD_LENGTH_POS));
    Serial.print("  value = "); 
    Serial.println(testValue);
    
  #endif
  
}


// -------------------------------------------------------------------------------------------------------
// Write test character, SSID and password to EEPROM ..

void writeSSIDandPassword(String ssidVal, String passwordVal) {
  
    writeTestChar();
    writeSSIDValue(ssidVal);  
    writePasswordValue(passwordVal);  
    EEPROM.commit();

}


// -------------------------------------------------------------------------------------------------------
// Dump the first 512 bytes of the EEPROM in a table ..  code copied from EEPROMUtils library.

#if defined(DEBUG) && defined(DEBUG_EEPROM)
void dumpEEPROMValues() {

  Serial.println(" ");
  Serial.println("-------------------------------------------------------------");
  
  int bytesPerRow = 16;
  // address counter
  int i;

  // row bytes counter
  int j;

  // byte read from eeprom
  byte b;

  // temporary buffer for sprintf
  char buf[10];


  // initialize row counter
  j = 0;

  // go from first to last eeprom address
  for (i = 0; i <= 511; i++) {

    // if this is the first byte of the row,
    // start row by printing the byte address
    if (j == 0) {
      sprintf(buf, "%03X: ", i);
      Serial.print(buf);
    }

    // read current byte from eeprom
    b = EEPROM.read(i);

    // write byte in hex form
    sprintf(buf, "%02X ", b);

    // increment row counter
    j++;

    // if this is the last byte of the row,
    // reset row counter and use println()
    // to start a new line
    if (j == bytesPerRow) {
      j = 0;
      Serial.println(buf);
    }
    // else just print the hex value with print()
    else {
      Serial.print(buf);
    }
  }
}
#endif
