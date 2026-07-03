#pragma once

#include "../core/hardware_manager.h"
#include "../core/motion_manager.h"
#include "app_base.h"

namespace tabos {

class SensorsApp : public AppBase {
 public:
  SensorsApp(HardwareManager& hardware, MotionManager& motion);

  const char* id() const override { return "sensors"; }
  const char* name() const override { return "Sensors"; }
  const char* icon() const override { return "SN"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  void drawMetric(uint8_t index, const char* label, const char* value);

  HardwareManager& _hardware;
  MotionManager& _motion;
};

}  // namespace tabos
