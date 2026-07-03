#include "battery.h"

namespace tabos {

void BatteryManager::begin(Logger& logger) {
  _logger = &logger;
  batteryHal().begin(logger);
  _snapshot = batteryHal().read();
  _lastRefresh = millis();
  _logger->info("Battery manager started");
}

void BatteryManager::loop() {
  const uint32_t now = millis();
  if (now - _lastRefresh < config::BatteryRefreshMs) {
    return;
  }

  _lastRefresh = now;
  _snapshot = batteryHal().read();

  if (_snapshot.available && _snapshot.percent <= 20 && !_lowBatteryLogged) {
    _lowBatteryLogged = true;
    if (_logger) {
      _logger->warn("Low battery");
    }
  }

  if (_snapshot.available && _snapshot.percent > 25) {
    _lowBatteryLogged = false;
  }
}

}  // namespace tabos
