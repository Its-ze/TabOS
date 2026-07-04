#pragma once

#include "../apps/app_about.h"
#include "../apps/app_atom_control.h"
#include "../apps/app_battery.h"
#include "../apps/app_bluetooth.h"
#include "../apps/app_gpio.h"
#include "../apps/app_ir_remote.h"
#include "../apps/app_led_control.h"
#include "../apps/app_logs.h"
#include "../apps/app_flashlight.h"
#include "../apps/app_macro.h"
#include "../apps/app_mesh.h"
#include "../apps/app_messages.h"
#include "../apps/app_nodes.h"
#include "../apps/app_notes.h"
#include "../apps/app_packet_log.h"
#include "../apps/app_relay.h"
#include "../apps/app_sensors.h"
#include "../apps/app_settings.h"
#include "../apps/app_storage.h"
#include "../apps/app_stopwatch.h"
#include "../apps/app_tasks.h"
#include "../apps/app_terminal.h"
#include "../apps/app_timer.h"
#include "../apps/app_wifi.h"
#include "../apps/placeholder_app.h"
#include "../ui/ui_manager.h"
#include "app_manager.h"
#include "battery.h"
#include "bluetooth_manager.h"
#include "hardware_manager.h"
#include "logger.h"
#include "mesh_manager.h"
#include "power_manager.h"
#include "settings.h"
#include "terminal_manager.h"
#include "utility_manager.h"
#include "wifi_manager.h"

namespace tabos {

class System {
 public:
  static System& instance();

  void begin();
  void loop();

  Logger& logger() { return _logger; }
  SettingsManager& settings() { return _settings; }
  AppManager& apps() { return _apps; }

 private:
  System();
  void showSplash();
  void registerApps();

  Logger _logger;
  SettingsManager _settings;
  BatteryManager _battery;
  WifiManager _wifi;
  BluetoothManager _bluetooth;
  MeshManager _mesh;
  HardwareManager _hardware;
  UtilityManager _utilities;
  TerminalManager _terminal;
  PowerManager _power;
  AppManager _apps;
  UiManager _ui;

  MeshApp _meshApp;
  MessagesApp _messagesApp;
  NodesApp _nodesApp;
  TerminalApp _terminalApp;
  SettingsApp _settingsApp;
  BatteryApp _batteryApp;

  WifiApp _wifiApp;
  BluetoothApp _bluetoothApp;
  PlaceholderApp _pingApp;
  PlaceholderApp _portCheckApp;
  PlaceholderApp _dnsApp;
  PlaceholderApp _qrApp;

  AtomControlApp _atomApp;
  IrRemoteApp _irApp;
  GpioApp _gpioApp;
  RelayApp _relayApp;
  LedControlApp _ledApp;
  SensorsApp _sensorsApp;

  LogsApp _logsApp;
  StorageApp _storageApp;
  AboutApp _aboutApp;
  PlaceholderApp _powerApp;
  PacketLogApp _packetLogApp;
  PlaceholderApp _developerApp;

  NotesApp _notesApp;
  TasksApp _tasksApp;
  TimerApp _timerApp;
  StopwatchApp _stopwatchApp;
  FlashlightApp _flashlightApp;
  MacroApp _macroApp;
};

}  // namespace tabos
