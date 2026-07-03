#include "power_manager.h"

namespace tabos {

void PowerManager::begin(SettingsManager& settings, Logger& logger) {
  _settings = &settings;
  _logger = &logger;
  _lastInputAt = millis();
  restoreBrightness();
  _logger->info("Power manager started");
}

bool PowerManager::handleInput(const InputEvent& event) {
  if (!isTouchEvent(event.type) && event.type != InputEventType::ButtonBack &&
      event.type != InputEventType::ButtonHome) {
    return false;
  }

  _lastInputAt = millis();
  if (_screenOff) {
    wakeScreen();
    return true;
  }

  if (_dimmed) {
    restoreBrightness();
  }
  return false;
}

void PowerManager::loop(bool appKeepAwake, bool charging) {
  if (_settings == nullptr || _screenOff) {
    return;
  }

  const SettingsData& s = _settings->data();
  const uint32_t idle = millis() - _lastInputAt;
  const bool keepAwake =
      (s.allowAppsKeepAwake && appKeepAwake) ||
      (charging && s.keepAwakeWhileCharging);

  if (keepAwake) {
    if (_dimmed) {
      restoreBrightness();
    }
    return;
  }

  if (s.autoDim && !_dimmed && idle >= s.dimAfterMs) {
    dimScreen();
  }

  if (s.autoScreenOff && idle >= s.screenOffAfterMs) {
    turnScreenOff();
  }
}

void PowerManager::restoreBrightness() {
  if (_settings) {
    display().setBrightness(_settings->data().brightness);
  }
  _dimmed = false;
}

void PowerManager::dimScreen() {
  if (_settings) {
    display().setBrightness(_settings->data().dimBrightness);
  }
  _dimmed = true;
  if (_logger) {
    _logger->debug("Screen dimmed");
  }
}

void PowerManager::turnScreenOff() {
  display().fillScreen(0x000000);
  display().setBrightness(0);
  _screenOff = true;
  _dimmed = false;
  if (_logger) {
    _logger->info("Screen off");
  }
}

void PowerManager::wakeScreen() {
  _screenOff = false;
  restoreBrightness();
  if (_logger) {
    _logger->info("Screen woke");
  }
}

}  // namespace tabos
