#pragma once

#include "../core/bluetooth_manager.h"
#include "app_base.h"

namespace tabos {

class BluetoothApp : public AppBase {
 public:
  explicit BluetoothApp(BluetoothManager& bluetooth);

  const char* id() const override { return "bluetooth"; }
  const char* name() const override { return "Bluetooth"; }
  const char* icon() const override { return "BL"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  BluetoothManager& _bluetooth;
};

}  // namespace tabos
