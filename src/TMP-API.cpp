#include "TMP-API.h"

static const char* TAG = "TMP_API";

TMP_RobotServer::TMP_RobotServer(int port) 
  : _server(port) { }

void TMP_RobotServer::begin()
{
  _server.on("/", [this]() { handleRoot(); });
  _server.on("/health", [this]() { handleHealth(); });
  
  _server.onNotFound([this]() { handleNotFound(); });

  _server.begin();
  ESP_LOGD(TAG, "Server started successfully");
}

void TMP_RobotServer::begin(const char* ssid, const char* pass, bool isAP)
{
  if (isAP) 
    setupAP(ssid, pass);
  else
    setupSTA(ssid, pass);
  
  begin();
}

void TMP_RobotServer::setupAP(const char* ssid, const char* pass)
{
  ESP_LOGD(TAG, "Configuring Access Point...");
  WiFi.mode(WIFI_AP); 
  
  if (WiFi.softAP(ssid, pass))
  {
    ESP_LOGI(TAG, "AP Created! SSID: %s", ssid);
    ESP_LOGD(TAG, "IP: %s", WiFi.softAPIP().toString().c_str());
  } 
  else 
  {
    ESP_LOGE(TAG, "Failed to create Access Point");
  }
}

void TMP_RobotServer::setupSTA(const char* ssid, const char* pass)
{
  ESP_LOGI(TAG, "Connecting to Station: %s", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  int attempts = 0;
  const int maxAttempts = 20; // 20 attempts with 500ms delay = 10 secs timeout

  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) 
  {
    delay(500);
    attempts++;
    ESP_LOGV(TAG, "[%d/%d] Waiting for WiFi...", attempts, maxAttempts);
  }

  if (WiFi.status() == WL_CONNECTED) 
  {
    ESP_LOGI(TAG, "WiFi connected! IP: %s", WiFi.localIP().toString().c_str());
  } 
  else 
  {
    ESP_LOGE(TAG, "WiFi connection FAILED after %d attempts.", maxAttempts);
  }
}

void TMP_RobotServer::update() { _server.handleClient(); }

void TMP_RobotServer::handleRoot()
{
  _server.send(200, "text/plain", "Hi");
  ESP_LOGD(TAG, "200 - Root accessed");
}

void TMP_RobotServer::handleHealth()
{
  _server.send(200, "application/json", "{\"status\":\"ok\"}");
  ESP_LOGD(TAG, "200 - Status accessed");
}

void TMP_RobotServer::handleNotFound()
{
  _server.send(404, "application/json", "{\"error\":\"not found\"}");
  ESP_LOGD(TAG, "404 - Not Found");
}
