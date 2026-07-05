#include "app_qr_code.h"

#include <stddef.h>

#include <M5GFX.h>
#include <lgfx/utility/lgfx_qrcode.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

namespace {
constexpr const char* kQrPayload = "https://github.com/Its-ze/TabOS";
constexpr uint8_t kQrVersion = 4;
constexpr uint8_t kQrModules = kQrVersion * 4 + 17;
constexpr size_t kQrBufferSize = ((kQrModules * kQrModules) + 7) / 8;
}  // namespace

void QrCodeApp::draw() {
  HalDisplay& d = display();
  const int16_t top = config::StatusBarHeight;
  d.fillRect(0, top, d.width(), d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("QR", 36, top + 52, 4, theme::Accent, theme::Background,
             TextAlign::MiddleLeft);
  d.drawText("QR Code", 112, top + 52, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  const int16_t scale = 7;
  const int16_t matrixX = 54;
  const int16_t matrixY = top + 116;
  drawMatrix(matrixX, matrixY, scale, kQrPayload);

  const int16_t cardX = matrixX + 300;
  d.fillRoundRect(cardX, matrixY, d.width() - cardX - 42, 188, 8, theme::Panel);
  d.drawRoundRect(cardX, matrixY, d.width() - cardX - 42, 188, 8, theme::Border);
  d.drawText("Device Card", cardX + 24, matrixY + 36, 2, theme::Accent,
             theme::Panel, TextAlign::MiddleLeft);
  d.drawText(TABOS_NAME, cardX + 24, matrixY + 82, 4, theme::Text,
             theme::Panel, TextAlign::MiddleLeft);
  d.drawText(TABOS_VERSION, cardX + 24, matrixY + 132, 2, theme::MutedText,
             theme::Panel, TextAlign::MiddleLeft);
  d.drawText(kQrPayload, cardX + 24, matrixY + 166, 1, theme::MutedText,
             theme::Panel, TextAlign::MiddleLeft);
}

void QrCodeApp::drawMatrix(int16_t x, int16_t y, int16_t scale,
                           const char* payload) {
  HalDisplay& d = display();
  uint8_t modules[kQrBufferSize];
  QRCode qrcode;
  const int8_t status =
      lgfx_qrcode_initText(&qrcode, modules, kQrVersion, ECC_MEDIUM, payload);

  const int16_t quiet = 4 * scale;
  const int16_t side = qrcode.size * scale;
  d.fillRoundRect(x - quiet, y - quiet, side + quiet * 2, side + quiet * 2, 8,
                  theme::Text);

  if (status < 0) {
    d.drawText("QR encode failed", x + side / 2, y + side / 2, 2,
               theme::Error, theme::Text, TextAlign::MiddleCenter);
    return;
  }

  for (uint8_t row = 0; row < qrcode.size; ++row) {
    for (uint8_t col = 0; col < qrcode.size; ++col) {
      d.fillRect(x + col * scale, y + row * scale, scale, scale,
                 lgfx_qrcode_getModule(&qrcode, col, row) ? theme::Black
                                                           : theme::Text);
    }
  }
}

}  // namespace tabos
