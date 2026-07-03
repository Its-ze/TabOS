#pragma once

#include "../core/battery.h"
#include "../core/mesh_manager.h"
#include "../core/settings.h"

namespace tabos {

class StatusBar {
 public:
  void draw(const BatteryManager& battery, const SettingsManager& settings,
            const MeshManager& mesh, const char* title);
};

}  // namespace tabos
