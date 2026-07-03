#pragma once

#include "../core/terminal_manager.h"
#include "app_base.h"

namespace tabos {

class TerminalApp : public AppBase {
 public:
  explicit TerminalApp(TerminalManager& terminal);

  const char* id() const override { return "terminal"; }
  const char* name() const override { return "Terminal"; }
  const char* icon() const override { return "TR"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;
  bool wantsKeepAwake() const override { return true; }

 private:
  TerminalManager& _terminal;
};

}  // namespace tabos
