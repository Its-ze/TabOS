#include "wifi_manager.h"

#include <WiFi.h>
#include <string.h>

namespace tabos {

void WifiManager::begin(SettingsManager& settings, Logger& logger) {
  _settings = &settings;
  _logger = &logger;
  applyRadioState();
  _logger->info("Wi-Fi manager started");
}

void WifiManager::loop() {
  if (!_scanning) {
    return;
  }

  const int16_t result = WiFi.scanComplete();
  if (result >= 0 || result == WIFI_SCAN_FAILED) {
    finishScan(result);
  }

  if (millis() - _scanStartedAt > 15000) {
    _scanning = false;
    _lastScanResult = WIFI_SCAN_FAILED;
    WiFi.scanDelete();
    if (_logger) {
      _logger->warn("Wi-Fi scan timed out");
    }
  }
}

void WifiManager::setEnabled(bool enabled) {
  if (_settings == nullptr) {
    return;
  }

  if (_settings->data().wifiEnabled == enabled) {
    return;
  }

  _settings->edit().wifiEnabled = enabled;
  _settings->save();
  applyRadioState();
  if (_logger) {
    _logger->logf(LogLevel::Info, "Wi-Fi %s", enabled ? "enabled" : "disabled");
  }
}

bool WifiManager::enabled() const {
  return _settings && _settings->data().wifiEnabled;
}

bool WifiManager::requestScan() {
  if (!enabled()) {
    if (_logger) {
      _logger->warn("Wi-Fi scan skipped; radio disabled");
    }
    return false;
  }

  if (_scanning) {
    return true;
  }

  WiFi.scanDelete();
  const int16_t started = WiFi.scanNetworks(true, true);
  if (started == WIFI_SCAN_RUNNING || started == 0) {
    _scanning = true;
    _scanStartedAt = millis();
    _lastScanResult = WIFI_SCAN_RUNNING;
    if (_logger) {
      _logger->info("Wi-Fi scan started");
    }
    return true;
  }

  _lastScanResult = started;
  if (_logger) {
    _logger->warn("Wi-Fi scan failed to start");
  }
  return false;
}

const WifiNetwork& WifiManager::network(uint8_t index) const {
  static WifiNetwork empty;
  if (index >= _networkCount) {
    return empty;
  }
  return _networks[index];
}

const char* WifiManager::statusText() const {
  if (!enabled()) {
    return "Disabled";
  }
  if (_scanning) {
    return "Scanning";
  }
  if (WiFi.status() == WL_CONNECTED) {
    return "Connected";
  }
  return "Ready";
}

void WifiManager::applyRadioState() {
  _scanning = false;
  _networkCount = 0;
  WiFi.scanDelete();

  if (enabled()) {
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    IPAddress ip = WiFi.localIP();
    snprintf(_ipAddress, sizeof(_ipAddress), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  } else {
    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_OFF);
    strlcpy(_ipAddress, "0.0.0.0", sizeof(_ipAddress));
  }
}

void WifiManager::finishScan(int16_t result) {
  _scanning = false;
  _lastScanResult = result;
  _networkCount = 0;

  if (result < 0) {
    WiFi.scanDelete();
    if (_logger) {
      _logger->warn("Wi-Fi scan failed");
    }
    return;
  }

  const uint8_t count = min<uint8_t>(result, MaxWifiNetworks);
  for (uint8_t i = 0; i < count; ++i) {
    String ssid = WiFi.SSID(i);
    strlcpy(_networks[i].ssid, ssid.c_str(), sizeof(_networks[i].ssid));
    _networks[i].rssi = WiFi.RSSI(i);
    _networks[i].channel = WiFi.channel(i);
    _networks[i].encryption = static_cast<uint8_t>(WiFi.encryptionType(i));
  }
  _networkCount = count;
  WiFi.scanDelete();

  if (_logger) {
    _logger->logf(LogLevel::Info, "Wi-Fi scan found %d networks", result);
  }
}

}  // namespace tabos
