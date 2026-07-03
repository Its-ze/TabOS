#pragma once

#include "../core/hardware_manager.h"
#include "app_base.h"

namespace tabos {

class RelayApp : public AppBase {
 public:
  explicit RelayApp(HardwareManager& hardware);

  const char* id() const override { return "relay"; }
  const char* name() const override { return "Relay"; }
  const char* icon() const override { return "RY"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  HardwareManager& _hardware;
};

}  // namespace tabos
