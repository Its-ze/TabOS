#pragma once

#include "app_base.h"

namespace tabos {

class QrCodeApp : public AppBase {
 public:
  const char* id() const override { return "qr"; }
  const char* name() const override { return "QR Code"; }
  const char* icon() const override { return "QR"; }
  void draw() override;

 private:
  void drawFinder(int16_t x, int16_t y, int16_t scale);
  void drawMatrix(int16_t x, int16_t y, int16_t scale);
};

}  // namespace tabos
