#pragma once

#include "../core/hardware_manager.h"
#include "app_base.h"

namespace tabos {

class AtomControlApp : public AppBase {
 public:
  explicit AtomControlApp(HardwareManager& hardware);

  const char* id() const override { return "atom"; }
  const char* name() const override { return "Atom Control"; }
  const char* icon() const override { return "AT"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  HardwareManager& _hardware;
};

}  // namespace tabos
