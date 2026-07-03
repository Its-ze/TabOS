#include "main.h"

#include "core/system.h"

void tabosSetup() {
  tabos::System::instance().begin();
}

void tabosLoop() {
  tabos::System::instance().loop();
}
