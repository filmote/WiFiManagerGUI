
// -------------------------------------------------------------------------------------------------------
// Draw an up arrow as part of the scroll bar (5 x 3 pixels) ..

void drawArrowUp(int left, int top) {

  display.setPixel(left + 2, top);
  display.drawRect(left + 1, top + 1, 3, 0);
  display.drawRect(left, top + 2, 5, 0);
    
}


// -------------------------------------------------------------------------------------------------------
// Draw a down arrow as part of the scroll bar (5 x 3 pixels) ..

void drawArrowDown(int left, int top) {

  display.drawRect(left, top, 5, 0);
  display.drawRect(left + 1, top + 1, 3, 0);
  display.setPixel(left + 2, top + 2);

}


// -------------------------------------------------------------------------------------------------------
// Draw a left arrow (4 x 7 pixels) ..

void drawArrowLeft(int left, int top) {
 
  display.setPixel(left, top + 3);
  display.drawRect(left + 1, top + 2, 0, 3);
  display.drawRect(left + 2, top + 1, 0, 5);
  display.drawRect(left + 3, top, 0, 7);

}


// -------------------------------------------------------------------------------------------------------
// Draw a dotted bar as part of the scroll bar (5 x 1 pixels) ..

void drawDottedBar(int left, int top) {

  display.setPixel(left, top);
  display.setPixel(left + 2, top);
  display.setPixel(left + 4, top);

}


// -------------------------------------------------------------------------------------------------------
// Draw a WiFi symbol (5 x 5 pixels) ..

void drawWiFiSymbol(int left, int top) {
  
  display.setPixel(left + 2, top + 4);
  display.drawRect(left + 1, top + 2, 3, 0);
  display.drawRect(left, top, 5, 0);

}


// -------------------------------------------------------------------------------------------------------
// Draw an exclamation mark for use with error or warning messages (8 x 8 pixels) ..

void drawExclamation(int left, int top) {

  display.drawRect(left + 2, top, 4, 0);
  display.drawRect(left + 1, top + 1, 2, 0);
  display.drawRect(left + 5, top + 1, 2, 0);
  display.fillRect(left, top + 2, 3, 4);
  display.fillRect(left + 5, top + 2, 3, 4);
  display.drawRect(left + 1, top + 6, 2, 0);
  display.drawRect(left + 3, top + 5, 2, 0);
  display.drawRect(left + 5, top + 6, 2, 0);
  display.drawRect(left + 2, top + 7, 4, 0);
  
}


// -------------------------------------------------------------------------------------------------------
// Draw a scroll bar with appropriately sized and positioned slider ..

void drawScrollBar(int itemCount, int itemsVisible, int topRow) {
  
  display.setColor(BLACK);
  display.fillRect(SCROLLBAR_AREA_LEFT, SCROLLBAR_AREA_TOP, SCROLLBAR_AREA_WIDTH, SCROLLBAR_AREA_HEIGHT);
  display.setColor(WHITE);
  display.drawRect(SCROLLBAR_BORDER_LEFT, SCROLLBAR_BORDER_TOP, SCROLLBAR_BORDER_WIDTH, SCROLLBAR_BORDER_HEIGHT);

  
  // Draw a top arrow ?

  if (topRow > 0) {

    drawArrowUp(SCROLLBAR_SLIDER_LEFT, SCROLLBAR_INDICATOR_UP_TOP);
    
  }
  else {
    
    drawDottedBar(SCROLLBAR_SLIDER_LEFT, SCROLLBAR_INDICATOR_UP_TOP + 3);
    
  }
  
  
  // Draw a bottom arrow ?

  if (topRow + itemsVisible < itemCount) {

    drawArrowDown(SCROLLBAR_SLIDER_LEFT, SCROLLBAR_INDICATOR_DOWN_TOP);
    
  }
  else {

    drawDottedBar(SCROLLBAR_SLIDER_LEFT, SCROLLBAR_INDICATOR_DOWN_TOP);
    
  }

  if (itemCount > itemsVisible) {

    int heightPerUnit = (SCROLLBAR_SLIDE_MAX_HEIGHT) / itemCount;
    int top = topRow * heightPerUnit;
    int height = itemsVisible * heightPerUnit;
    display.fillRect(SCROLLBAR_SLIDER_LEFT, SCROLLBAR_SLIDER_TOP + top, SCROLLBAR_SLIDER_WIDTH, height);
    
  }

}


// -------------------------------------------------------------------------------------------------------
// Draw a horizontal line across entire screen ..

void drawHorizontalLine(int top) {

  display.drawRect(0, top, 127, 0);
  
}


// -------------------------------------------------------------------------------------------------------
// Truncate a string to Xpx in width by removing characters from the left or the right.  The final string 
// is returned with an ellipses (..) pre-pended or appended to the shortened string.

String truncateString(String longString, int width, bool leftTruncate) {

  String shortString = longString;
       
  #if defined(DEBUG) 
    Serial.println("  ");
    Serial.println("truncateString()");
    Serial.println("  Orig string = " + longString);
    Serial.print("  Orig width = ");
    Serial.println(display.getStringWidth(longString));
    Serial.print("  Truncate to = ");
    Serial.println(width);
  #endif

  if (display.getStringWidth(longString) > width) {
    
    while (display.getStringWidth("..") + display.getStringWidth(shortString) > width) {

      if (leftTruncate) {
        shortString = shortString.substring(1);
      }
      else {
        shortString = shortString.substring(0, shortString.length() - 2);
      }
      
      #if defined(DEBUG) 
        Serial.print("  ");
        Serial.println(shortString);
      #endif

    }
      
    if (leftTruncate) {
      shortString = ".." + shortString;
    }
    else {
      shortString = shortString + "..";
    }
      
    #if defined(DEBUG) && defined(DEBUG_PASSWORD)
      Serial.print("  ");
      Serial.println(shortString);
    #endif

    return shortString;
    
  }
  else {

    return longString;
    
  }
  
}
