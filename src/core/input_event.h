#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace tabos {

enum class InputEventType : uint8_t {
  None,
  TouchActivity,
  TouchTap,
  TouchLongPress,
  TouchSwipeLeft,
  TouchSwipeRight,
  TouchSwipeUp,
  TouchSwipeDown,
  ButtonBack,
  ButtonHome
};

struct InputEvent {
  InputEventType type = InputEventType::None;
  int16_t x = 0;
  int16_t y = 0;
  int16_t dx = 0;
  int16_t dy = 0;
  uint32_t at = 0;
};

inline bool isTouchEvent(InputEventType type) {
  return type == InputEventType::TouchActivity ||
         type == InputEventType::TouchTap ||
         type == InputEventType::TouchLongPress ||
         type == InputEventType::TouchSwipeLeft ||
         type == InputEventType::TouchSwipeRight ||
         type == InputEventType::TouchSwipeUp ||
         type == InputEventType::TouchSwipeDown;
}

}  // namespace tabos
