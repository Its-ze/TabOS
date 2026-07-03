#include "app_stopwatch.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

StopwatchApp::StopwatchApp(UtilityManager& utilities) : _utilities(utilities) {}

void StopwatchApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t y = top + 130;
  const uint32_t elapsed = _utilities.stopwatchSeconds();

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("Stopwatch", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  char timeText[24];
  snprintf(timeText, sizeof(timeText), "%02lu:%02lu:%02lu",
           static_cast<unsigned long>(elapsed / 3600),
           static_cast<unsigned long>((elapsed / 60) % 60),
           static_cast<unsigned long>(elapsed % 60));
  d.drawText(timeText, w / 2, y, 4,
             _utilities.stopwatchRunning() ? theme::Accent : theme::Text,
             theme::Background, TextAlign::MiddleCenter);

  const int16_t x = 38;
  const int16_t buttonY = y + 88;
  const char* labels[3] = {"Start", "Stop", "Reset"};
  for (uint8_t i = 0; i < 3; ++i) {
    const int16_t bx = x + i * 190;
    d.fillRoundRect(bx, buttonY, 170, 62, 8, theme::PanelAlt);
    d.drawRoundRect(bx, buttonY, 170, 62, 8, theme::Border);
    d.drawText(labels[i], bx + 85, buttonY + 31, 2, theme::Text,
               theme::PanelAlt, TextAlign::MiddleCenter);
  }
}

void StopwatchApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t y = config::StatusBarHeight + 130 + 88;
  const int16_t x = 38;
  if (event.y < y || event.y >= y + 62 || event.x < x) {
    return;
  }
  const uint8_t index = (event.x - x) / 190;
  if (index == 0) {
    _utilities.startStopwatch();
  } else if (index == 1) {
    _utilities.stopStopwatch();
  } else if (index == 2) {
    _utilities.resetStopwatch();
  }
}

}  // namespace tabos
