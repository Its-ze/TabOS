#include "app_power.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

namespace {
struct Rect {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
};

Rect toggleRect(uint8_t index) {
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38 + (index % 2) * 250;
  const int16_t y = top + 188 + (index / 2) * 82;
  return Rect{x, y, 220, 62};
}
}  // namespace

PowerApp::PowerApp(SettingsManager& settings, PowerManager& power,
                   BatteryManager& battery, Logger& logger)
    : _settings(settings), _power(power), _battery(battery), _logger(logger) {}

void PowerApp::draw() {
  HalDisplay& d = display();
  const int16_t top = config::StatusBarHeight;
  d.fillRect(0, top, d.width(), d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("PW", 36, top + 52, 4, theme::Accent, theme::Background,
             TextAlign::MiddleLeft);
  d.drawText("Power", 112, top + 52, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const BatterySnapshot& b = _battery.snapshot();
  char line[96];
  snprintf(line, sizeof(line), "Battery %s%d%%   Brightness %u   Idle %lus",
           b.charging ? "+" : "", b.available ? b.percent : 0,
           _settings.data().brightness,
           static_cast<unsigned long>((millis() - _power.lastInputAt()) / 1000));

  d.fillRoundRect(38, top + 108, d.width() - 76, 58, 8, theme::Panel);
  d.drawText(line, 62, top + 137, 2, theme::Text, theme::Panel,
             TextAlign::MiddleLeft);

  const SettingsData& s = _settings.data();
  drawToggle(0, "Auto dim", s.autoDim ? "On" : "Off", s.autoDim);
  drawToggle(1, "Screen off", s.autoScreenOff ? "On" : "Off", s.autoScreenOff);
  drawToggle(2, "Charge awake", s.keepAwakeWhileCharging ? "On" : "Off",
             s.keepAwakeWhileCharging);
  drawToggle(3, "Brightness", "Cycle", true);
}

void PowerApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }

  SettingsData& s = _settings.edit();
  for (uint8_t i = 0; i < 4; ++i) {
    const Rect r = toggleRect(i);
    if (event.x >= r.x && event.x < r.x + r.w &&
        event.y >= r.y && event.y < r.y + r.h) {
      if (i == 0) {
        s.autoDim = !s.autoDim;
      } else if (i == 1) {
        s.autoScreenOff = !s.autoScreenOff;
      } else if (i == 2) {
        s.keepAwakeWhileCharging = !s.keepAwakeWhileCharging;
      } else {
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
      }
      _settings.save();
      _logger.info("Power setting changed");
      return;
    }
  }
}

void PowerApp::drawToggle(uint8_t index, const char* label, const char* value,
                          bool active) {
  HalDisplay& d = display();
  const Rect r = toggleRect(index);
  const uint32_t fill = active ? theme::PanelAlt : theme::Panel;
  d.fillRoundRect(r.x, r.y, r.w, r.h, 8, fill);
  d.drawRoundRect(r.x, r.y, r.w, r.h, 8, active ? theme::Accent : theme::Border);
  d.drawText(label, r.x + 18, r.y + 20, 2, theme::Text, fill,
             TextAlign::MiddleLeft);
  d.drawText(value, r.x + 18, r.y + 46, 2,
             active ? theme::Accent : theme::MutedText, fill,
             TextAlign::MiddleLeft);
}

}  // namespace tabos
