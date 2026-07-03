#include "bluetooth_manager.h"

#include <string>
#include <string.h>

#if __has_include(<BLEDevice.h>) && __has_include(<BLEScan.h>) && __has_include(<BLEAdvertisedDevice.h>)
#define TABOS_HAS_BLE_LIB 1
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#else
#define TABOS_HAS_BLE_LIB 0
#endif

namespace tabos {

namespace {
BluetoothManager* gBluetoothManager = nullptr;

#if TABOS_HAS_BLE_LIB
class TabOsBleCallbacks : public BLEAdvertisedDeviceCallbacks {
 public:
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    if (gBluetoothManager == nullptr) {
      return;
    }

    std::string name = advertisedDevice.haveName()
                           ? advertisedDevice.getName()
                           : std::string("(unnamed)");
    std::string address = advertisedDevice.getAddress().toString();
    gBluetoothManager->addDevice(name.c_str(), address.c_str(),
                                 advertisedDevice.getRSSI());
  }
};

void scanComplete(BLEScanResults results) {
  (void)results;
  if (gBluetoothManager) {
    gBluetoothManager->markScanComplete();
  }
}

TabOsBleCallbacks gBleCallbacks;
#endif
}  // namespace

BluetoothManager* bluetoothManagerInstance() {
  return gBluetoothManager;
}

void BluetoothManager::begin(SettingsManager& settings, Logger& logger) {
  _settings = &settings;
  _logger = &logger;
  gBluetoothManager = this;
  _supported = TABOS_HAS_BLE_LIB;
  if (_logger) {
    _logger->logf(LogLevel::Info, "Bluetooth manager %s",
                  _supported ? "started" : "unsupported");
  }
}

void BluetoothManager::loop() {
  if (_scanning && millis() - _scanStartedAt > 15000) {
    _scanning = false;
#if TABOS_HAS_BLE_LIB
    BLEDevice::getScan()->stop();
#endif
    if (_logger) {
      _logger->warn("BLE scan timed out");
    }
  }
}

void BluetoothManager::setEnabled(bool enabled) {
  if (_settings == nullptr) {
    return;
  }

  if (_settings->data().bluetoothEnabled == enabled) {
    return;
  }

  _settings->edit().bluetoothEnabled = enabled;
  _settings->save();
  if (!enabled) {
    _scanning = false;
    clearDevices();
#if TABOS_HAS_BLE_LIB
    if (_initialized) {
      BLEDevice::deinit(false);
      _initialized = false;
    }
#endif
  }

  if (_logger) {
    _logger->logf(LogLevel::Info, "Bluetooth %s", enabled ? "enabled" : "disabled");
  }
}

bool BluetoothManager::enabled() const {
  return _settings && _settings->data().bluetoothEnabled;
}

bool BluetoothManager::requestScan() {
  if (!enabled()) {
    if (_logger) {
      _logger->warn("BLE scan skipped; radio disabled");
    }
    return false;
  }

  if (!_supported) {
    if (_logger) {
      _logger->warn("BLE scan unsupported by this build");
    }
    return false;
  }

  if (_scanning) {
    return true;
  }

#if TABOS_HAS_BLE_LIB
  if (!_initialized) {
    BLEDevice::init("TabOS");
    _initialized = true;
  }

  clearDevices();
  BLEScan* scan = BLEDevice::getScan();
  scan->setAdvertisedDeviceCallbacks(&gBleCallbacks);
  scan->setActiveScan(true);
  scan->setInterval(100);
  scan->setWindow(80);
  _scanning = scan->start(5, scanComplete, false);
  _scanStartedAt = millis();
  if (_logger) {
    _logger->logf(LogLevel::Info, "BLE scan %s", _scanning ? "started" : "failed");
  }
  return _scanning;
#else
  return false;
#endif
}

const BleDeviceInfo& BluetoothManager::device(uint8_t index) const {
  static BleDeviceInfo empty;
  if (index >= _deviceCount) {
    return empty;
  }
  return _devices[index];
}

const char* BluetoothManager::statusText() const {
  if (!_supported) {
    return "Unsupported";
  }
  if (!enabled()) {
    return "Disabled";
  }
  if (_scanning) {
    return "Scanning";
  }
  return "Ready";
}

void BluetoothManager::addDevice(const char* name, const char* address, int rssi) {
  for (uint8_t i = 0; i < _deviceCount; ++i) {
    if (strncmp(_devices[i].address, address, sizeof(_devices[i].address)) == 0) {
      _devices[i].rssi = rssi;
      return;
    }
  }

  if (_deviceCount >= MaxBleDevices) {
    return;
  }

  BleDeviceInfo& slot = _devices[_deviceCount++];
  strlcpy(slot.name, name && name[0] ? name : "(unnamed)", sizeof(slot.name));
  strlcpy(slot.address, address ? address : "--", sizeof(slot.address));
  slot.rssi = rssi;
}

void BluetoothManager::markScanComplete() {
  _scanning = false;
#if TABOS_HAS_BLE_LIB
  BLEDevice::getScan()->clearResults();
#endif
  if (_logger) {
    _logger->logf(LogLevel::Info, "BLE scan found %u devices", _deviceCount);
  }
}

void BluetoothManager::clearDevices() {
  memset(_devices, 0, sizeof(_devices));
  _deviceCount = 0;
}

}  // namespace tabos
