#pragma once

#include "../core/settings.h"
#include "../core/utility_manager.h"
#include "app_base.h"

namespace tabos {

class FlashlightApp : public AppBase {
 public:
  FlashlightApp(UtilityManager& utilities, SettingsManager& settings);

  const char* id() const override { return "flashlight"; }
  const char* name() const override { return "Flashlight"; }
  const char* icon() const override { return "FL"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;
  bool wantsKeepAwake() const override {
    return _utilities.flashlightMode() != FlashlightMode::Off;
  }

 private:
  void applyBrightness();

  UtilityManager& _utilities;
  SettingsManager& _settings;
};

}  // namespace tabos
