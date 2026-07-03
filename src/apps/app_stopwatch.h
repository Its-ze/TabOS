#pragma once

#include "../core/utility_manager.h"
#include "app_base.h"

namespace tabos {

class StopwatchApp : public AppBase {
 public:
  explicit StopwatchApp(UtilityManager& utilities);

  const char* id() const override { return "stopwatch"; }
  const char* name() const override { return "Stopwatch"; }
  const char* icon() const override { return "SW"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;
  bool wantsKeepAwake() const override { return _utilities.stopwatchRunning(); }

 private:
  UtilityManager& _utilities;
};

}  // namespace tabos
