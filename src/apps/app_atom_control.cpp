#include "app_atom_control.h"

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

AtomControlApp::AtomControlApp(HardwareManager& hardware) : _hardware(hardware) {}

void AtomControlApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 112;
  const int16_t cardW = w - 76;

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("Atom Control", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  d.fillRoundRect(x, y, cardW, 130, 8, theme::Panel);
  d.drawRoundRect(x, y, cardW, 130, 8, theme::Border);
  d.drawText("Connected unit", x + 26, y + 34, 2, theme::Accent, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText(_hardware.atomStatus(), x + cardW - 26, y + 34, 2,
             _hardware.atomDetected() ? theme::Accent : theme::MutedText,
             theme::Panel, TextAlign::MiddleRight);
  d.drawText("Last command", x + 26, y + 84, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText(_hardware.lastAtomCommand(), x + cardW - 26, y + 84, 2,
             theme::Text, theme::Panel, TextAlign::MiddleRight);

  const char* labels[6] = {"Detect", "Status", "LED Red", "LED Green", "LED Blue", "Relay"};
  for (uint8_t i = 0; i < 6; ++i) {
    const int16_t col = i % 3;
    const int16_t row = i / 3;
    const int16_t bx = x + col * 190;
    const int16_t by = y + 160 + row * 72;
    d.fillRoundRect(bx, by, 170, 54, 7, theme::PanelAlt);
    d.drawRoundRect(bx, by, 170, 54, 7, theme::Border);
    d.drawText(labels[i], bx + 85, by + 27, 2, theme::Text, theme::PanelAlt,
               TextAlign::MiddleCenter);
  }
}

void AtomControlApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }

  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 112 + 160;
  if (event.y < y || event.y >= y + 144) {
    return;
  }

  const uint8_t col = (event.x - x) / 190;
  const uint8_t row = (event.y - y) / 72;
  if (event.x < x || col > 2 || row > 1) {
    return;
  }
  const uint8_t index = row * 3 + col;
  switch (index) {
    case 0:
      _hardware.detectAtom();
      break;
    case 1:
      _hardware.sendAtomCommand("ATOM:STATUS");
      break;
    case 2:
      _hardware.sendAtomCommand("ATOM:LED:RED");
      break;
    case 3:
      _hardware.sendAtomCommand("ATOM:LED:GREEN");
      break;
    case 4:
      _hardware.sendAtomCommand("ATOM:LED:BLUE");
      break;
    case 5:
      _hardware.sendAtomCommand("ATOM:RELAY:TOGGLE");
      break;
  }
}

}  // namespace tabos
