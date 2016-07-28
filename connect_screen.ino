void renderConnect(bool drawButtons) {

  display.displayOn();
  display.clear();
  display.setFont(ArialMT_Plain_10);

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 2, connect_line1_text);
    display.drawString(0, 13, connect_line2_text);
    display.drawString(0, 24, connect_line3_text);
    display.drawString(0, 35, connect_line4_text);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 46, connect_line5_text);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
  
    drawHorizontalLine(0);

    if (!drawButtons) {
      drawHorizontalLine(63);
    }
    else {


      // Back button ..
    
      if (connect_highlightCol == CONNECT_MENU_OPTION_BACK) { //Highlighted ..
        display.fillRect(CONNECT_MENU_BACK_HIGHLIGHT_LEFT, CONNECT_MENU_BACK_HIGHLIGHT_TOP, CONNECT_MENU_BACK_HIGHLIGHT_WIDTH, CONNECT_MENU_BACK_HIGHLIGHT_HEIGHT);
        display.setColor(BLACK);
      }
    
      drawArrowLeft(CONNECT_MENU_BACK_IMAGE_LEFT, CONNECT_MENU_BACK_IMAGE_TOP);
      display.drawString(CONNECT_MENU_BACK_TEXT_LEFT, CONNECT_MENU_BACK_TEXT_TOP, "Back");    
      display.setColor(WHITE);


      // Retry button .. 
    
      if (connect_highlightCol == CONNECT_MENU_OPTION_RETRY) { //Highlighted ..
        display.fillRect(CONNECT_MENU_RETRY_HIGHLIGHT_LEFT, CONNECT_MENU_RETRY_HIGHLIGHT_TOP, CONNECT_MENU_RETRY_HIGHLIGHT_WIDTH, CONNECT_MENU_RETRY_HIGHLIGHT_HEIGHT);
        display.setColor(BLACK);
      }
    
      display.drawString(CONNECT_MENU_RETRY_TEXT_LEFT, CONNECT_MENU_RETRY_TEXT_TOP, "Retry");    
      display.setColor(WHITE);


      // Dividers ..

      display.drawRect(CONNECT_MENU_DIVIDER1_LEFT, CONNECT_MENU_DIVIDERS_TOP, 0, CONNECT_MENU_DIVIDERS_HEIGHT);
  
    }

  display.display();

}

