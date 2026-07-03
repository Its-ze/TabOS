#include "quick_settings.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "theme.h"

namespace tabos {

namespace {
constexpr int16_t ShadeTop = config::StatusBarHeight;
constexpr int16_t ShadeHeight = 306;
constexpr int16_t TileW = 180;
constexpr int16_t TileH = 70;
constexpr int16_t Gap = 16;

struct Rect {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
};

Rect toggleRect(uint8_t index) {
  const int16_t margin = 28;
  const uint8_t col = index % 3;
  const uint8_t row = index / 3;
  return Rect{static_cast<int16_t>(margin + col * (TileW + Gap)),
              static_cast<int16_t>(ShadeTop + 72 + row * (TileH + Gap)),
              TileW, TileH};
}
}  // namespace

void QuickSettings::begin(SettingsManager& settings, WifiManager& wifi,
                          BluetoothManager& bluetooth, Logger& logger) {
  _settings = &settings;
  _wifi = &wifi;
  _bluetooth = &bluetooth;
  _logger = &logger;
}

void QuickSettings::open() {
  _open = true;
  if (_logger) {
    _logger->debug("Quick settings opened");
  }
}

void QuickSettings::close() {
  _open = false;
}

void QuickSettings::draw() {
  if (!_open || _settings == nullptr || _wifi == nullptr || _bluetooth == nullptr) {
    return;
  }

  HalDisplay& d = display();
  const int16_t w = d.width();
  d.fillRect(0, ShadeTop, w, ShadeHeight, 0x0B1512);
  d.drawLine(0, ShadeTop + ShadeHeight - 1, w, ShadeTop + ShadeHeight - 1,
             theme::Border);
  d.drawText("Quick Settings", 28, ShadeTop + 34, 4, theme::Text, 0x0B1512,
             TextAlign::MiddleLeft);
  d.drawText("Swipe up or tap below to close", w - 28, ShadeTop + 34, 2,
             theme::MutedText, 0x0B1512, TextAlign::MiddleRight);

  char brightness[12];
  snprintf(brightness, sizeof(brightness), "%u", _settings->data().brightness);

  drawToggle(0, "Wi-Fi", _wifi->enabled() ? "On" : "Off", _wifi->enabled());
  drawToggle(1, "Bluetooth", _bluetooth->enabled() ? "On" : "Off",
             _bluetooth->enabled());
  drawToggle(2, "Brightness", brightness, true);
  drawToggle(3, "Saver", _settings->data().batterySaver ? "On" : "Off",
             _settings->data().batterySaver);
  drawToggle(4, "Rotate", "Locked", false);
  drawToggle(5, "Close", "Hide", false);
}

bool QuickSettings::handleInput(const InputEvent& event) {
  if (!_open) {
    return false;
  }

  if (event.type == InputEventType::TouchSwipeUp) {
    close();
    return true;
  }

  if (event.type != InputEventType::TouchTap) {
    return true;
  }

  if (event.y > ShadeTop + ShadeHeight) {
    close();
    return true;
  }

  for (uint8_t i = 0; i < 6; ++i) {
    const Rect r = toggleRect(i);
    if (event.x >= r.x && event.x < r.x + r.w &&
        event.y >= r.y && event.y < r.y + r.h) {
      handleToggle(i);
      return true;
    }
  }

  return true;
}

void QuickSettings::drawToggle(uint8_t index, const char* label, const char* value,
                               bool active) {
  HalDisplay& d = display();
  const Rect r = toggleRect(index);
  const uint32_t fill = active ? theme::PanelAlt : theme::Panel;
  const uint32_t valueColor = active ? theme::Accent : theme::MutedText;

  d.fillRoundRect(r.x, r.y, r.w, r.h, 8, fill);
  d.drawRoundRect(r.x, r.y, r.w, r.h, 8, active ? theme::Accent : theme::Border);
  d.drawText(label, r.x + 16, r.y + 22, 2, theme::Text, fill,
             TextAlign::MiddleLeft);
  d.drawText(value, r.x + 16, r.y + 50, 2, valueColor, fill,
             TextAlign::MiddleLeft);
}

void QuickSettings::handleToggle(uint8_t index) {
  if (_settings == nullptr || _wifi == nullptr || _bluetooth == nullptr) {
    return;
  }

  SettingsData& s = _settings->edit();
  switch (index) {
    case 0:
      _wifi->toggleEnabled();
      break;
    case 1:
      _bluetooth->toggleEnabled();
      break;
    case 2:
      cycleBrightness();
      break;
    case 3:
      s.batterySaver = !s.batterySaver;
      _settings->save();
      break;
    case 4:
      s.autoRotate = false;
      _settings->save();
      break;
    case 5:
      close();
      break;
  }
}

void QuickSettings::cycleBrightness() {
  SettingsData& s = _settings->edit();
  if (s.brightness < 96) {
    s.brightness = 128;
  } else if (s.brightness < 176) {
    s.brightness = 192;
  } else if (s.brightness < 230) {
    s.brightness = 255;
  } else {
    s.brightness = 64;
  }
  display().setBrightness(s.brightness);
  _settings->save();
}

}  // namespace tabos
