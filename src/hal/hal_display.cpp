#include "hal_display.h"

#include <M5Unified.h>

namespace tabos {

namespace {
HalDisplay gDisplay;
}

HalDisplay& display() {
  return gDisplay;
}

bool HalDisplay::begin(Logger& logger) {
  M5.begin();
  M5.Power.begin();

  if (M5.Display.width() < M5.Display.height()) {
    setRotation(1);
  } else {
    _rotation = M5.Display.getRotation();
    _width = M5.Display.width();
    _height = M5.Display.height();
  }

  M5.Display.setTextWrap(false);
  setBrightness(_brightness);
  fillScreen(0x000000);

  logger.logf(LogLevel::Info, "Display ready %dx%d", _width, _height);
  return _width > 0 && _height > 0;
}

void HalDisplay::update() {
  M5.update();
}

void HalDisplay::setBrightness(uint8_t brightness) {
  _brightness = brightness;
  M5.Display.setBrightness(brightness);
}

void HalDisplay::setRotation(uint8_t rotation) {
  _rotation = rotation & 3;
  M5.Display.setRotation(_rotation);
  _width = M5.Display.width();
  _height = M5.Display.height();
}

void HalDisplay::fillScreen(uint32_t color) {
  M5.Display.fillScreen(color);
}

void HalDisplay::fillRect(int32_t x, int32_t y, int32_t w, int32_t h,
                          uint32_t color) {
  M5.Display.fillRect(x, y, w, h, color);
}

void HalDisplay::drawRect(int32_t x, int32_t y, int32_t w, int32_t h,
                          uint32_t color) {
  M5.Display.drawRect(x, y, w, h, color);
}

void HalDisplay::fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h,
                               int32_t radius, uint32_t color) {
  M5.Display.fillRoundRect(x, y, w, h, radius, color);
}

void HalDisplay::drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h,
                               int32_t radius, uint32_t color) {
  M5.Display.drawRoundRect(x, y, w, h, radius, color);
}

void HalDisplay::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                          uint32_t color) {
  M5.Display.drawLine(x0, y0, x1, y1, color);
}

void HalDisplay::drawText(const char* text, int32_t x, int32_t y, uint8_t font,
                          uint32_t fg, uint32_t bg, TextAlign align) {
  if (text == nullptr) {
    return;
  }

  M5.Display.setTextFont(font);
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(fg, bg);

  const int32_t textW = M5.Display.textWidth(text);
  const int32_t textH = M5.Display.fontHeight();
  int32_t drawX = x;
  int32_t drawY = y;

  switch (align) {
    case TextAlign::TopCenter:
      drawX -= textW / 2;
      break;
    case TextAlign::TopRight:
      drawX -= textW;
      break;
    case TextAlign::MiddleLeft:
      drawY -= textH / 2;
      break;
    case TextAlign::MiddleCenter:
      drawX -= textW / 2;
      drawY -= textH / 2;
      break;
    case TextAlign::MiddleRight:
      drawX -= textW;
      drawY -= textH / 2;
      break;
    case TextAlign::TopLeft:
    default:
      break;
  }

  M5.Display.setCursor(drawX, drawY);
  M5.Display.print(text);
}

void HalDisplay::drawProgress(int32_t x, int32_t y, int32_t w, int32_t h,
                              uint8_t value, uint32_t fill, uint32_t track,
                              uint32_t outline) {
  value = constrain(value, 0, 100);
  fillRoundRect(x, y, w, h, 4, track);
  const int32_t filled = (static_cast<int32_t>(value) * w) / 100;
  if (filled > 0) {
    fillRoundRect(x, y, filled, h, 4, fill);
  }
  drawRoundRect(x, y, w, h, 4, outline);
}

}  // namespace tabos
