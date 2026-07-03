#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "../core/logger.h"

namespace tabos {

struct BatterySnapshot {
  bool available = false;
  bool charging = false;
  bool externalPower = false;
  int8_t percent = -1;
  int16_t millivolts = 0;
  int16_t vbusMillivolts = 0;
};

class HalBattery {
 public:
  bool begin(Logger& logger);
  BatterySnapshot read();
};

HalBattery& batteryHal();

}  // namespace tabos
