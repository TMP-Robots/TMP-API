#ifndef TMP_API_H
#define TMP_API_H

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_log.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <map>
#include <functional>

class TMP_RobotServer {
public:
  TMP_RobotServer();
  TMP_RobotServer(int port);
  TMP_RobotServer(const char* hostname);
  TMP_RobotServer(const char* hostname, int port);

  /**
   * @brief Starts the server. Use this if WiFi is already connected manually.
   */
  void begin(); 

  /**
   * @brief Connects to WiFi (STA) or creates an Access Point (AP) and starts the server.
   * @param ssid Network Name.
   * @param pass Network Password.
   * @param isAP True for Access Point, False for Station (default).
   */
  void begin(const char* ssid, const char* pass, bool isAP = false);
  
  // OTA (Over-The-Air) updates
  void enableOTA();
  void enableOTA(const char* password);
  void disableOTA();
  
  /**
   * @brief Registers a numeric or boolean variable to be exposed via /config.
   * Supports: int, float, double, bool.
   * * @param key Unique name for the variable in the JSON API.
   * @param ref Reference to the variable in your main code.
   */
  template<typename T>
  void registerVar(const char* key, T& ref) {
    T* ptr = &ref;
    _getters[key] = [ptr]() -> String {
        return _varToString(*ptr);
    };
    _updaters[key] = [ptr](const String& val) {
        _varFromString(*ptr, val);
    };
  }

  /**
   * @brief Must be called in the main loop() to handle client requests and OTA.
   */
  void update();

private:
  const char* _hostname;
  WebServer _server;
  
  // Storage for the logic to read/write variables
  std::map<String, std::function<String()>>            _getters;
  std::map<String, std::function<void(const String&)>> _updaters;

  // Numerical & Boolean converters to String
  static String _varToString(uint8_t&  v) { return String(v); }
  static String _varToString(uint16_t& v) { return String(v); }
  static String _varToString(uint32_t& v) { return String(v); }
  static String _varToString(int8_t&   v) { return String(v); }
  static String _varToString(int16_t&  v) { return String(v); }
  static String _varToString(int32_t&  v) { return String(v); }
  static String _varToString(float&  v) { return String(v, 2); }
  static String _varToString(double& v) { return String(v, 2); }
  static String _varToString(bool&   v) { return v ? "true" : "false"; }

  // String converters back to Numerical & Boolean
  static void _varFromString(uint8_t&  v, const String& s) { v = (uint8_t)s.toInt(); }
  static void _varFromString(uint16_t& v, const String& s) { v = (uint16_t)s.toInt(); }
  static void _varFromString(uint32_t& v, const String& s) { v = (uint32_t)s.toInt(); }
  static void _varFromString(int8_t&   v, const String& s) { v = (int8_t)s.toInt(); }
  static void _varFromString(int16_t&  v, const String& s) { v = (int16_t)s.toInt(); }
  static void _varFromString(int32_t&  v, const String& s) { v = (int32_t)s.toInt(); }
  static void _varFromString(float&  v, const String& s) { v = s.toFloat(); }
  static void _varFromString(double& v, const String& s) { v = s.toDouble(); }
  static void _varFromString(bool&   v, const String& s) { 
      v = (s == "true" || s == "1" || s == "on"); 
  }

  bool _OTAEnabled = false;

  void _setupAP(const char* ssid, const char* pass);
  void _setupSTA(const char* ssid, const char* pass);
  void _setup_OTA();

  // Route handlers
  void _handleGetRoot();
  void _handleGetHealth();
  void _handleGetConfig();
  void _handlePostConfig();
  void _handleNotFound();
};

#endif