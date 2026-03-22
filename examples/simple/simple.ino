#include <Arduino.h>
#include <TMP-API.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

TMP_RobotServer robotServer(80);

void setup()
{
  #ifdef DEBUG_MODE
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(4000); // Wait for Serial to initialize
  #endif
  
  /* Change the last parameter to true if you want the ESP32 to create its own
   * Access Point instead of connecting to an existing WiFi network */
  robotServer.begin(ssid, password, false); 
  
  // If you want to enable OTA updates
  robotServer.enableOTA();
  /* can also be enabled later in the code with robotServer.enableOTA() or 
   * disabled with robotServer.disableOTA() 
   */
}

void loop() 
{
  robotServer.update();
}