#pragma once

#include "../core/hardware_manager.h"
#include "app_base.h"

namespace tabos {

class IrRemoteApp : public AppBase {
 public:
  explicit IrRemoteApp(HardwareManager& hardware);

  const char* id() const override { return "ir"; }
  const char* name() const override { return "IR Remote"; }
  const char* icon() const override { return "IR"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  HardwareManager& _hardware;
};

}  // namespace tabos
