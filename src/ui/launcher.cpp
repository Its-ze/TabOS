#include "launcher.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "theme.h"

namespace tabos {

namespace {
constexpr uint8_t GridCols = 3;
constexpr uint8_t GridRows = 2;

struct TileRect {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
};

TileRect tileRect(uint8_t index) {
  HalDisplay& d = display();
  const int16_t top = config::StatusBarHeight + 76;
  const int16_t bottom = d.height() - config::BottomNavHeight - 52;
  const int16_t margin = 28;
  const int16_t gap = 18;
  const int16_t tileW = (d.width() - margin * 2 - gap * (GridCols - 1)) / GridCols;
  const int16_t tileH = (bottom - top - gap * (GridRows - 1)) / GridRows;
  const int16_t col = index % GridCols;
  const int16_t row = index / GridCols;
  return TileRect{static_cast<int16_t>(margin + col * (tileW + gap)),
                  static_cast<int16_t>(top + row * (tileH + gap)),
                  tileW,
                  tileH};
}
}  // namespace

void Launcher::draw(const AppManager& apps) {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t h = d.height();
  const int16_t top = config::StatusBarHeight;
  const int16_t contentH = h - config::StatusBarHeight - config::BottomNavHeight;

  d.fillRect(0, top, w, contentH, theme::Background);
  d.drawText(theme::pageTitle(_page), 30, top + 26, 4, theme::Text,
             theme::Background, TextAlign::MiddleLeft);

  char pageText[20];
  snprintf(pageText, sizeof(pageText), "%u / %u", _page + 1,
           config::LauncherPageCount);
  d.drawText(pageText, w - 30, top + 26, 2, theme::MutedText, theme::Background,
             TextAlign::MiddleRight);

  uint8_t tileIndex = 0;
  for (size_t i = 0; i < apps.appCount(); ++i) {
    const AppSlot& slot = apps.slot(i);
    if (slot.page != _page || slot.app == nullptr) {
      continue;
    }
    if (tileIndex >= GridCols * GridRows) {
      break;
    }

    TileRect r = tileRect(tileIndex++);
    d.fillRoundRect(r.x, r.y, r.w, r.h, 8, theme::Panel);
    d.drawRoundRect(r.x, r.y, r.w, r.h, 8, theme::Border);
    d.drawText(slot.app->icon(), r.x + 24, r.y + 34, 4, theme::Accent,
               theme::Panel, TextAlign::MiddleLeft);
    d.drawText(slot.app->name(), r.x + 24, r.y + r.h - 34, 2, theme::Text,
               theme::Panel, TextAlign::MiddleLeft);
  }

  const int16_t dotsY = h - config::BottomNavHeight - 24;
  const int16_t startX = w / 2 - (config::LauncherPageCount * 20) / 2;
  for (uint8_t i = 0; i < config::LauncherPageCount; ++i) {
    const uint32_t c = (i == _page) ? theme::Accent : theme::Border;
    d.fillRoundRect(startX + i * 20, dotsY, 12, 8, 4, c);
  }
}

bool Launcher::handleInput(const InputEvent& event, AppManager& apps) {
  if (event.type == InputEventType::TouchSwipeLeft) {
    if (_page + 1 < config::LauncherPageCount) {
      ++_page;
      return true;
    }
  }

  if (event.type == InputEventType::TouchSwipeRight) {
    if (_page > 0) {
      --_page;
      return true;
    }
  }

  if (event.type == InputEventType::TouchTap) {
    return openTileAt(event.x, event.y, apps);
  }

  return false;
}

bool Launcher::openTileAt(int16_t x, int16_t y, AppManager& apps) {
  uint8_t tileIndex = 0;
  for (size_t i = 0; i < apps.appCount(); ++i) {
    const AppSlot& slot = apps.slot(i);
    if (slot.page != _page || slot.app == nullptr) {
      continue;
    }
    if (tileIndex >= GridCols * GridRows) {
      break;
    }

    TileRect r = tileRect(tileIndex++);
    if (x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h) {
      return apps.open(slot.app);
    }
  }
  return false;
}

}  // namespace tabos
