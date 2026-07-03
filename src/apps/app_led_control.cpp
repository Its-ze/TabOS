#include "app_led_control.h"

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

LedControlApp::LedControlApp(HardwareManager& hardware) : _hardware(hardware) {}

void LedControlApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 112;
  const int16_t cardW = w - 76;

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("LED Control", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  d.fillRoundRect(x, y, cardW, 100, 8, theme::Panel);
  d.drawRoundRect(x, y, cardW, 100, 8, theme::Border);
  d.drawText("Selected color", x + 26, y + 50, 4, theme::Text, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText(_hardware.ledColor(), x + cardW - 26, y + 50, 4, theme::Accent,
             theme::Panel, TextAlign::MiddleRight);

  const char* labels[5] = {"Red", "Green", "Blue", "White", "Off"};
  for (uint8_t i = 0; i < 5; ++i) {
    const int16_t bx = x + (i % 3) * 190;
    const int16_t by = y + 132 + (i / 3) * 72;
    d.fillRoundRect(bx, by, 170, 54, 7, theme::PanelAlt);
    d.drawRoundRect(bx, by, 170, 54, 7, theme::Border);
    d.drawText(labels[i], bx + 85, by + 27, 2, theme::Text, theme::PanelAlt,
               TextAlign::MiddleCenter);
  }
}

void LedControlApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 112 + 132;
  if (event.y < y || event.y >= y + 144 || event.x < x) {
    return;
  }
  const uint8_t col = (event.x - x) / 190;
  const uint8_t row = (event.y - y) / 72;
  const uint8_t index = row * 3 + col;
  const char* colors[5] = {"Red", "Green", "Blue", "White", "Off"};
  if (index < 5) {
    _hardware.setLedColor(colors[index]);
  }
}

}  // namespace tabos
