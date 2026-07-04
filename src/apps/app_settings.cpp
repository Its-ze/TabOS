#include "app_settings.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

namespace {
constexpr int16_t RowStartY = config::StatusBarHeight + 104;
constexpr int16_t RowHeight = 72;
constexpr int16_t RowGap = 10;
}

SettingsApp::SettingsApp(SettingsManager& settings, Logger& logger)
    : _settings(settings), _logger(logger) {}

void SettingsApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const SettingsData& s = _settings.data();

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("Settings", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  char brightness[20];
  snprintf(brightness, sizeof(brightness), "%u", s.brightness);

  drawRow(0, "Brightness", brightness, true);
  drawRow(1, "Auto dim", s.autoDim ? "On" : "Off", false);
  drawRow(2, "Screen off", s.autoScreenOff ? "On" : "Off", false);
  drawRow(3, "Battery saver", s.batterySaver ? "On" : "Off", false);
  drawRow(4, "Auto flip", s.autoRotate ? "On" : "Off", false);
  drawRow(5, "Reset defaults", "Tap to reset", false);
}

void SettingsApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }

  if (event.y < RowStartY) {
    return;
  }

  const int row = (event.y - RowStartY) / (RowHeight + RowGap);
  SettingsData& s = _settings.edit();

  switch (row) {
    case 0:
      cycleBrightness();
      break;
    case 1:
      s.autoDim = !s.autoDim;
      _settings.save();
      break;
    case 2:
      s.autoScreenOff = !s.autoScreenOff;
      _settings.save();
      break;
    case 3:
      s.batterySaver = !s.batterySaver;
      _settings.save();
      break;
    case 4:
      s.autoRotate = !s.autoRotate;
      _settings.save();
      break;
    case 5:
      _settings.resetDefaults();
      _settings.save();
      display().setBrightness(_settings.data().brightness);
      break;
    default:
      return;
  }
}

void SettingsApp::drawRow(uint8_t row, const char* label, const char* value,
                          bool accent) {
  HalDisplay& d = display();
  const int16_t x = 34;
  const int16_t y = RowStartY + row * (RowHeight + RowGap);
  const int16_t w = d.width() - 68;
  const uint32_t fill = accent ? theme::PanelAlt : theme::Panel;

  d.fillRoundRect(x, y, w, RowHeight, 8, fill);
  d.drawRoundRect(x, y, w, RowHeight, 8, theme::Border);
  d.drawText(label, x + 22, y + RowHeight / 2, 2, theme::Text, fill,
             TextAlign::MiddleLeft);
  d.drawText(value, x + w - 22, y + RowHeight / 2, 2,
             accent ? theme::Accent : theme::MutedText, fill, TextAlign::MiddleRight);
}

void SettingsApp::cycleBrightness() {
  uint8_t& b = _settings.edit().brightness;
  if (b < 96) {
    b = 128;
  } else if (b < 176) {
    b = 192;
  } else if (b < 230) {
    b = 255;
  } else {
    b = 64;
  }
  display().setBrightness(b);
  _settings.save();
  _logger.logf(LogLevel::Info, "Brightness set to %u", b);
}

}  // namespace tabos
