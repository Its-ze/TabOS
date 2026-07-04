#include "app_sensors.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

SensorsApp::SensorsApp(HardwareManager& hardware) : _hardware(hardware) {}

void SensorsApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  const SensorSnapshot& s = _hardware.sensors();

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("Sensors", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  char value[24];
  snprintf(value, sizeof(value), "%.1f C", s.temperatureC);
  drawMetric(0, "Temperature", value);
  snprintf(value, sizeof(value), "%.0f%%", s.humidity);
  drawMetric(1, "Humidity", value);
  snprintf(value, sizeof(value), "%.0f lx", s.light);
  drawMetric(2, "Light", value);
  snprintf(value, sizeof(value), "%.2f V", s.voltage);
  drawMetric(3, "Voltage", value);
  drawMetric(4, "Motion", s.motion ? "Yes" : "No");
  snprintf(value, sizeof(value), "%lus", static_cast<unsigned long>(s.updatedAt));
  drawMetric(5, "Updated", value);

  d.fillRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Panel);
  d.drawRoundRect(w - 184, bottom - 60, 154, 44, 6, theme::Border);
  d.drawText("Refresh", w - 107, bottom - 38, 2, theme::Text, theme::Panel,
             TextAlign::MiddleCenter);
}

void SensorsApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t bottom = display().height() - config::BottomNavHeight;
  if (event.y > bottom - 70 && event.x > display().width() - 210) {
    _hardware.refreshSensors();
  }
}

void SensorsApp::drawMetric(uint8_t index, const char* label, const char* value) {
  HalDisplay& d = display();
  const int16_t x = 34 + (index % 2) * ((d.width() - 86) / 2 + 18);
  const int16_t y = config::StatusBarHeight + 100 + (index / 2) * 92;
  const int16_t w = (d.width() - 86) / 2;
  const uint32_t fill = (index % 2 == 0) ? theme::Panel : theme::PanelAlt;
  d.fillRoundRect(x, y, w, 74, 8, fill);
  d.drawText(label, x + 18, y + 22, 2, theme::MutedText, fill,
             TextAlign::MiddleLeft);
  d.drawText(value, x + 18, y + 52, 4, theme::Text, fill,
             TextAlign::MiddleLeft);
}

}  // namespace tabos
