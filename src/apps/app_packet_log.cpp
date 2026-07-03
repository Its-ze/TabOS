#include "app_packet_log.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

PacketLogApp::PacketLogApp(MeshManager& mesh) : _mesh(mesh) {}

void PacketLogApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  const int16_t x = 30;

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("Packet Log", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  int16_t y = top + 92;
  const uint8_t count = _mesh.packetCount();
  const uint8_t start = count > 8 ? count - 8 : 0;
  for (uint8_t i = start; i < count; ++i) {
    const MeshPacket& pkt = _mesh.packet(i);
    const uint32_t fill = (i % 2 == 0) ? theme::Panel : theme::PanelAlt;
    d.fillRoundRect(x, y, w - 60, 44, 6, fill);

    char line[128];
    snprintf(line, sizeof(line), "%lus  %s  !%08lX  %ddBm  %.1f  %s",
             static_cast<unsigned long>(pkt.at), pkt.type,
             static_cast<unsigned long>(pkt.from), pkt.rssi, pkt.snr,
             pkt.summary);
    d.drawText(line, x + 14, y + 22, 1, theme::Text, fill,
               TextAlign::MiddleLeft);
    y += 50;
  }

  if (count == 0) {
    d.fillRoundRect(x, y, w - 60, 100, 8, theme::Panel);
    d.drawText("No packets logged", x + 22, y + 34, 2, theme::Text,
               theme::Panel, TextAlign::MiddleLeft);
    d.drawText("Open Mesh and tap Discover to generate test packets.",
               x + 22, y + 72, 2, theme::MutedText, theme::Panel,
               TextAlign::MiddleLeft);
  }

  d.fillRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Panel);
  d.drawRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Border);
  d.drawText("Clear", w - 107, bottom - 38, 2, theme::Text, theme::Panel,
             TextAlign::MiddleCenter);
}

void PacketLogApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t bottom = display().height() - config::BottomNavHeight;
  if (event.y > bottom - 70 && event.x > display().width() - 210) {
    _mesh.clearPacketLog();
  }
}

}  // namespace tabos
