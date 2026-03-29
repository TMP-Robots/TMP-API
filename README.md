# TMP-API 🤖

**Status: ⚠️ UNDER ACTIVE DEVELOPMENT** This library is currently a development 
playground. Features and API signatures are subject to change.

## Overview
**TMP-API** is a lightweight ESP32 library designed for robotics developers who 
need to tune parameters (PID gains, speeds, offsets, colors) in real-time without 
recompiling. It automatically exposes C++ variables through a **JSON-based HTTP API** 
and includes built-in **OTA (Over-The-Air)** update support.

---

## Features
- [x] **Automatic Variable Binding:** Bind `int8_t/32_t`, `uint8_t/32_t`, 
  `float`, `double` and `bool` variables via pointers/lambdas.
- [x] **RESTful API:** Basic endpoints to `GET` and `POST` configuration values 
  in JSON format.
- [x] **OTA Updates:** Integrated ArduinoOTA for wireless firmware flashing.
- [x] **Dual Mode WiFi:** Support for both Station (STA) and Access Point (AP) modes.
- [x] **S3 Compatibility:** Tested and optimized for ESP32-S3 DevKitC-1.
- [x] **ESP Logging Integration:** Optional debug logging with adjustable log levels.
- [x] **Non-Blocking:** Designed to be called within the main `loop()` with 
  minimal overhead.
- [x] **Basic OTA Authentication:** Simple password protection for OTA updates.
- [ ] **API Authentication:** Basic token-based authentication for API endpoints.
- [ ] **Telemetry over websockets:** Real-time telemetry streaming.

---

## 🚀 Quick Start (TEST-API Example)

The [`TEST-API` repository](https://github.com/TMP-Robots/TEST-API) serves as 
the primary development testbed. It controls a NeoPixel LED using variables 
synced over the network.
