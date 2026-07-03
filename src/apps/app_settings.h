#pragma once

#include "../core/logger.h"
#include "../core/settings.h"
#include "app_base.h"

namespace tabos {

class SettingsApp : public AppBase {
 public:
  SettingsApp(SettingsManager& settings, Logger& logger);

  const char* id() const override { return "settings"; }
  const char* name() const override { return "Settings"; }
  const char* icon() const override { return "ST"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  void drawRow(uint8_t row, const char* label, const char* value, bool accent);
  void cycleBrightness();

  SettingsManager& _settings;
  Logger& _logger;
};

}  // namespace tabos
