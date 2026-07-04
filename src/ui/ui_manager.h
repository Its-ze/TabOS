#pragma once

#include "../core/app_manager.h"
#include "../core/battery.h"
#include "../core/bluetooth_manager.h"
#include "../core/input_event.h"
#include "../core/logger.h"
#include "../core/mesh_manager.h"
#include "../core/settings.h"
#include "../core/wifi_manager.h"
#include "bottom_nav.h"
#include "launcher.h"
#include "quick_settings.h"
#include "status_bar.h"

namespace tabos {

class UiManager {
 public:
  void begin(AppManager& apps, SettingsManager& settings, BatteryManager& battery,
             WifiManager& wifi, BluetoothManager& bluetooth, MeshManager& mesh,
             Logger& logger);
  void loop();
  void draw();
  void handleInput(const InputEvent& event);
  void requestRedraw() { _dirty = true; }

 private:
  enum class Transition : uint8_t {
    None,
    SlideLeft,
    SlideRight,
    OpenApp,
    BackHome
  };

  const char* currentTitle() const;
  void handleNav(NavAction action);
  void startTransition(Transition transition);
  void playTransition();

  AppManager* _apps = nullptr;
  SettingsManager* _settings = nullptr;
  BatteryManager* _battery = nullptr;
  MeshManager* _mesh = nullptr;
  StatusBar _statusBar;
  BottomNav _bottomNav;
  Launcher _launcher;
  QuickSettings _quickSettings;
  bool _dirty = true;
  Transition _transition = Transition::None;
  uint32_t _lastStatusDraw = 0;
};

}  // namespace tabos
