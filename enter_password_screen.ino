void renderEnterPassword() {

  String password_display = "";                     // With all characters bar the last replaced with *

  #if defined(DEBUG) && defined(DEBUG_PASSWORD)
    Serial.println("");
    Serial.println("renderEnterPassword()");
  #endif


  // Substitute characters for * in the password? 
  
  #if defined(PASSWORD_HIDE_CHARACTERS)
  
    password_display = "";

    #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(PASSWORD_HIDE_CHARACTERS)
      Serial.print("  Clear text password: ");
      Serial.print(password);
      Serial.print("  (length ");
      Serial.print(password.length());
      Serial.print(" chars)");
    #endif

    if (password.length() > 0) {
    
      for (int i = 0; i < password.length() - 1; i++) {
  
        password_display = password_display + "*";
        
        #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(PASSWORD_HIDE_CHARACTERS)
          Serial.print("  ");
          Serial.println(password_display);
        #endif
          
      }
      
      password_display = password_display + password.substring(password.length() - 1);
      
      #if defined(DEBUG) && defined(DEBUG_PASSWORD) && defined(PASSWORD_HIDE_CHARACTERS)
        Serial.print("  ");
        Serial.println(password_display);
      #endif
      
    }

  #else

    password_display = password;

  #endif


  // Is the password too long to display?  If so remove the first character fromthe password until it fits.  
  // Final password will be rendered with '..' in front to indicate that there is more letters to the left ..

  if (display.getStringWidth(password_display) + 4 > PASSWORD_KEY_BORDER_WIDTH) {

    password_display = truncateString(password_display, (PASSWORD_KEY_BORDER_WIDTH - 4), true);

  }
  
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.drawString(PASSWORD_KEY_LABEL_LEFT, PASSWORD_KEY_LABEL_TOP, "Key : ");   
  display.drawString(36, 0, password_display);  
  display.drawRect(PASSWORD_KEY_BORDER_LEFT, PASSWORD_KEY_BORDER_TOP, PASSWORD_KEY_BORDER_WIDTH, PASSWORD_KEY_BORDER_HEIGHT);


  // Render three password character rows ..
  
  for (int i = 0; i < PASSWORD_CHARS_PER_ROW; i++) {
    if (password_topRow == password_highlightRow && i == password_highlightCol) { 
      display.fillRect(PASSWORD_HIGHLIGHT_LEFT + (i * PASSWORD_CHAR_SPACING), PASSWORD_ROW_1_TOP + 1, PASSWORD_HIGHLIGHT_SIZE, PASSWORD_HIGHLIGHT_SIZE);
      display.setColor(BLACK);
    }
    display.drawString(PASSWORD_CHAR_LEFT + (i * PASSWORD_CHAR_SPACING), PASSWORD_ROW_1_TOP, String(password_chars[(password_topRow * PASSWORD_CHARS_PER_ROW) + i]));    
    display.setColor(WHITE);
  }
  
  for (int i = 0; i < PASSWORD_CHARS_PER_ROW; i++) {
    if ((password_topRow + 1) == password_highlightRow && i == password_highlightCol) { 
      display.fillRect(PASSWORD_HIGHLIGHT_LEFT + (i * PASSWORD_CHAR_SPACING), PASSWORD_ROW_2_TOP + 1, PASSWORD_HIGHLIGHT_SIZE, PASSWORD_HIGHLIGHT_SIZE);
      display.setColor(BLACK);
    }
    display.drawString(PASSWORD_CHAR_LEFT + (i * PASSWORD_CHAR_SPACING), PASSWORD_ROW_2_TOP, String(password_chars[((password_topRow + 1) * PASSWORD_CHARS_PER_ROW) + i]));    
    display.setColor(WHITE);
  }

  if (password_topRow + 2 == password_rowCount - 1) {
    for (int i = 0; i < password_charsOnLastRow; i++) {
      if ((password_topRow + 2) == password_highlightRow && i == password_highlightCol) { 
        display.fillRect(PASSWORD_HIGHLIGHT_LEFT + (i * PASSWORD_CHAR_SPACING), PASSWORD_ROW_3_TOP + 1, PASSWORD_HIGHLIGHT_SIZE, PASSWORD_HIGHLIGHT_SIZE);
        display.setColor(BLACK);
      }
      display.drawString(PASSWORD_CHAR_LEFT + (i * PASSWORD_CHAR_SPACING), PASSWORD_ROW_3_TOP, String(password_chars[((password_topRow + 2) * PASSWORD_CHARS_PER_ROW) + i]));    
      display.setColor(WHITE);
    }
  }
  else {
    for (int i = 0; i < PASSWORD_CHARS_PER_ROW; i++) {
      if ((password_topRow + 2) == password_highlightRow && i == password_highlightCol) { 
        display.fillRect(PASSWORD_HIGHLIGHT_LEFT + (i * PASSWORD_CHAR_SPACING), PASSWORD_ROW_3_TOP + 1, PASSWORD_HIGHLIGHT_SIZE, PASSWORD_HIGHLIGHT_SIZE);
        display.setColor(BLACK);
      }
      display.drawString(PASSWORD_CHAR_LEFT + (i * PASSWORD_CHAR_SPACING), PASSWORD_ROW_3_TOP, String(password_chars[((password_topRow + 2) * PASSWORD_CHARS_PER_ROW) + i]));    
      display.setColor(WHITE);
    }
  }


  // Back button ..

  if (password_highlightRow == PASSWORD_MENU_ROW && password_highlightCol == 0) { //Highlighted ..
    display.fillRect(PASSWORD_MENU_BACK_HIGHLIGHT_LEFT, PASSWORD_MENU_BACK_HIGHLIGHT_TOP, PASSWORD_MENU_BACK_HIGHLIGHT_WIDTH, PASSWORD_MENU_BACK_HIGHLIGHT_HEIGHT);
    display.setColor(BLACK);
  }

  drawArrowLeft(PASSWORD_MENU_BACK_IMAGE_LEFT, PASSWORD_MENU_BACK_IMAGE_TOP);
  display.drawString(PASSWORD_MENU_BACK_TEXT_LEFT, PASSWORD_MENU_BACK_TEXT_TOP, "Back");    
  display.setColor(WHITE);


  // Delete character button ..
  
  if (password_highlightRow == -1 && password_highlightCol == 1) { //Highlighted ..
    display.fillRect(PASSWORD_MENU_DELETE_HIGHLIGHT_LEFT, PASSWORD_MENU_DELETE_HIGHLIGHT_TOP, PASSWORD_MENU_DELETE_HIGHLIGHT_WIDTH, PASSWORD_MENU_DELETE_HIGHLIGHT_HEIGHT);
    display.setColor(BLACK);
  }
  
  display.drawString(PASSWORD_MENU_DELETE_TEXT_LEFT, PASSWORD_MENU_DELETE_TEXT_TOP, "Delete");    
  display.setColor(WHITE);


  // Connect button ..

  if (password_highlightRow == -1 && password_highlightCol == 2) { //Highlighted ..
    display.fillRect(PASSWORD_MENU_CONNECT_HIGHLIGHT_LEFT, PASSWORD_MENU_CONNECT_HIGHLIGHT_TOP, PASSWORD_MENU_CONNECT_HIGHLIGHT_WIDTH, PASSWORD_MENU_CONNECT_HIGHLIGHT_HEIGHT);
    display.setColor(BLACK);
  }
  
  display.drawString(PASSWORD_MENU_CONNECT_TEXT_LEFT, PASSWORD_MENU_CONNECT_TEXT_TOP, "Connect");    
  display.setColor(WHITE);


  // Dividers ..

  display.drawRect(PASSWORD_MENU_DIVIDER1_LEFT, PASSWORD_MENU_DIVIDERS_TOP, 0, PASSWORD_MENU_DIVIDERS_HEIGHT);
  display.drawRect(PASSWORD_MENU_DIVIDER2_LEFT, PASSWORD_MENU_DIVIDERS_TOP, 0, PASSWORD_MENU_DIVIDERS_HEIGHT);


  // Render scroll bar ..

  drawScrollBar(password_rowCount, PASSWORD_ROWS_VISIBLE, password_topRow);
  
  display.display();

}
