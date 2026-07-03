#include "bottom_nav.h"

#include "../config.h"
#include "../hal/hal_display.h"
#include "theme.h"

namespace tabos {

void BottomNav::draw(bool launcherActive) {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t h = d.height();
  const int16_t y = h - config::BottomNavHeight;
  const int16_t buttonW = w / 4;
  const char* labels[4] = {"< Back", "Home", "Apps", "Settings"};

  d.fillRect(0, y, w, config::BottomNavHeight, theme::Bar);
  d.drawLine(0, y, w, y, theme::Border);

  for (int i = 0; i < 4; ++i) {
    const int16_t x = i * buttonW + 8;
    const int16_t bw = buttonW - 16;
    const uint32_t fill =
        (launcherActive && (i == 1 || i == 2)) ? theme::PanelAlt : theme::Panel;
    d.fillRoundRect(x, y + 10, bw, config::BottomNavHeight - 20, 6, fill);
    d.drawRoundRect(x, y + 10, bw, config::BottomNavHeight - 20, 6, theme::Border);
    d.drawText(labels[i], x + bw / 2, y + config::BottomNavHeight / 2, 2,
               theme::Text, fill, TextAlign::MiddleCenter);
  }
}

NavAction BottomNav::hitTest(const InputEvent& event) const {
  if (event.type != InputEventType::TouchTap) {
    return NavAction::None;
  }

  const int16_t y = display().height() - config::BottomNavHeight;
  if (event.y < y) {
    return NavAction::None;
  }

  const int16_t buttonW = display().width() / 4;
  const int index = constrain(event.x / buttonW, 0, 3);
  switch (index) {
    case 0:
      return NavAction::Back;
    case 1:
      return NavAction::Home;
    case 2:
      return NavAction::Apps;
    case 3:
      return NavAction::Settings;
  }
  return NavAction::None;
}

}  // namespace tabos
