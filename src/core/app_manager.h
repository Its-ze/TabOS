#pragma once

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

#include "../apps/app_base.h"
#include "../config.h"
#include "logger.h"

namespace tabos {

struct AppSlot {
  AppBase* app = nullptr;
  uint8_t page = 0;
};

class AppManager {
 public:
  void begin(Logger& logger);
  bool registerApp(AppBase& app, uint8_t page);

  bool open(AppBase* app);
  bool openById(const char* id);
  void home();
  void back();

  bool isLauncherActive() const { return _active == nullptr; }
  AppBase* activeApp() const { return _active; }

  size_t appCount() const { return _count; }
  const AppSlot& slot(size_t index) const;

 private:
  AppSlot _slots[config::MaxApps];
  size_t _count = 0;
  AppBase* _active = nullptr;
  Logger* _logger = nullptr;
};

}  // namespace tabos
