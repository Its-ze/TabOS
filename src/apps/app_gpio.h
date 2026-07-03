#pragma once

#include "../core/hardware_manager.h"
#include "app_base.h"

namespace tabos {

class GpioApp : public AppBase {
 public:
  explicit GpioApp(HardwareManager& hardware);

  const char* id() const override { return "gpio"; }
  const char* name() const override { return "GPIO"; }
  const char* icon() const override { return "GP"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  HardwareManager& _hardware;
};

}  // namespace tabos
