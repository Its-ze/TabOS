#pragma once

#include "../core/app_manager.h"
#include "../core/logger.h"
#include "app_base.h"

namespace tabos {

class DeveloperApp : public AppBase {
 public:
  DeveloperApp(AppManager& apps, Logger& logger);

  const char* id() const override { return "developer"; }
  const char* name() const override { return "Developer"; }
  const char* icon() const override { return "DV"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  AppManager& _apps;
  Logger& _logger;
};

}  // namespace tabos
