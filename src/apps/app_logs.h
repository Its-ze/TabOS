#pragma once

#include "../core/logger.h"
#include "app_base.h"

namespace tabos {

class LogsApp : public AppBase {
 public:
  explicit LogsApp(Logger& logger);

  const char* id() const override { return "logs"; }
  const char* name() const override { return "Logs"; }
  const char* icon() const override { return "LG"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  Logger& _logger;
};

}  // namespace tabos
