#pragma once

#include "../core/utility_manager.h"
#include "app_base.h"

namespace tabos {

class NotesApp : public AppBase {
 public:
  explicit NotesApp(UtilityManager& utilities);

  const char* id() const override { return "notes"; }
  const char* name() const override { return "Notes"; }
  const char* icon() const override { return "NT"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  UtilityManager& _utilities;
};

}  // namespace tabos
