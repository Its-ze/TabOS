#include "app_battery.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

BatteryApp::BatteryApp(BatteryManager& battery) : _battery(battery) {}

void BatteryApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  const BatterySnapshot& b = _battery.snapshot();

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("Battery", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const int16_t x = 38;
  const int16_t y = top + 118;
  const int16_t cardW = w - 76;
  d.fillRoundRect(x, y, cardW, 180, 8, theme::Panel);
  d.drawRoundRect(x, y, cardW, 180, 8, theme::Border);

  char percent[24];
  if (b.available) {
    snprintf(percent, sizeof(percent), "%d%%", b.percent);
  } else {
    snprintf(percent, sizeof(percent), "--%%");
  }
  d.drawText(percent, x + 34, y + 56, 4, theme::Accent, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawProgress(x + 34, y + 116, cardW - 68, 28,
                 b.available ? static_cast<uint8_t>(b.percent) : 0,
                 theme::Accent, theme::Track, theme::Border);

  char meta[80];
  snprintf(meta, sizeof(meta), "Voltage: %d mV    Charging: %s",
           b.millivolts, b.charging ? "yes" : "no");
  d.drawText(meta, x + 34, y + 160, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
}

}  // namespace tabos
