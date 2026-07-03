#include "status_bar.h"

#include <Arduino.h>
#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "theme.h"

namespace tabos {

void StatusBar::draw(const BatteryManager& battery, const SettingsManager& settings,
                     const MeshManager& mesh, const char* title) {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t h = config::StatusBarHeight;

  d.fillRect(0, 0, w, h, theme::Bar);
  d.drawLine(0, h - 1, w, h - 1, theme::Border);

  char left[48];
  snprintf(left, sizeof(left), "%s  %s", settings.data().deviceName,
           title ? title : "");
  d.drawText(left, 18, h / 2, 2, theme::Text, theme::Bar, TextAlign::MiddleLeft);

  const uint32_t seconds = millis() / 1000;
  const uint32_t mins = (seconds / 60) % 100;
  const uint32_t secs = seconds % 60;

  char batteryText[12];
  if (battery.available()) {
    snprintf(batteryText, sizeof(batteryText), "%d%%", battery.percent());
  } else {
    snprintf(batteryText, sizeof(batteryText), "--%%");
  }

  char right[96];
  snprintf(right, sizeof(right), "W:%s  B:%s  M:%s  %s%s  %02lu:%02lu",
           settings.data().wifiEnabled ? "ON" : "--",
           settings.data().bluetoothEnabled ? "ON" : "--",
           mesh.connected() ? "ON" : "--",
           batteryText,
           battery.isCharging() ? "+" : "",
           static_cast<unsigned long>(mins),
           static_cast<unsigned long>(secs));

  d.drawText(right, w - 18, h / 2, 2, theme::MutedText, theme::Bar,
             TextAlign::MiddleRight);
}

}  // namespace tabos
