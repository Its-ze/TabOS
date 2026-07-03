#include "app_ir_remote.h"

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

IrRemoteApp::IrRemoteApp(HardwareManager& hardware) : _hardware(hardware) {}

void IrRemoteApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 112;
  const int16_t cardW = w - 76;

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("IR Remote", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  d.fillRoundRect(x, y, cardW, 112, 8, theme::Panel);
  d.drawRoundRect(x, y, cardW, 112, 8, theme::Border);
  d.drawText("Profile", x + 26, y + 34, 2, theme::Accent, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText(_hardware.irProfile(), x + cardW - 26, y + 34, 2, theme::Text,
             theme::Panel, TextAlign::MiddleRight);
  d.drawText("Last command", x + 26, y + 76, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText(_hardware.lastIrCommand(), x + cardW - 26, y + 76, 2, theme::Text,
             theme::Panel, TextAlign::MiddleRight);

  const char* labels[6] = {"Profile", "Power", "Input", "Vol +", "Vol -", "Mute"};
  for (uint8_t i = 0; i < 6; ++i) {
    const int16_t col = i % 3;
    const int16_t row = i / 3;
    const int16_t bx = x + col * 190;
    const int16_t by = y + 142 + row * 72;
    d.fillRoundRect(bx, by, 170, 54, 7, theme::PanelAlt);
    d.drawRoundRect(bx, by, 170, 54, 7, theme::Border);
    d.drawText(labels[i], bx + 85, by + 27, 2, theme::Text, theme::PanelAlt,
               TextAlign::MiddleCenter);
  }
}

void IrRemoteApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 112 + 142;
  if (event.y < y || event.y >= y + 144 || event.x < x) {
    return;
  }
  const uint8_t col = (event.x - x) / 190;
  const uint8_t row = (event.y - y) / 72;
  if (col > 2 || row > 1) {
    return;
  }
  const uint8_t index = row * 3 + col;
  const char* commands[6] = {"", "POWER", "INPUT", "VOL_UP", "VOL_DOWN", "MUTE"};
  if (index == 0) {
    _hardware.nextIrProfile();
  } else {
    _hardware.sendIrCommand(commands[index]);
  }
}

}  // namespace tabos
