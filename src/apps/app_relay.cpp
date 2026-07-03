#include "app_relay.h"

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

RelayApp::RelayApp(HardwareManager& hardware) : _hardware(hardware) {}

void RelayApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 112;
  const int16_t cardW = w - 76;

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("Relay", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  d.fillRoundRect(x, y, cardW, 130, 8, theme::Panel);
  d.drawRoundRect(x, y, cardW, 130, 8, theme::Border);
  d.drawText("Relay state", x + 26, y + 46, 4,
             _hardware.relayOn() ? theme::Accent : theme::MutedText,
             theme::Panel, TextAlign::MiddleLeft);
  d.drawText(_hardware.relayOn() ? "ON" : "OFF", x + cardW - 32, y + 46, 4,
             _hardware.relayOn() ? theme::Accent : theme::MutedText,
             theme::Panel, TextAlign::MiddleRight);
  d.drawText("Safe stub mode: no physical relay output yet.", x + 26, y + 100,
             2, theme::MutedText, theme::Panel, TextAlign::MiddleLeft);

  const int16_t buttonY = y + 166;
  d.fillRoundRect(x, buttonY, 220, 70, 8, theme::PanelAlt);
  d.drawRoundRect(x, buttonY, 220, 70, 8, theme::Border);
  d.drawText("Relay ON", x + 110, buttonY + 35, 2, theme::Text, theme::PanelAlt,
             TextAlign::MiddleCenter);
  d.fillRoundRect(x + 250, buttonY, 220, 70, 8, theme::PanelAlt);
  d.drawRoundRect(x + 250, buttonY, 220, 70, 8, theme::Border);
  d.drawText("Relay OFF", x + 360, buttonY + 35, 2, theme::Text,
             theme::PanelAlt, TextAlign::MiddleCenter);
}

void RelayApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t buttonY = top + 112 + 166;
  if (event.y < buttonY || event.y >= buttonY + 70) {
    return;
  }
  if (event.x >= x && event.x < x + 220) {
    _hardware.setRelay(true);
  } else if (event.x >= x + 250 && event.x < x + 470) {
    _hardware.setRelay(false);
  }
}

}  // namespace tabos
