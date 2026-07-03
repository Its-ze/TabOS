#pragma once

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

#include "../config.h"

namespace tabos {

enum class LogLevel : uint8_t {
  Debug,
  Info,
  Warn,
  Error,
  Critical
};

struct LogEntry {
  uint32_t at = 0;
  LogLevel level = LogLevel::Info;
  char message[96] = {0};
};

class Logger {
 public:
  void begin();
  void clear();
  void log(LogLevel level, const char* message);
  void logf(LogLevel level, const char* format, ...);

  void debug(const char* message) { log(LogLevel::Debug, message); }
  void info(const char* message) { log(LogLevel::Info, message); }
  void warn(const char* message) { log(LogLevel::Warn, message); }
  void error(const char* message) { log(LogLevel::Error, message); }
  void critical(const char* message) { log(LogLevel::Critical, message); }

  size_t count() const { return _count; }
  const LogEntry& entry(size_t index) const;
  static const char* levelName(LogLevel level);

 private:
  LogEntry _entries[config::MaxLogEntries];
  size_t _next = 0;
  size_t _count = 0;
  bool _started = false;
};

}  // namespace tabos
