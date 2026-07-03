#pragma once

#include "../core/logger.h"
#include "../core/settings.h"
#include "../core/wifi_manager.h"
#include "app_base.h"

namespace tabos {

class WifiApp : public AppBase {
 public:
  WifiApp(SettingsManager& settings, WifiManager& wifi, Logger& logger);

  const char* id() const override { return "wifi"; }
  const char* name() const override { return "Wi-Fi"; }
  const char* icon() const override { return "WF"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  SettingsManager& _settings;
  WifiManager& _wifi;
  Logger& _logger;
};

}  // namespace tabos
