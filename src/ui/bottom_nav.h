#pragma once

#include "../core/input_event.h"

namespace tabos {

enum class NavAction : uint8_t {
  None,
  Back,
  Home,
  Apps,
  Settings
};

class BottomNav {
 public:
  void draw(bool launcherActive);
  NavAction hitTest(const InputEvent& event) const;
};

}  // namespace tabos
