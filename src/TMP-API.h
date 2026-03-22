#ifndef TMP_API_H
#define TMP_API_H

#include <Arduino.h>
#include <Client.h>
#include <WebServer.h>

class TMP_RobotServer {
public:
  TMP_RobotServer(Client& client, int port = 80);

  void begin(); 
  void update();

private:
  Client* _client;
  WebServer _server;

  void handleRoot();
  void handleHealth();
  void handleNotFound();
};

#endif
