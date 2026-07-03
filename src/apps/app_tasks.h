#pragma once

#include "../core/utility_manager.h"
#include "app_base.h"

namespace tabos {

class TasksApp : public AppBase {
 public:
  explicit TasksApp(UtilityManager& utilities);

  const char* id() const override { return "tasks"; }
  const char* name() const override { return "Tasks"; }
  const char* icon() const override { return "TK"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  UtilityManager& _utilities;
};

}  // namespace tabos
