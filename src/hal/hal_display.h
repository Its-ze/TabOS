#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "../core/logger.h"

namespace tabos {

enum class TextAlign : uint8_t {
  TopLeft,
  TopCenter,
  TopRight,
  MiddleLeft,
  MiddleCenter,
  MiddleRight
};

class HalDisplay {
 public:
  bool begin(Logger& logger);
  void update();

  int16_t width() const { return _width; }
  int16_t height() const { return _height; }

  void setBrightness(uint8_t brightness);
  uint8_t brightness() const { return _brightness; }

  void beginWrite();
  void endWrite();
  void fillScreen(uint32_t color);
  void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
  void drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
  void fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h,
                     int32_t radius, uint32_t color);
  void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h,
                     int32_t radius, uint32_t color);
  void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);
  void drawText(const char* text, int32_t x, int32_t y, uint8_t font,
                uint32_t fg, uint32_t bg, TextAlign align = TextAlign::TopLeft);
  void drawProgress(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t value,
                    uint32_t fill, uint32_t track, uint32_t outline);

 private:
  int16_t _width = 0;
  int16_t _height = 0;
  uint8_t _brightness = 192;
};

HalDisplay& display();

}  // namespace tabos
