#pragma once

#include <stdint.h>

#define TABOS_NAME "TabOS"
#define TABOS_VERSION "0.5.8-m5"
#define TABOS_BUILD_DATE __DATE__ " " __TIME__

namespace tabos {
namespace config {
constexpr uint8_t MaxApps = 30;
constexpr uint8_t LauncherPageCount = 5;
constexpr uint8_t MaxLogEntries = 64;
constexpr uint16_t StatusBarHeight = 48;
constexpr uint16_t BottomNavHeight = 72;
constexpr uint32_t StatusRefreshMs = 1000;
constexpr uint32_t BatteryRefreshMs = 5000;
}  // namespace config
}  // namespace tabos
