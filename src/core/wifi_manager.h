#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "../config.h"
#include "logger.h"
#include "settings.h"

namespace tabos {

constexpr uint8_t MaxWifiNetworks = 8;

struct WifiNetwork {
  char ssid[33] = {0};
  int32_t rssi = 0;
  int32_t channel = 0;
  uint8_t encryption = 0;
};

class WifiManager {
 public:
  void begin(SettingsManager& settings, Logger& logger);
  void loop();
  void setEnabled(bool enabled);
  void toggleEnabled() { setEnabled(!enabled()); }
  bool enabled() const;

  bool requestScan();
  bool scanning() const { return _scanning; }
  int16_t lastScanResult() const { return _lastScanResult; }
  uint8_t networkCount() const { return _networkCount; }
  const WifiNetwork& network(uint8_t index) const;
  const char* ipAddressText() const { return _ipAddress; }
  const char* statusText() const;

 private:
  void applyRadioState();
  void finishScan(int16_t result);

  SettingsManager* _settings = nullptr;
  Logger* _logger = nullptr;
  WifiNetwork _networks[MaxWifiNetworks];
  uint8_t _networkCount = 0;
  int16_t _lastScanResult = 0;
  bool _scanning = false;
  uint32_t _scanStartedAt = 0;
  char _ipAddress[16] = "0.0.0.0";
};

}  // namespace tabos
