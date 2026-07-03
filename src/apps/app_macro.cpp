#include "app_macro.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

MacroApp::MacroApp(UtilityManager& utilities, WifiManager& wifi,
                   BluetoothManager& bluetooth, MeshManager& mesh,
                   HardwareManager& hardware)
    : _utilities(utilities),
      _wifi(wifi),
      _bluetooth(bluetooth),
      _mesh(mesh),
      _hardware(hardware) {}

void MacroApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 30;

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("Macro Pad", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const char* labels[6] = {"Wi-Fi", "Bluetooth", "Mesh Demo", "Relay", "LED Blue", "Mesh Ping"};
  for (uint8_t i = 0; i < 6; ++i) {
    const int16_t bx = x + (i % 3) * 190;
    const int16_t by = top + 92 + (i / 3) * 72;
    d.fillRoundRect(bx, by, 170, 54, 7, theme::PanelAlt);
    d.drawRoundRect(bx, by, 170, 54, 7, theme::Border);
    d.drawText(labels[i], bx + 85, by + 27, 2, theme::Text, theme::PanelAlt,
               TextAlign::MiddleCenter);
  }

  int16_t y = top + 260;
  d.drawText("Recent macros", x, y, 2, theme::MutedText, theme::Background,
             TextAlign::MiddleLeft);
  y += 32;
  const uint8_t start =
      _utilities.macroEventCount() > 5 ? _utilities.macroEventCount() - 5 : 0;
  for (uint8_t i = start; i < _utilities.macroEventCount(); ++i) {
    const MacroEvent& event = _utilities.macroEvent(i);
    const uint32_t fill = (i % 2 == 0) ? theme::Panel : theme::PanelAlt;
    d.fillRoundRect(x, y, w - 60, 42, 6, fill);
    char line[90];
    snprintf(line, sizeof(line), "%lus  %s",
             static_cast<unsigned long>(event.at), event.text);
    d.drawText(line, x + 14, y + 21, 1, theme::Text, fill,
               TextAlign::MiddleLeft);
    y += 48;
  }
}

void MacroApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 30;
  const int16_t y = top + 92;
  if (event.y < y || event.y >= y + 144 || event.x < x) {
    return;
  }
  const uint8_t col = (event.x - x) / 190;
  const uint8_t row = (event.y - y) / 72;
  const uint8_t index = row * 3 + col;
  switch (index) {
    case 0:
      _wifi.toggleEnabled();
      _utilities.addMacroEvent("Toggled Wi-Fi");
      break;
    case 1:
      _bluetooth.toggleEnabled();
      _utilities.addMacroEvent("Toggled Bluetooth");
      break;
    case 2:
      _mesh.injectDemoTraffic();
      _utilities.addMacroEvent("Injected mesh demo");
      break;
    case 3:
      _hardware.setRelay(!_hardware.relayOn());
      _utilities.addMacroEvent("Toggled relay");
      break;
    case 4:
      _hardware.setLedColor("Blue");
      _utilities.addMacroEvent("LED Blue");
      break;
    case 5:
      _mesh.connectSerialText();
      _mesh.sendBroadcast("Macro ping from TabOS");
      _utilities.addMacroEvent("Sent mesh ping");
      break;
  }
}

}  // namespace tabos
