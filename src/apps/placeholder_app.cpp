#include "placeholder_app.h"

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

PlaceholderApp::PlaceholderApp(const char* id, const char* name, const char* icon,
                               const char* detail)
    : _id(id), _name(name), _icon(icon), _detail(detail) {}

void PlaceholderApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText(_icon, 36, top + 58, 4, theme::Accent, theme::Background,
             TextAlign::MiddleLeft);
  d.drawText(_name, 112, top + 58, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const int16_t cardX = 36;
  const int16_t cardW = w - 72;
  const int16_t cardY = top + 120;
  d.fillRoundRect(cardX, cardY, cardW, 118, 8, theme::Panel);
  d.drawRoundRect(cardX, cardY, cardW, 118, 8, theme::Border);
  d.drawText("Future feature placeholder", cardX + 24, cardY + 32, 2, theme::Accent,
             theme::Panel, TextAlign::MiddleLeft);
  d.drawText(_detail, cardX + 24, cardY + 76, 2, theme::Text, theme::Panel,
             TextAlign::MiddleLeft);

  const int16_t noteY = cardY + 148;
  d.fillRoundRect(cardX, noteY, cardW, 86, 8, theme::PanelAlt);
  d.drawText("Back returns to the launcher. Future hardware work will stay behind HALs.",
             cardX + 24, noteY + 43, 2, theme::MutedText, theme::PanelAlt,
             TextAlign::MiddleLeft);
}

}  // namespace tabos
