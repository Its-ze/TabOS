#include "app_timer.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

TimerApp::TimerApp(UtilityManager& utilities) : _utilities(utilities) {}

void TimerApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 118;
  const uint32_t remaining = _utilities.timerRemainingSeconds();

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("Timer", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  char timeText[24];
  snprintf(timeText, sizeof(timeText), "%02lu:%02lu",
           static_cast<unsigned long>(remaining / 60),
           static_cast<unsigned long>(remaining % 60));
  d.drawText(timeText, w / 2, y + 40, 4,
             _utilities.timerRunning() ? theme::Accent : theme::Text,
             theme::Background, TextAlign::MiddleCenter);

  const char* labels[4] = {"1 min", "5 min", "Stop", "Reset"};
  for (uint8_t i = 0; i < 4; ++i) {
    const int16_t bx = x + (i % 2) * 230;
    const int16_t by = y + 110 + (i / 2) * 82;
    d.fillRoundRect(bx, by, 200, 62, 8, theme::PanelAlt);
    d.drawRoundRect(bx, by, 200, 62, 8, theme::Border);
    d.drawText(labels[i], bx + 100, by + 31, 2, theme::Text, theme::PanelAlt,
               TextAlign::MiddleCenter);
  }
}

void TimerApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 118 + 110;
  if (event.y < y || event.y >= y + 164 || event.x < x) {
    return;
  }
  const uint8_t col = (event.x - x) / 230;
  const uint8_t row = (event.y - y) / 82;
  const uint8_t index = row * 2 + col;
  switch (index) {
    case 0:
      _utilities.startTimer(60);
      break;
    case 1:
      _utilities.startTimer(300);
      break;
    case 2:
      _utilities.stopTimer();
      break;
    case 3:
      _utilities.resetTimer();
      break;
  }
}

}  // namespace tabos
