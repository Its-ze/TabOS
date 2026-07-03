#include "app_messages.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

MessagesApp::MessagesApp(MeshManager& mesh) : _mesh(mesh) {}

void MessagesApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  const int16_t x = 30;

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("Messages", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const int16_t buttonY = top + 82;
  d.fillRoundRect(x, buttonY, 220, 50, 7, theme::PanelAlt);
  d.drawRoundRect(x, buttonY, 220, 50, 7, theme::Border);
  d.drawText("Send Ping", x + 110, buttonY + 25, 2, theme::Text,
             theme::PanelAlt, TextAlign::MiddleCenter);

  d.fillRoundRect(x + 244, buttonY, 220, 50, 7, theme::PanelAlt);
  d.drawRoundRect(x + 244, buttonY, 220, 50, 7, theme::Border);
  d.drawText("Status Check", x + 354, buttonY + 25, 2, theme::Text,
             theme::PanelAlt, TextAlign::MiddleCenter);

  int16_t y = buttonY + 74;
  const uint8_t count = _mesh.messageCount();
  const uint8_t start = count > 6 ? count - 6 : 0;
  for (uint8_t i = start; i < count; ++i) {
    const MeshMessage& msg = _mesh.message(i);
    const uint32_t fill = msg.outgoing ? theme::PanelAlt : theme::Panel;
    d.fillRoundRect(x, y, w - 60, 58, 6, fill);

    char line[112];
    snprintf(line, sizeof(line), "%s  %s", msg.outgoing ? "OUT" : "IN ",
             msg.delivered ? "delivered" : "pending");
    d.drawText(line, x + 16, y + 18, 1, theme::MutedText, fill,
               TextAlign::MiddleLeft);
    d.drawText(msg.text, x + 16, y + 42, 2, theme::Text, fill,
               TextAlign::MiddleLeft);
    y += 66;
  }

  if (count == 0) {
    d.fillRoundRect(x, y, w - 60, 100, 8, theme::Panel);
    d.drawText("No messages yet", x + 22, y + 34, 2, theme::Text, theme::Panel,
               TextAlign::MiddleLeft);
    d.drawText("Send a canned broadcast or use mesh msg text over serial.",
               x + 22, y + 72, 2, theme::MutedText, theme::Panel,
               TextAlign::MiddleLeft);
  }
}

void MessagesApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }

  const int16_t top = config::StatusBarHeight;
  const int16_t x = 30;
  const int16_t buttonY = top + 82;
  if (event.y < buttonY || event.y >= buttonY + 50) {
    return;
  }

  if (event.x >= x && event.x < x + 220) {
    _mesh.connectSerialText();
    _mesh.sendBroadcast("Ping from TabOS");
  } else if (event.x >= x + 244 && event.x < x + 464) {
    _mesh.connectSerialText();
    _mesh.sendBroadcast("Status check from TabOS");
  }
}

}  // namespace tabos
