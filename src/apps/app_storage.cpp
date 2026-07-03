#include "app_storage.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

StorageApp::StorageApp(UtilityManager& utilities) : _utilities(utilities) {}

void StorageApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 30;

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("Storage", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  d.drawText(_utilities.storageAvailable() ? "NVS ready" : "Storage unavailable",
             w - 34, top + 44, 2,
             _utilities.storageAvailable() ? theme::Accent : theme::Warning,
             theme::Background, TextAlign::MiddleRight);

  int16_t y = top + 96;
  for (uint8_t i = 0; i < _utilities.virtualFileCount(); ++i) {
    const uint32_t fill = (i % 2 == 0) ? theme::Panel : theme::PanelAlt;
    d.fillRoundRect(x, y, w - 60, 58, 6, fill);
    d.drawText(_utilities.virtualFileName(i), x + 16, y + 20, 2, theme::Text,
               fill, TextAlign::MiddleLeft);
    char info[70];
    _utilities.virtualFileInfo(i, info, sizeof(info));
    d.drawText(info, x + 16, y + 44, 1, theme::MutedText, fill,
               TextAlign::MiddleLeft);
    y += 66;
  }

  d.fillRoundRect(x, y + 12, w - 60, 74, 8, theme::Panel);
  d.drawText("This browser is a safe local storage index.", x + 18, y + 40, 2,
             theme::MutedText, theme::Panel, TextAlign::MiddleLeft);
  d.drawText("Raw file deletion/export comes after filesystem HAL support.",
             x + 18, y + 64, 1, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
}

}  // namespace tabos
