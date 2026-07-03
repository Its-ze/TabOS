#include "app_gpio.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

GpioApp::GpioApp(HardwareManager& hardware) : _hardware(hardware) {}

void GpioApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  const int16_t x = 34;

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("GPIO", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  char armText[28];
  snprintf(armText, sizeof(armText), _hardware.gpioArmed() ? "Armed %us" : "Locked",
           _hardware.gpioArmSecondsLeft());
  d.drawText(armText, w - 34, top + 44, 2,
             _hardware.gpioArmed() ? theme::Accent : theme::Warning,
             theme::Background, TextAlign::MiddleRight);

  int16_t y = top + 88;
  d.fillRoundRect(x, y, w - 68, 70, 8, theme::Panel);
  d.drawText("GPIO is simulated until board-safe pin profiles are added.",
             x + 20, y + 24, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText("Tap Arm before any write-style action.", x + 20, y + 50, 2,
             theme::Warning, theme::Panel, TextAlign::MiddleLeft);

  y += 88;
  d.fillRoundRect(x, y, 170, 50, 7, theme::PanelAlt);
  d.drawRoundRect(x, y, 170, 50, 7, theme::Border);
  d.drawText("Arm 15s", x + 85, y + 25, 2, theme::Text, theme::PanelAlt,
             TextAlign::MiddleCenter);

  y += 70;
  for (uint8_t i = 0; i < MaxGpioPins; ++i) {
    const GpioPinState& pin = _hardware.gpioPin(i);
    const uint32_t fill = (i % 2 == 0) ? theme::Panel : theme::PanelAlt;
    d.fillRoundRect(x, y, w - 68, 52, 6, fill);
    char row[96];
    snprintf(row, sizeof(row), "GPIO %u   %s   %s", pin.pin,
             pin.mode == GpioMode::Output ? "OUTPUT" : "INPUT",
             pin.value ? "HIGH" : "LOW");
    d.drawText(row, x + 18, y + 26, 2, theme::Text, fill,
               TextAlign::MiddleLeft);
    d.drawText("tap left: mode   tap right: value", w - 52, y + 26, 1,
               theme::MutedText, fill, TextAlign::MiddleRight);
    y += 60;
  }
}

void GpioApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 34;
  const int16_t armY = top + 88 + 88;
  if (event.y >= armY && event.y < armY + 50 && event.x >= x && event.x < x + 170) {
    _hardware.armGpio();
    return;
  }

  const int16_t listY = armY + 70;
  if (event.y < listY) {
    return;
  }
  const uint8_t row = (event.y - listY) / 60;
  if (row >= MaxGpioPins) {
    return;
  }
  if (event.x < display().width() / 2) {
    const GpioPinState& pin = _hardware.gpioPin(row);
    _hardware.setGpioMode(row, pin.mode == GpioMode::Output ? GpioMode::Input
                                                            : GpioMode::Output);
  } else {
    _hardware.toggleGpioValue(row);
  }
}

}  // namespace tabos
