#include "hal_battery.h"

#include <M5Unified.h>

namespace tabos {

namespace {
HalBattery gBattery;
}

HalBattery& batteryHal() {
  return gBattery;
}

bool HalBattery::begin(Logger& logger) {
  logger.info("Battery HAL ready");
  return true;
}

BatterySnapshot HalBattery::read() {
  BatterySnapshot snapshot;

  const int32_t level = M5.Power.getBatteryLevel();
  if (level >= 0 && level <= 100) {
    snapshot.available = true;
    snapshot.percent = static_cast<int8_t>(level);
  }

  snapshot.millivolts = M5.Power.getBatteryVoltage();
  snapshot.vbusMillivolts = M5.Power.getVBUSVoltage();
  snapshot.charging = static_cast<int>(M5.Power.isCharging()) == 1;
  snapshot.externalPower = snapshot.charging || snapshot.vbusMillivolts > 4200;
  return snapshot;
}

}  // namespace tabos
