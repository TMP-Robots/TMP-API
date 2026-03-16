#ifndef TMP_API_H
#define TMP_API_H

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

class TMP_RobotServer {
public:
  // Constructor: recibe el puerto del servidor
  TMP_RobotServer(int port = 80);

  // Configura el WiFi y los endpoints
  void begin(const char *ssid = "ESP32-Hotspot",
             const char *password = "1234567890");

  // llamar en el loop() del main
  void update();

private:
  WebServer _server;

  // Manejadores de rutas (deben ser internos)
  void handleHealth();
  void handleNotFound();
};

#endif // TMP_API_H
