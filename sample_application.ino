// Sample application ..


int init_application() {

  status = STATUS_APPLICATION_LOOP;
  return loop_application();

}


// -------------------------------------------------------------------------------------------------------
//  Process user input ..
//

int loop_application() {

  while (status == STATUS_APPLICATION_LOOP) {


    // Should we turn the screen off automatically ?
  
    #if defined(CONNECT_DIM_AFTER_CONNECTION)
      
      if (connect_dimAfterConnetionDelay > 0) {

        connect_dimAfterConnetionDelay--;
  
        if (connect_dimAfterConnetionDelay == 0) {
  
          display.displayOff();
            
        }
        
      }
    #endif




    // Is the connection still valid ?  Should we attempt to reconnect ??

    #if defined(CONNECT_RETRY_AUTOMATICALLY)
      
      if (WiFi.status() != WL_CONNECTED) {
        
        for (int i = 0; i < CONNECT_RETRY_ATTEMPTS; i++) {

          wifiStatus = connectSilently();

          if (wifiStatus == WL_CONNECTED) break;

        }
      
      }


      // If unable to connect to the network, return to the connection phase and try again ..
      
      if (wifiStatus != WL_CONNECTED) return STATUS_CONNECT_INIT;
      
    #endif




    // Do stuff ..
    
    delay(100);



  }
  
}

