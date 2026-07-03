#pragma once

#include <Arduino.h>

#include "../hal/hal_display.h"
#include "input_event.h"
#include "logger.h"
#include "settings.h"

namespace tabos {

class PowerManager {
 public:
  void begin(SettingsManager& settings, Logger& logger);
  bool handleInput(const InputEvent& event);
  void loop(bool appKeepAwake, bool charging);

  bool isScreenOff() const { return _screenOff; }
  bool isDimmed() const { return _dimmed; }
  uint32_t lastInputAt() const { return _lastInputAt; }

 private:
  void restoreBrightness();
  void dimScreen();
  void turnScreenOff();
  void wakeScreen();

  SettingsManager* _settings = nullptr;
  Logger* _logger = nullptr;
  uint32_t _lastInputAt = 0;
  bool _screenOff = false;
  bool _dimmed = false;
};

}  // namespace tabos
