#include "app_developer.h"

#include <Arduino.h>
#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

DeveloperApp::DeveloperApp(AppManager& apps, Logger& logger)
    : _apps(apps), _logger(logger) {}

void DeveloperApp::draw() {
  HalDisplay& d = display();
  const int16_t top = config::StatusBarHeight;
  d.fillRect(0, top, d.width(), d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("DV", 36, top + 52, 4, theme::Accent, theme::Background,
             TextAlign::MiddleLeft);
  d.drawText("Developer", 112, top + 52, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const int16_t x = 38;
  const int16_t y = top + 112;
  const int16_t w = d.width() - 76;
  d.fillRoundRect(x, y, w, 214, 8, theme::Panel);
  d.drawRoundRect(x, y, w, 214, 8, theme::Border);

  char line[96];
  snprintf(line, sizeof(line), "%s %s", TABOS_NAME, TABOS_VERSION);
  d.drawText(line, x + 24, y + 34, 2, theme::Accent, theme::Panel,
             TextAlign::MiddleLeft);
  snprintf(line, sizeof(line), "Apps: %u   Logs: %u   Uptime: %lus",
           static_cast<unsigned>(_apps.appCount()),
           static_cast<unsigned>(_logger.count()),
           static_cast<unsigned long>(millis() / 1000));
  d.drawText(line, x + 24, y + 76, 2, theme::Text, theme::Panel,
             TextAlign::MiddleLeft);
  snprintf(line, sizeof(line), "Heap: %lu   PSRAM: %lu",
           static_cast<unsigned long>(ESP.getFreeHeap()),
           static_cast<unsigned long>(ESP.getFreePsram()));
  d.drawText(line, x + 24, y + 118, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText("Tap Clear Logs to reset the ring buffer.", x + 24, y + 160, 2,
             theme::MutedText, theme::Panel, TextAlign::MiddleLeft);

  d.fillRoundRect(x, y + 242, 190, 58, 8, theme::PanelAlt);
  d.drawRoundRect(x, y + 242, 190, 58, 8, theme::Border);
  d.drawText("Clear Logs", x + 95, y + 271, 2, theme::Text, theme::PanelAlt,
             TextAlign::MiddleCenter);
}

void DeveloperApp::handleInput(const InputEvent& event) {
  const int16_t x = 38;
  const int16_t y = config::StatusBarHeight + 112 + 242;
  if (event.type == InputEventType::TouchTap &&
      event.x >= x && event.x < x + 190 &&
      event.y >= y && event.y < y + 58) {
    _logger.clear();
  }
}

}  // namespace tabos
