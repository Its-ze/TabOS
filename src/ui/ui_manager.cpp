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

  playTransition();

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
      if (event.type == InputEventType::TouchSwipeLeft) {
        startTransition(Transition::SlideLeft);
      } else if (event.type == InputEventType::TouchSwipeRight) {
        startTransition(Transition::SlideRight);
      } else if (event.type == InputEventType::TouchTap) {
        startTransition(Transition::OpenApp);
      }
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
      startTransition(Transition::BackHome);
      _apps->back();
      break;
    case NavAction::Home:
    case NavAction::Apps:
      startTransition(Transition::BackHome);
      _apps->home();
      break;
    case NavAction::Settings:
      startTransition(Transition::OpenApp);
      _apps->openById("settings");
      break;
    case NavAction::None:
      break;
  }
}

void UiManager::startTransition(Transition transition) {
  if (transition == Transition::None) {
    return;
  }
  _transition = transition;
}

void UiManager::playTransition() {
  if (_transition == Transition::None) {
    return;
  }

  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  const int16_t h = bottom - top;
  constexpr uint8_t Frames = 5;
  constexpr uint8_t FrameDelayMs = 14;

  for (uint8_t frame = 1; frame <= Frames; ++frame) {
    const int16_t span = (w * frame) / Frames;
    const uint32_t fill = (frame % 2 == 0) ? theme::Panel : theme::PanelAlt;

    switch (_transition) {
      case Transition::SlideLeft: {
        const int16_t x = w - span;
        d.fillRect(x, top, span, h, fill);
        d.drawLine(x, top, x, bottom, theme::Accent);
        break;
      }
      case Transition::SlideRight:
        d.fillRect(0, top, span, h, fill);
        d.drawLine(span, top, span, bottom, theme::Accent);
        break;
      case Transition::OpenApp: {
        const int16_t inset = ((Frames - frame) * 34);
        d.fillRoundRect(inset, top + inset / 3, w - inset * 2,
                        h - (inset * 2) / 3, 10, fill);
        d.drawRoundRect(inset, top + inset / 3, w - inset * 2,
                        h - (inset * 2) / 3, 10, theme::Accent);
        break;
      }
      case Transition::BackHome: {
        const int16_t y = bottom - (h * frame) / Frames;
        d.fillRect(0, y, w, bottom - y, fill);
        d.drawLine(0, y, w, y, theme::Accent2);
        break;
      }
      case Transition::None:
        break;
    }

    delay(FrameDelayMs);
  }

  _transition = Transition::None;
}

}  // namespace tabos
