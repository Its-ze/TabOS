#pragma once

#include "../core/input_event.h"

namespace tabos {

class AppBase {
 public:
  virtual ~AppBase() = default;

  virtual const char* id() const = 0;
  virtual const char* name() const = 0;
  virtual const char* icon() const = 0;

  virtual void setup() {}
  virtual void onOpen() {}
  virtual void onClose() {}
  virtual void loop() {}
  virtual void draw() = 0;
  virtual void handleInput(const InputEvent& event) { (void)event; }

  virtual bool wantsKeepAwake() const { return false; }
  virtual bool allowsBackgroundRun() const { return false; }
};

}  // namespace tabos
