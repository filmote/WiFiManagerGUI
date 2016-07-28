
void renderAccessPoints_Retrieving() {

  display.clear();
  drawHorizontalLine(0);
  drawHorizontalLine(63);
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setColor(WHITE);
  display.drawString(ACCESSPOINT_PROGRESS_MESSAGE_TEXT_LEFT, ACCESSPOINT_PROGRESS_MESSAGE_TEXT_TOP, "Retrieving Access Points ..");
  display.display();

}

void renderAccessPoints() {

  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(ACCESSPOINT_HEADING_TEXT_LEFT, ACCESSPOINT_HEADING_TEXT_TOP, "Choose a Network (" + String(accessPoint_count) + ") :");
    

  // Render highlight ..

  if (accessPoint_highlightRow >= 0) {
    
    display.setColor(WHITE); 
    display.fillRect(ACCESSPOINT_HIGHLIGHT_ROW_LEFT, ACCESSPOINT_HIGHLIGHT_ROW_TOP + ((accessPoint_highlightRow - accessPoint_topRow) * ACCESSPOINT_ROW_HEIGHT), ACCESSPOINT_HIGHLIGHT_ROW_WIDTH, ACCESSPOINT_ROW_HEIGHT);
    
  }


  // Render access points ..

  int accessPointsToRender = (accessPoint_count > ACCESSPOINT_ROWS_VISIBLE ? ACCESSPOINT_ROWS_VISIBLE : accessPoint_count);

  if (accessPointsToRender > 0) {
      
    for (int i = 0; i < accessPointsToRender; i++) { 
  
      if ((accessPoint_highlightRow - accessPoint_topRow) == i) { display.setColor(BLACK); }   
      display.drawString(1, ACCESSPOINT_HIGHLIGHT_ROW_TEXT_TOP + (ACCESSPOINT_ROW_HEIGHT * i), accessPoints[accessPoint_topRow + i]);    
      if ((accessPoint_highlightRow - accessPoint_topRow) == i) { display.setColor(WHITE); }
    
    }
  
  }
  else {


    // Render a message that no access points were found ..

    display.setColor(WHITE);
    drawExclamation(ACCESSPOINT_ERROR_MESSAGE_IMAGE_LEFT, ACCESSPOINT_ERROR_MESSAGE_IMAGE_TOP);
    display.drawString(ACCESSPOINT_ERROR_MESSAGE_TEXT_LEFT, ACCESSPOINT_ERROR_MESSAGE_TEXT_TOP, "No access points found.");

  }


  // Render scroll bar if needed ..
  
  if (accessPoint_count > ACCESSPOINT_ROWS_VISIBLE) {

    drawScrollBar(accessPoint_count, ACCESSPOINT_ROWS_VISIBLE, accessPoint_topRow);

  }


  // Render menu options ..
  
  if (accessPoint_highlightRow == ACCESSPOINT_ROW_MENU) {
    display.fillRect(ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_LEFT, ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_TOP, ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_WIDTH, ACCESSPOINT_MENU_REFRESH_HIGHLIGHT_HEIGHT);
    display.setColor(BLACK);
  }

  drawWiFiSymbol(ACCESSPOINT_MENU_REFRESH_IMAGE_LEFT, ACCESSPOINT_MENU_REFRESH_IMAGE_TOP);
  display.drawString(ACCESSPOINT_MENU_REFRESH_TEXT_LEFT, ACCESSPOINT_MENU_REFRESH_TEXT_TOP, "Refresh");    
  display.setColor(WHITE);
 
  display.display();
  
}

