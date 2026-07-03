#pragma once

#include "app_base.h"

namespace tabos {

class AboutApp : public AppBase {
 public:
  const char* id() const override { return "about"; }
  const char* name() const override { return "About"; }
  const char* icon() const override { return "AB"; }
  void draw() override;
};

}  // namespace tabos
