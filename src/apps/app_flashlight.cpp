#include "app_flashlight.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

FlashlightApp::FlashlightApp(UtilityManager& utilities, SettingsManager& settings)
    : _utilities(utilities), _settings(settings) {}

void FlashlightApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  uint32_t bg = theme::Background;
  uint32_t fg = theme::Text;
  if (_utilities.flashlightMode() == FlashlightMode::White) {
    bg = 0xFFFFFF;
    fg = 0x000000;
  } else if (_utilities.flashlightMode() == FlashlightMode::Red) {
    bg = 0xB00000;
    fg = 0xFFFFFF;
  }

  d.fillRect(0, top, w, bottom - top, bg);
  d.drawText(_utilities.flashlightModeText(), w / 2, top + 82, 4, fg, bg,
             TextAlign::MiddleCenter);
  char brightness[24];
  snprintf(brightness, sizeof(brightness), "Brightness %u",
           _utilities.flashlightBrightness());
  d.drawText(brightness, w / 2, top + 132, 2, fg, bg, TextAlign::MiddleCenter);

  const int16_t x = 38;
  const int16_t buttonY = bottom - 170;
  const char* labels[4] = {"White", "Red", "Bright", "Off"};
  for (uint8_t i = 0; i < 4; ++i) {
    const int16_t bx = x + (i % 2) * 230;
    const int16_t by = buttonY + (i / 2) * 72;
    d.fillRoundRect(bx, by, 200, 54, 7, theme::Panel);
    d.drawRoundRect(bx, by, 200, 54, 7, theme::Border);
    d.drawText(labels[i], bx + 100, by + 27, 2, theme::Text, theme::Panel,
               TextAlign::MiddleCenter);
  }
}

void FlashlightApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t bottom = display().height() - config::BottomNavHeight;
  const int16_t x = 38;
  const int16_t buttonY = bottom - 170;
  if (event.y < buttonY || event.y >= buttonY + 144 || event.x < x) {
    return;
  }
  const uint8_t col = (event.x - x) / 230;
  const uint8_t row = (event.y - buttonY) / 72;
  const uint8_t index = row * 2 + col;
  if (index == 0) {
    _utilities.setFlashlightMode(FlashlightMode::White);
    applyBrightness();
  } else if (index == 1) {
    _utilities.setFlashlightMode(FlashlightMode::Red);
    applyBrightness();
  } else if (index == 2) {
    _utilities.cycleFlashlightBrightness();
    applyBrightness();
  } else if (index == 3) {
    _utilities.setFlashlightMode(FlashlightMode::Off);
    display().setBrightness(_settings.data().brightness);
  }
}

void FlashlightApp::applyBrightness() {
  display().setBrightness(_utilities.flashlightBrightness());
}

}  // namespace tabos
