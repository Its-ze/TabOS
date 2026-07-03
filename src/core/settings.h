#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <stdint.h>

#include "logger.h"

namespace tabos {

struct SettingsData {
  uint8_t brightness = 192;
  uint8_t dimBrightness = 42;
  bool autoDim = true;
  bool autoScreenOff = true;
  uint32_t dimAfterMs = 15000;
  uint32_t screenOffAfterMs = 60000;
  bool keepAwakeWhileCharging = true;
  bool allowAppsKeepAwake = true;
  bool batterySaver = false;
  bool autoRotate = false;
  bool wifiEnabled = false;
  bool bluetoothEnabled = false;
  char deviceName[20] = TABOS_NAME;
};

class SettingsManager {
 public:
  bool begin(Logger& logger);
  bool load();
  bool save();
  void resetDefaults();

  const SettingsData& data() const { return _settings; }
  SettingsData& edit() { return _settings; }
  bool available() const { return _available; }

 private:
  Preferences _prefs;
  SettingsData _settings;
  Logger* _logger = nullptr;
  bool _available = false;
};

}  // namespace tabos
