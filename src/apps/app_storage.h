#pragma once

#include "../core/utility_manager.h"
#include "app_base.h"

namespace tabos {

class StorageApp : public AppBase {
 public:
  explicit StorageApp(UtilityManager& utilities);

  const char* id() const override { return "storage"; }
  const char* name() const override { return "Storage"; }
  const char* icon() const override { return "SD"; }
  void draw() override;

 private:
  UtilityManager& _utilities;
};

}  // namespace tabos
