#pragma once

#include <Arduino.h>

#include "../core/input_event.h"
#include "../core/logger.h"

namespace tabos {

class HalTouch {
 public:
  bool begin(Logger& logger);
  bool poll(InputEvent& event);

 private:
  bool _pressed = false;
};

HalTouch& touch();

}  // namespace tabos
