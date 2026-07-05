#pragma once

#include "../core/battery.h"
#include "../core/logger.h"
#include "../core/power_manager.h"
#include "../core/settings.h"
#include "app_base.h"

namespace tabos {

class PowerApp : public AppBase {
 public:
  PowerApp(SettingsManager& settings, PowerManager& power, BatteryManager& battery,
           Logger& logger);

  const char* id() const override { return "power"; }
  const char* name() const override { return "Power"; }
  const char* icon() const override { return "PW"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  void drawToggle(uint8_t index, const char* label, const char* value, bool active);

  SettingsManager& _settings;
  PowerManager& _power;
  BatteryManager& _battery;
  Logger& _logger;
};

}  // namespace tabos
