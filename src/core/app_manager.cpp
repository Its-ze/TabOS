#include "app_manager.h"

#include <string.h>

namespace tabos {

void AppManager::begin(Logger& logger) {
  _logger = &logger;
  _logger->info("App manager started");
}

bool AppManager::registerApp(AppBase& app, uint8_t page) {
  if (_count >= config::MaxApps) {
    if (_logger) {
      _logger->error("App registration failed; app table full");
    }
    return false;
  }

  _slots[_count].app = &app;
  _slots[_count].page = page;
  ++_count;
  app.setup();

  if (_logger) {
    _logger->logf(LogLevel::Debug, "Registered app %s", app.name());
  }
  return true;
}

bool AppManager::open(AppBase* app) {
  if (app == nullptr) {
    return false;
  }

  if (_active == app) {
    return true;
  }

  if (_active) {
    _active->onClose();
  }
  _active = app;
  _active->onOpen();

  if (_logger) {
    _logger->logf(LogLevel::Info, "Opened app %s", _active->name());
  }
  return true;
}

bool AppManager::openById(const char* id) {
  if (id == nullptr) {
    return false;
  }

  for (size_t i = 0; i < _count; ++i) {
    if (_slots[i].app && strcmp(_slots[i].app->id(), id) == 0) {
      return open(_slots[i].app);
    }
  }
  return false;
}

void AppManager::home() {
  if (_active) {
    _active->onClose();
    _active = nullptr;
    if (_logger) {
      _logger->info("Returned home");
    }
  }
}

void AppManager::back() {
  home();
}

const AppSlot& AppManager::slot(size_t index) const {
  static AppSlot empty;
  if (index >= _count) {
    return empty;
  }
  return _slots[index];
}

}  // namespace tabos
