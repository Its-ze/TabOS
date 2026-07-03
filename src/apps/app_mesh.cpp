#include "app_mesh.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

MeshApp::MeshApp(MeshManager& mesh) : _mesh(mesh) {}

void MeshApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  const int16_t x = 38;
  const int16_t y = top + 112;
  const int16_t cardW = w - 76;

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("Mesh", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  d.fillRoundRect(x, y, cardW, 150, 8, theme::Panel);
  d.drawRoundRect(x, y, cardW, 150, 8, theme::Border);
  d.drawText("Meshtastic connection layer", x + 26, y + 34, 2, theme::Accent,
             theme::Panel, TextAlign::MiddleLeft);
  d.drawText(_mesh.stateText(), x + cardW - 26, y + 34, 2,
             _mesh.connected() ? theme::Accent : theme::MutedText,
             theme::Panel, TextAlign::MiddleRight);

  char meta[96];
  snprintf(meta, sizeof(meta), "Transport: %s    Channel: %s",
           _mesh.connectionText(), _mesh.channelText());
  d.drawText(meta, x + 26, y + 78, 2, theme::Text, theme::Panel,
             TextAlign::MiddleLeft);
  snprintf(meta, sizeof(meta), "Nodes: %u    Messages: %u    Packets: %u",
           _mesh.nodeCount(), _mesh.messageCount(), _mesh.packetCount());
  d.drawText(meta, x + 26, y + 116, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);

  const int16_t buttonY = y + 176;
  d.fillRoundRect(x, buttonY, 170, 58, 7, theme::PanelAlt);
  d.drawRoundRect(x, buttonY, 170, 58, 7, theme::Border);
  d.drawText(_mesh.connected() ? "Disconnect" : "Connect", x + 85,
             buttonY + 29, 2, theme::Text, theme::PanelAlt,
             TextAlign::MiddleCenter);

  d.fillRoundRect(x + 190, buttonY, 170, 58, 7, theme::PanelAlt);
  d.drawRoundRect(x + 190, buttonY, 170, 58, 7, theme::Border);
  d.drawText("Discover", x + 275, buttonY + 29, 2, theme::Text,
             theme::PanelAlt, TextAlign::MiddleCenter);

  d.fillRoundRect(x + 380, buttonY, 170, 58, 7, theme::PanelAlt);
  d.drawRoundRect(x + 380, buttonY, 170, 58, 7, theme::Border);
  d.drawText("Ping", x + 465, buttonY + 29, 2, theme::Text,
             theme::PanelAlt, TextAlign::MiddleCenter);

  const int16_t noteY = buttonY + 86;
  d.fillRoundRect(x, noteY, cardW, 92, 8, theme::Panel);
  d.drawText("Serial text adapter is active for mesh testing.",
             x + 24, noteY + 32, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText("Use Serial Monitor: mesh connect, mesh demo, mesh msg text",
             x + 24, noteY + 66, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
}

void MeshApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }

  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t buttonY = top + 112 + 176;
  if (event.y < buttonY || event.y >= buttonY + 58) {
    return;
  }

  if (event.x >= x && event.x < x + 170) {
    if (_mesh.connected()) {
      _mesh.disconnect();
    } else {
      _mesh.connectSerialText();
    }
  } else if (event.x >= x + 190 && event.x < x + 360) {
    _mesh.injectDemoTraffic();
  } else if (event.x >= x + 380 && event.x < x + 550) {
    _mesh.sendBroadcast("Ping from TabOS");
  }
}

}  // namespace tabos
