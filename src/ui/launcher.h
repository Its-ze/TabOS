#pragma once

#include "../core/app_manager.h"
#include "../core/input_event.h"

namespace tabos {

class Launcher {
 public:
  void draw(const AppManager& apps);
  bool handleInput(const InputEvent& event, AppManager& apps);
  uint8_t page() const { return _page; }

 private:
  bool openTileAt(int16_t x, int16_t y, AppManager& apps);
  uint8_t _page = 0;
};

}  // namespace tabos
