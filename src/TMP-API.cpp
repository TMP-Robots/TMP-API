#include "TMP-API.h"

TMP_RobotServer::TMP_RobotServer(Client& client, int port) 
  : _client(&client), _server(port) { }


void TMP_RobotServer::begin()
{
  _server.on("/", [this]() { handleRoot(); });
  _server.on("/health", [this]() { handleHealth(); });
  
  _server.onNotFound([this]() { handleNotFound(); });

  _server.begin();
  Serial.println("API: Server started successfully");
}


void TMP_RobotServer::update() { _server.handleClient(); }

void TMP_RobotServer::handleRoot()
{
  _server.send(200, "text/plain", "Hi");
  Serial.print("API: root endpoint accessed\n");
}

void TMP_RobotServer::handleHealth()
{
  _server.send(200, "application/json", "{\"status\":\"ok\"}");
  Serial.print("API: status ok\n");
}

void TMP_RobotServer::handleNotFound()
{
  _server.send(404, "application/json", "{\"error\":\"not found\"}");
  Serial.print("API: not found\n");
}
