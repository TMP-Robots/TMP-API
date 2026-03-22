#include "TMP-API.h"

static const char* TAG = "TMP_API";

TMP_RobotServer::TMP_RobotServer() : TMP_RobotServer("TMP-ESP32", 80) {}

TMP_RobotServer::TMP_RobotServer(int port) : TMP_RobotServer("TMP-ESP32", port) {}

TMP_RobotServer::TMP_RobotServer(const char* hostname) : TMP_RobotServer(hostname, 80) {}

TMP_RobotServer::TMP_RobotServer(const char* hostname, int port) 
  : _server(port), _hostname(hostname) {}

void TMP_RobotServer::begin()
{
  _server.on("/", [this]() { _handleRoot(); });
  _server.on("/health", [this]() { _handleHealth(); });
  
  _server.onNotFound([this]() { _handleNotFound(); });

  _server.begin();
  _setup_OTA();
  ESP_LOGD(TAG, "Server started successfully");
}

void TMP_RobotServer::begin(const char* ssid, const char* pass, bool isAP)
{
  if (isAP) 
    _setupAP(ssid, pass);
  else
    _setupSTA(ssid, pass);
  
  begin();
}

void TMP_RobotServer::_setupAP(const char* ssid, const char* pass)
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

void TMP_RobotServer::_setupSTA(const char* ssid, const char* pass)
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

void TMP_RobotServer::_setup_OTA()
{
  ArduinoOTA.onStart([]() {
    ESP_LOGI(TAG, "Start updating...");
  });
  ArduinoOTA.onEnd([]() {
    ESP_LOGI(TAG, "\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    ESP_LOGI(TAG, "Progress: %u%%", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    ESP_LOGE(TAG, "Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      ESP_LOGE(TAG, "Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      ESP_LOGE(TAG, "Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      ESP_LOGE(TAG, "Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      ESP_LOGE(TAG, "Receive Failed");
    } else if (error == OTA_END_ERROR) {
      ESP_LOGE(TAG, "End Failed");
    }
  });
  ArduinoOTA.setHostname(_hostname);
  ArduinoOTA.begin();
  ESP_LOGI(TAG, "OTA ready");
}

void TMP_RobotServer::update() 
{ 
  _server.handleClient(); 
  ArduinoOTA.handle();
}

void TMP_RobotServer::_handleRoot()
{
  _server.send(200, "text/plain", "Hi");
  ESP_LOGD(TAG, "200 - Root accessed");
}

void TMP_RobotServer::_handleHealth()
{
  _server.send(200, "application/json", "{\"status\":\"ok\"}");
  ESP_LOGD(TAG, "200 - Status accessed");
}

void TMP_RobotServer::_handleNotFound()
{
  _server.send(404, "application/json", "{\"error\":\"not found\"}");
  ESP_LOGD(TAG, "404 - Not Found");
}
