#include "app_about.h"

#include <Arduino.h>
#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

void AboutApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("About", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const int16_t x = 38;
  const int16_t y = top + 112;
  const int16_t cardW = w - 76;
  d.fillRoundRect(x, y, cardW, 238, 8, theme::Panel);
  d.drawRoundRect(x, y, cardW, 238, 8, theme::Border);

  char line[96];
  snprintf(line, sizeof(line), "%s  %s", TABOS_NAME, TABOS_VERSION);
  d.drawText(line, x + 28, y + 42, 4, theme::Accent, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText("Build: " TABOS_BUILD_DATE, x + 28, y + 96, 2, theme::Text,
             theme::Panel, TextAlign::MiddleLeft);
  d.drawText("Target: M5Stack Tab5 / ESP32-P4", x + 28, y + 134, 2,
             theme::MutedText, theme::Panel, TextAlign::MiddleLeft);

  snprintf(line, sizeof(line), "Display: %dx%d    Free heap: %lu",
           d.width(), d.height(), static_cast<unsigned long>(ESP.getFreeHeap()));
  d.drawText(line, x + 28, y + 172, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);

  snprintf(line, sizeof(line), "Uptime: %lu sec",
           static_cast<unsigned long>(millis() / 1000));
  d.drawText(line, x + 28, y + 210, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
}

}  // namespace tabos
