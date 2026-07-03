#pragma once

#include "../core/battery.h"
#include "app_base.h"

namespace tabos {

class BatteryApp : public AppBase {
 public:
  explicit BatteryApp(BatteryManager& battery);

  const char* id() const override { return "battery"; }
  const char* name() const override { return "Battery"; }
  const char* icon() const override { return "BT"; }
  void draw() override;

 private:
  BatteryManager& _battery;
};

}  // namespace tabos
