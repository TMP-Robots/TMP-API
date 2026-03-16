#include "TMP-API.h"

TMP_RobotServer::TMP_RobotServer(int port) : _server(port) {}

void TMP_RobotServer::begin(const char *ssid, const char *passwd) {

  WiFi.softAP(ssid, passwd);

  // routes config using lambdas to access class methods
  _server.on("/health", HTTP_GET, [this]() { this->handleHealth(); });
  _server.onNotFound([this]() { this->handleNotFound(); });

  _server.begin();
}

void TMP_RobotServer::update() { _server.handleClient(); }

void TMP_RobotServer::handleHealth() {
  _server.send(200, "application/json", "{\"status\":\"ok\"}");
  Serial.print("Status ok\n");
}

void TMP_RobotServer::handleNotFound() {
  _server.send(404, "application/json", "{\"error\":\"not found\"}");
  Serial.print("Ruta desconocida\n");
}
