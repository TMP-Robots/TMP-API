#ifndef TMP_API_H
#define TMP_API_H

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_log.h>
#include <ArduinoOTA.h>

class TMP_RobotServer {
public:
  TMP_RobotServer();
  TMP_RobotServer(int port);
  TMP_RobotServer(const char* hostname);
  TMP_RobotServer(const char* hostname, int port);

  /**
   * @brief Initializes the server without managing the WiFi connection.
   * Use this if you have already connected the ESP32 to a network manually.
   */
  void begin(); 

  /**
   * @brief Connects to WiFi and starts the server in a single step.
   * * @param ssid The Name (SSID) of the WiFi network or the Access Point to create.
   * @param pass The Password for the network.
   * @param isAP If true, the device will act as an Access Point. 
   * If false (default), it will connect to an existing network as a station.
   */
  void begin(const char* ssid, const char* pass, bool isAP = false);

  void update();

private:
  const char* _hostname;
  WebServer _server;

  void _setupAP(const char* ssid, const char* pass);
  void _setupSTA(const char* ssid, const char* pass);
  void _setup_OTA();

  void _handleRoot();
  void _handleHealth();
  void _handleNotFound();
};

#endif
