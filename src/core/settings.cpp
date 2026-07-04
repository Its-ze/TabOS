#include "settings.h"

#include <string.h>

namespace tabos {

namespace {
constexpr uint32_t SettingsSchema = 5;
}

bool SettingsManager::begin(Logger& logger) {
  _logger = &logger;
  _available = _prefs.begin("tabos", false);
  if (!_available) {
    resetDefaults();
    _logger->warn("Settings storage unavailable; using defaults");
    return false;
  }

  if (!load()) {
    resetDefaults();
    save();
    _logger->warn("Settings failed to load; defaults saved");
  } else {
    _logger->info("Settings loaded");
  }
  return true;
}

bool SettingsManager::load() {
  if (!_available) {
    return false;
  }

  if (_prefs.getUInt("schema", 0) != SettingsSchema) {
    return false;
  }

  _settings.brightness = _prefs.getUChar("brightness", _settings.brightness);
  _settings.dimBrightness = _prefs.getUChar("dimBright", _settings.dimBrightness);
  _settings.autoDim = _prefs.getBool("autoDim", _settings.autoDim);
  _settings.autoScreenOff = _prefs.getBool("autoOff", _settings.autoScreenOff);
  _settings.dimAfterMs = _prefs.getUInt("dimAfter", _settings.dimAfterMs);
  _settings.screenOffAfterMs = _prefs.getUInt("offAfter", _settings.screenOffAfterMs);
  _settings.keepAwakeWhileCharging =
      _prefs.getBool("chargeAwake", _settings.keepAwakeWhileCharging);
  _settings.allowAppsKeepAwake =
      _prefs.getBool("appAwake", _settings.allowAppsKeepAwake);
  _settings.batterySaver = _prefs.getBool("battSaver", _settings.batterySaver);
  _settings.autoRotate = _prefs.getBool("autoRot", _settings.autoRotate);
  _settings.wifiEnabled = _prefs.getBool("wifi", _settings.wifiEnabled);
  _settings.bluetoothEnabled = _prefs.getBool("ble", _settings.bluetoothEnabled);
  String name = _prefs.getString("name", TABOS_NAME);
  strlcpy(_settings.deviceName, name.c_str(), sizeof(_settings.deviceName));
  return true;
}

bool SettingsManager::save() {
  if (!_available) {
    if (_logger) {
      _logger->warn("Settings save skipped; storage unavailable");
    }
    return false;
  }

  _prefs.putUInt("schema", SettingsSchema);
  _prefs.putUChar("brightness", _settings.brightness);
  _prefs.putUChar("dimBright", _settings.dimBrightness);
  _prefs.putBool("autoDim", _settings.autoDim);
  _prefs.putBool("autoOff", _settings.autoScreenOff);
  _prefs.putUInt("dimAfter", _settings.dimAfterMs);
  _prefs.putUInt("offAfter", _settings.screenOffAfterMs);
  _prefs.putBool("chargeAwake", _settings.keepAwakeWhileCharging);
  _prefs.putBool("appAwake", _settings.allowAppsKeepAwake);
  _prefs.putBool("battSaver", _settings.batterySaver);
  _prefs.putBool("autoRot", _settings.autoRotate);
  _prefs.putBool("wifi", _settings.wifiEnabled);
  _prefs.putBool("ble", _settings.bluetoothEnabled);
  _prefs.putString("name", _settings.deviceName);
  if (_logger) {
    _logger->info("Settings saved");
  }
  return true;
}

void SettingsManager::resetDefaults() {
  _settings = SettingsData{};
  if (_logger) {
    _logger->info("Settings reset to defaults");
  }
}

}  // namespace tabos
