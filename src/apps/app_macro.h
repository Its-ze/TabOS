#pragma once

#include "../core/bluetooth_manager.h"
#include "../core/hardware_manager.h"
#include "../core/mesh_manager.h"
#include "../core/utility_manager.h"
#include "../core/wifi_manager.h"
#include "app_base.h"

namespace tabos {

class MacroApp : public AppBase {
 public:
  MacroApp(UtilityManager& utilities, WifiManager& wifi,
           BluetoothManager& bluetooth, MeshManager& mesh,
           HardwareManager& hardware);

  const char* id() const override { return "macro"; }
  const char* name() const override { return "Macro Pad"; }
  const char* icon() const override { return "MC"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  UtilityManager& _utilities;
  WifiManager& _wifi;
  BluetoothManager& _bluetooth;
  MeshManager& _mesh;
  HardwareManager& _hardware;
};

}  // namespace tabos
