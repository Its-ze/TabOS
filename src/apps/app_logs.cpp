#include "app_logs.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

LogsApp::LogsApp(Logger& logger) : _logger(logger) {}

void LogsApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("Logs", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const int16_t x = 30;
  int16_t y = top + 92;
  const size_t count = _logger.count();
  const size_t visible = count > 8 ? 8 : count;
  const size_t start = count > visible ? count - visible : 0;

  for (size_t i = 0; i < visible; ++i) {
    const LogEntry& entry = _logger.entry(start + i);
    const uint32_t fill = (i % 2 == 0) ? theme::Panel : theme::PanelAlt;
    d.fillRoundRect(x, y, w - 60, 44, 6, fill);

    char line[132];
    snprintf(line, sizeof(line), "[%s] %lu  %s", Logger::levelName(entry.level),
             static_cast<unsigned long>(entry.at / 1000), entry.message);
    d.drawText(line, x + 16, y + 22, 2, theme::Text, fill, TextAlign::MiddleLeft);
    y += 50;
  }

  if (visible == 0) {
    d.drawText("No log entries yet", x + 16, y + 22, 2, theme::MutedText,
               theme::Background, TextAlign::MiddleLeft);
  }

  d.fillRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Panel);
  d.drawRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Border);
  d.drawText("Clear", w - 107, bottom - 38, 2, theme::Text, theme::Panel,
             TextAlign::MiddleCenter);
}

void LogsApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }

  const int16_t bottom = display().height() - config::BottomNavHeight;
  if (event.y > bottom - 70 && event.x > display().width() - 210) {
    _logger.clear();
  }
}

}  // namespace tabos
