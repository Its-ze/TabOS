#include "hal_touch.h"

#include <M5Unified.h>
#include <stdlib.h>

namespace tabos {

namespace {
HalTouch gTouch;
}

HalTouch& touch() {
  return gTouch;
}

bool HalTouch::begin(Logger& logger) {
  logger.info("Touch ready");
  return true;
}

bool HalTouch::poll(InputEvent& event) {
  event = InputEvent{};
  event.at = millis();

  const auto detail = M5.Touch.getDetail();
  const auto count = M5.Touch.getCount();

  if (count == 0) {
    _pressed = false;
  }

  if (detail.wasFlicked()) {
    event.x = detail.x;
    event.y = detail.y;
    event.dx = detail.distanceX();
    event.dy = detail.distanceY();

    if (abs(event.dx) > abs(event.dy)) {
      event.type = event.dx < 0 ? InputEventType::TouchSwipeLeft
                                : InputEventType::TouchSwipeRight;
    } else {
      event.type = event.dy < 0 ? InputEventType::TouchSwipeUp
                                : InputEventType::TouchSwipeDown;
    }
    return true;
  }

  if (detail.wasHold()) {
    event.type = InputEventType::TouchLongPress;
    event.x = detail.x;
    event.y = detail.y;
    return true;
  }

  if (detail.wasClicked()) {
    event.type = InputEventType::TouchTap;
    event.x = detail.x;
    event.y = detail.y;
    return true;
  }

  if (count > 0 && !_pressed) {
    _pressed = true;
    event.type = InputEventType::TouchActivity;
    event.x = detail.x;
    event.y = detail.y;
    return true;
  }

  return false;
}

}  // namespace tabos
