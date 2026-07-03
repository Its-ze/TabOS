#include "logger.h"

#include <stdarg.h>
#include <string.h>

namespace tabos {

void Logger::begin() {
  if (!_started) {
    Serial.begin(115200);
    _started = true;
  }
  info("Logger started");
}

void Logger::clear() {
  _next = 0;
  _count = 0;
  memset(_entries, 0, sizeof(_entries));
  info("Logs cleared");
}

void Logger::log(LogLevel level, const char* message) {
  LogEntry& slot = _entries[_next];
  slot.at = millis();
  slot.level = level;
  strlcpy(slot.message, message ? message : "", sizeof(slot.message));

  _next = (_next + 1) % config::MaxLogEntries;
  if (_count < config::MaxLogEntries) {
    ++_count;
  }

  if (_started) {
    Serial.printf("[%s] %lu %s\n", levelName(level),
                  static_cast<unsigned long>(slot.at), slot.message);
  }
}

void Logger::logf(LogLevel level, const char* format, ...) {
  char buffer[128];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  log(level, buffer);
}

const LogEntry& Logger::entry(size_t index) const {
  static LogEntry empty;
  if (index >= _count) {
    return empty;
  }
  const size_t oldest = (_count == config::MaxLogEntries) ? _next : 0;
  const size_t realIndex = (oldest + index) % config::MaxLogEntries;
  return _entries[realIndex];
}

const char* Logger::levelName(LogLevel level) {
  switch (level) {
    case LogLevel::Debug:
      return "DEBUG";
    case LogLevel::Info:
      return "INFO";
    case LogLevel::Warn:
      return "WARN";
    case LogLevel::Error:
      return "ERROR";
    case LogLevel::Critical:
      return "CRITICAL";
  }
  return "INFO";
}

}  // namespace tabos
