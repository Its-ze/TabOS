#include "app_qr_code.h"

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

void QrCodeApp::draw() {
  HalDisplay& d = display();
  const int16_t top = config::StatusBarHeight;
  d.fillRect(0, top, d.width(), d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("QR", 36, top + 52, 4, theme::Accent, theme::Background,
             TextAlign::MiddleLeft);
  d.drawText("QR Code", 112, top + 52, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const int16_t scale = 10;
  const int16_t matrixX = 54;
  const int16_t matrixY = top + 116;
  drawMatrix(matrixX, matrixY, scale);

  const int16_t cardX = matrixX + 250;
  d.fillRoundRect(cardX, matrixY, d.width() - cardX - 42, 188, 8, theme::Panel);
  d.drawRoundRect(cardX, matrixY, d.width() - cardX - 42, 188, 8, theme::Border);
  d.drawText("Device Card", cardX + 24, matrixY + 36, 2, theme::Accent,
             theme::Panel, TextAlign::MiddleLeft);
  d.drawText(TABOS_NAME, cardX + 24, matrixY + 82, 4, theme::Text,
             theme::Panel, TextAlign::MiddleLeft);
  d.drawText(TABOS_VERSION, cardX + 24, matrixY + 132, 2, theme::MutedText,
             theme::Panel, TextAlign::MiddleLeft);
}

void QrCodeApp::drawFinder(int16_t x, int16_t y, int16_t scale) {
  HalDisplay& d = display();
  d.fillRect(x, y, 7 * scale, 7 * scale, theme::Text);
  d.fillRect(x + scale, y + scale, 5 * scale, 5 * scale, theme::Background);
  d.fillRect(x + 2 * scale, y + 2 * scale, 3 * scale, 3 * scale, theme::Text);
}

void QrCodeApp::drawMatrix(int16_t x, int16_t y, int16_t scale) {
  HalDisplay& d = display();
  constexpr uint8_t modules = 21;
  d.fillRoundRect(x - scale, y - scale, (modules + 2) * scale,
                  (modules + 2) * scale, 8, theme::Text);
  d.fillRect(x, y, modules * scale, modules * scale, theme::Background);

  drawFinder(x, y, scale);
  drawFinder(x + 14 * scale, y, scale);
  drawFinder(x, y + 14 * scale, scale);

  uint32_t seed = 0x5A17u;
  for (const char* p = TABOS_VERSION; *p; ++p) {
    seed = (seed * 33u) ^ static_cast<uint8_t>(*p);
  }

  for (uint8_t row = 0; row < modules; ++row) {
    for (uint8_t col = 0; col < modules; ++col) {
      const bool inFinder =
          (row < 7 && col < 7) || (row < 7 && col >= 14) ||
          (row >= 14 && col < 7);
      if (inFinder) {
        continue;
      }
      const uint32_t v = (row * 29u + col * 17u + seed + row * col) & 0x07u;
      if (v == 0 || v == 3 || v == 5) {
        d.fillRect(x + col * scale, y + row * scale, scale, scale,
                   theme::Text);
      }
    }
  }
}

}  // namespace tabos
