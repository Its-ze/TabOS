#pragma once

#include <Arduino.h>

#include "../config.h"
#include "../hal/hal_battery.h"
#include "logger.h"

namespace tabos {

class BatteryManager {
 public:
  void begin(Logger& logger);
  void loop();

  const BatterySnapshot& snapshot() const { return _snapshot; }
  bool isCharging() const { return _snapshot.charging; }
  bool isExternalPowered() const { return _snapshot.externalPower; }
  int8_t percent() const { return _snapshot.percent; }
  bool available() const { return _snapshot.available; }

 private:
  Logger* _logger = nullptr;
  BatterySnapshot _snapshot;
  uint32_t _lastRefresh = 0;
  bool _lowBatteryLogged = false;
};

}  // namespace tabos
