#include "app_bluetooth.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

BluetoothApp::BluetoothApp(BluetoothManager& bluetooth) : _bluetooth(bluetooth) {}

void BluetoothApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 118;
  const int16_t cardW = w - 76;

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("Bluetooth", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  d.fillRoundRect(x, y, cardW, 116, 8, theme::Panel);
  d.drawRoundRect(x, y, cardW, 116, 8, theme::Border);
  d.drawText("BLE scanner", x + 26, y + 34, 2, theme::Accent, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText(_bluetooth.statusText(), x + cardW - 26, y + 34, 2,
             _bluetooth.enabled() ? theme::Accent : theme::MutedText,
             theme::Panel, TextAlign::MiddleRight);

  char meta[64];
  snprintf(meta, sizeof(meta), "Devices: %u", _bluetooth.deviceCount());
  d.drawText(meta, x + 26, y + 78, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);

  const int16_t buttonY = y + 140;
  d.fillRoundRect(x, buttonY, 160, 50, 7, theme::PanelAlt);
  d.drawRoundRect(x, buttonY, 160, 50, 7, theme::Border);
  d.drawText(_bluetooth.enabled() ? "Turn Off" : "Turn On", x + 80, buttonY + 25,
             2, theme::Text, theme::PanelAlt, TextAlign::MiddleCenter);

  d.fillRoundRect(x + 180, buttonY, 160, 50, 7, theme::PanelAlt);
  d.drawRoundRect(x + 180, buttonY, 160, 50, 7, theme::Border);
  d.drawText(_bluetooth.scanning() ? "Scanning" : "Scan", x + 260, buttonY + 25,
             2, theme::Text, theme::PanelAlt, TextAlign::MiddleCenter);

  int16_t listY = buttonY + 72;
  for (uint8_t i = 0; i < _bluetooth.deviceCount(); ++i) {
    const BleDeviceInfo& dev = _bluetooth.device(i);
    const uint32_t fill = (i % 2 == 0) ? theme::Panel : theme::PanelAlt;
    d.fillRoundRect(x, listY, cardW, 44, 6, fill);
    char row[100];
    snprintf(row, sizeof(row), "%s   %d dBm", dev.name, dev.rssi);
    d.drawText(row, x + 16, listY + 15, 2, theme::Text, fill,
               TextAlign::MiddleLeft);
    d.drawText(dev.address, x + 16, listY + 33, 1, theme::MutedText, fill,
               TextAlign::MiddleLeft);
    listY += 52;
  }

  if (!_bluetooth.supported()) {
    d.drawText("BLE library is unavailable for this build.", x + 16, listY + 22,
               2, theme::Warning, theme::Background, TextAlign::MiddleLeft);
  } else if (_bluetooth.enabled() && !_bluetooth.scanning() &&
             _bluetooth.deviceCount() == 0) {
    d.drawText("Tap Scan to discover nearby BLE devices.", x + 16, listY + 22,
               2, theme::MutedText, theme::Background, TextAlign::MiddleLeft);
  }
}

void BluetoothApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }

  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t buttonY = top + 118 + 140;
  if (event.y >= buttonY && event.y < buttonY + 50) {
    if (event.x >= x && event.x < x + 160) {
      _bluetooth.toggleEnabled();
    } else if (event.x >= x + 180 && event.x < x + 340) {
      _bluetooth.requestScan();
    }
  }
}

}  // namespace tabos
