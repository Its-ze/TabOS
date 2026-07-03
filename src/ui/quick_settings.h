#pragma once

#include "../core/bluetooth_manager.h"
#include "../core/input_event.h"
#include "../core/logger.h"
#include "../core/settings.h"
#include "../core/wifi_manager.h"

namespace tabos {

class QuickSettings {
 public:
  void begin(SettingsManager& settings, WifiManager& wifi,
             BluetoothManager& bluetooth, Logger& logger);
  void open();
  void close();
  bool isOpen() const { return _open; }
  void draw();
  bool handleInput(const InputEvent& event);

 private:
  void drawToggle(uint8_t index, const char* label, const char* value, bool active);
  void handleToggle(uint8_t index);
  void cycleBrightness();

  SettingsManager* _settings = nullptr;
  WifiManager* _wifi = nullptr;
  BluetoothManager* _bluetooth = nullptr;
  Logger* _logger = nullptr;
  bool _open = false;
};

}  // namespace tabos
