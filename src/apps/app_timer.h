#pragma once

#include "../core/utility_manager.h"
#include "app_base.h"

namespace tabos {

class TimerApp : public AppBase {
 public:
  explicit TimerApp(UtilityManager& utilities);

  const char* id() const override { return "timer"; }
  const char* name() const override { return "Timer"; }
  const char* icon() const override { return "TM"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;
  bool wantsKeepAwake() const override { return _utilities.timerRunning(); }

 private:
  UtilityManager& _utilities;
};

}  // namespace tabos
