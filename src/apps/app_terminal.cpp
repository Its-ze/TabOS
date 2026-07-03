#include "app_terminal.h"

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

TerminalApp::TerminalApp(TerminalManager& terminal) : _terminal(terminal) {}

void TerminalApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("Terminal", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const int16_t x = 30;
  const int16_t y = top + 92;
  const int16_t cardW = w - 60;
  const int16_t cardH = bottom - y - 72;
  d.fillRoundRect(x, y, cardW, cardH, 8, 0x050807);
  d.drawRoundRect(x, y, cardW, cardH, 8, theme::Border);

  int16_t lineY = y + 26;
  const uint8_t start =
      _terminal.lineCount() > 9 ? _terminal.lineCount() - 9 : 0;
  for (uint8_t i = start; i < _terminal.lineCount(); ++i) {
    d.drawText(_terminal.line(i), x + 18, lineY, 2, theme::Accent, 0x050807,
               TextAlign::MiddleLeft);
    lineY += 34;
  }

  d.drawText("USB serial monitor: help, status, version, clear, logs",
             x + 18, y + cardH - 26, 1, theme::MutedText, 0x050807,
             TextAlign::MiddleLeft);

  d.fillRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Panel);
  d.drawRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Border);
  d.drawText("Clear", w - 107, bottom - 38, 2, theme::Text, theme::Panel,
             TextAlign::MiddleCenter);
}

void TerminalApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t bottom = display().height() - config::BottomNavHeight;
  if (event.y > bottom - 70 && event.x > display().width() - 210) {
    _terminal.clear();
    _terminal.printLine("Terminal cleared");
  }
}

}  // namespace tabos
