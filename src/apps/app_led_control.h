#pragma once

#include "../core/hardware_manager.h"
#include "app_base.h"

namespace tabos {

class LedControlApp : public AppBase {
 public:
  explicit LedControlApp(HardwareManager& hardware);

  const char* id() const override { return "led"; }
  const char* name() const override { return "LED Control"; }
  const char* icon() const override { return "LD"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  HardwareManager& _hardware;
};

}  // namespace tabos
