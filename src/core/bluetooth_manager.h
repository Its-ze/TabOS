#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "logger.h"
#include "settings.h"

namespace tabos {

constexpr uint8_t MaxBleDevices = 8;

struct BleDeviceInfo {
  char name[32] = {0};
  char address[24] = {0};
  int rssi = 0;
};

class BluetoothManager {
 public:
  void begin(SettingsManager& settings, Logger& logger);
  void loop();
  void setEnabled(bool enabled);
  void toggleEnabled() { setEnabled(!enabled()); }
  bool enabled() const;
  bool supported() const { return _supported; }

  bool requestScan();
  bool scanning() const { return _scanning; }
  uint8_t deviceCount() const { return _deviceCount; }
  const BleDeviceInfo& device(uint8_t index) const;
  const char* statusText() const;

  void addDevice(const char* name, const char* address, int rssi);
  void markScanComplete();

 private:
  void clearDevices();

  SettingsManager* _settings = nullptr;
  Logger* _logger = nullptr;
  BleDeviceInfo _devices[MaxBleDevices];
  uint8_t _deviceCount = 0;
  bool _supported = false;
  bool _initialized = false;
  bool _scanning = false;
  uint32_t _scanStartedAt = 0;
};

BluetoothManager* bluetoothManagerInstance();

}  // namespace tabos
