void renderEnterPassword() {
  
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.drawString(PASSWORD_KEY_LABEL_LEFT, PASSWORD_KEY_LABEL_TOP, "Key : ");    
  display.drawString(36, 0, password);    
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
