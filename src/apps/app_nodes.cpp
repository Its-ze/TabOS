#include "app_nodes.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

namespace {
void formatNodeId(uint32_t id, char* out, size_t outLen) {
  snprintf(out, outLen, "!%08lX", static_cast<unsigned long>(id));
}
}  // namespace

NodesApp::NodesApp(MeshManager& mesh) : _mesh(mesh) {}

void NodesApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  const int16_t x = 30;

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("Nodes", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  char summary[48];
  snprintf(summary, sizeof(summary), "%u known", _mesh.nodeCount());
  d.drawText(summary, w - 34, top + 44, 2, theme::MutedText, theme::Background,
             TextAlign::MiddleRight);

  int16_t y = top + 92;
  const uint8_t visible = min<uint8_t>(_mesh.nodeCount(), 7);
  for (uint8_t i = 0; i < visible; ++i) {
    const MeshNode& node = _mesh.node(i);
    const uint32_t fill = node.newNode ? theme::PanelAlt : theme::Panel;
    d.fillRoundRect(x, y, w - 60, 58, 6, fill);
    d.drawRoundRect(x, y, w - 60, 58, 6,
                    node.newNode ? theme::Accent : theme::Border);

    char id[16];
    formatNodeId(node.nodeId, id, sizeof(id));
    char line[110];
    snprintf(line, sizeof(line), "%s  %s  %s", node.shortName, node.longName,
             node.newNode ? "NEW" : "");
    d.drawText(line, x + 16, y + 18, 2, theme::Text, fill,
               TextAlign::MiddleLeft);

    if (node.battery >= 0) {
      snprintf(line, sizeof(line), "%s   %ddBm  SNR %.1f  hops %u  batt %d%%",
               id, node.rssi, node.snr, node.hopCount, node.battery);
    } else {
      snprintf(line, sizeof(line), "%s   %ddBm  SNR %.1f  hops %u",
               id, node.rssi, node.snr, node.hopCount);
    }
    d.drawText(line, x + 16, y + 42, 1, theme::MutedText, fill,
               TextAlign::MiddleLeft);
    y += 66;
  }

  if (_mesh.nodeCount() == 0) {
    d.fillRoundRect(x, y, w - 60, 100, 8, theme::Panel);
    d.drawText("No nodes yet", x + 22, y + 34, 2, theme::Text, theme::Panel,
               TextAlign::MiddleLeft);
    d.drawText("Open Mesh and tap Discover, or use mesh demo over serial.",
               x + 22, y + 72, 2, theme::MutedText, theme::Panel,
               TextAlign::MiddleLeft);
  }

  d.fillRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Panel);
  d.drawRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Border);
  d.drawText("Clear NEW", w - 107, bottom - 38, 2, theme::Text, theme::Panel,
             TextAlign::MiddleCenter);
}

void NodesApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t bottom = display().height() - config::BottomNavHeight;
  if (event.y > bottom - 70 && event.x > display().width() - 210) {
    _mesh.clearNewFlags();
  }
}

}  // namespace tabos
