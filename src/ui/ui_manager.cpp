#include "ui_manager.h"

#include <Arduino.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "theme.h"

namespace tabos {

void UiManager::begin(AppManager& apps, SettingsManager& settings,
                      BatteryManager& battery, WifiManager& wifi,
                      BluetoothManager& bluetooth, MeshManager& mesh,
                      Logger& logger) {
  _apps = &apps;
  _settings = &settings;
  _battery = &battery;
  _mesh = &mesh;
  _quickSettings.begin(settings, wifi, bluetooth, logger);
  _dirty = true;
}

void UiManager::loop() {
  const uint32_t now = millis();
  if (_dirty) {
    draw();
    return;
  }

  if (now - _lastStatusDraw >= config::StatusRefreshMs) {
    if (_apps && !_apps->isLauncherActive()) {
      draw();
    } else {
      _statusBar.draw(*_battery, *_settings, *_mesh, currentTitle());
      if (_quickSettings.isOpen()) {
        _quickSettings.draw();
      }
    }
    _lastStatusDraw = now;
  }
}

void UiManager::draw() {
  if (_apps == nullptr || _settings == nullptr || _battery == nullptr ||
      _mesh == nullptr) {
    return;
  }

  if (_apps->isLauncherActive()) {
    _launcher.draw(*_apps);
  } else if (_apps->activeApp()) {
    _apps->activeApp()->draw();
  }

  _statusBar.draw(*_battery, *_settings, *_mesh, currentTitle());
  _bottomNav.draw(_apps->isLauncherActive());
  if (_quickSettings.isOpen()) {
    _quickSettings.draw();
  }
  _lastStatusDraw = millis();
  _dirty = false;
}

void UiManager::handleInput(const InputEvent& event) {
  if (_apps == nullptr) {
    return;
  }

  if (event.type == InputEventType::TouchSwipeDown) {
    _quickSettings.open();
    _dirty = true;
    return;
  }

  if (_quickSettings.isOpen()) {
    if (_quickSettings.handleInput(event)) {
      _dirty = true;
      return;
    }
  }

  const NavAction nav = _bottomNav.hitTest(event);
  if (nav != NavAction::None) {
    handleNav(nav);
    _dirty = true;
    return;
  }

  if (_apps->isLauncherActive()) {
    if (_launcher.handleInput(event, *_apps)) {
      _dirty = true;
    }
    return;
  }

  AppBase* app = _apps->activeApp();
  if (app) {
    app->handleInput(event);
    _dirty = true;
  }
}

const char* UiManager::currentTitle() const {
  if (_apps == nullptr || _apps->isLauncherActive() || _apps->activeApp() == nullptr) {
    return "Launcher";
  }
  return _apps->activeApp()->name();
}

void UiManager::handleNav(NavAction action) {
  switch (action) {
    case NavAction::Back:
      _apps->back();
      break;
    case NavAction::Home:
    case NavAction::Apps:
      _apps->home();
      break;
    case NavAction::Settings:
      _apps->openById("settings");
      break;
    case NavAction::None:
      break;
  }
}

}  // namespace tabos
