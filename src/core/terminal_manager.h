#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "../config.h"
#include "logger.h"

namespace tabos {

constexpr uint8_t MaxTerminalLines = 12;

class MeshManager;
class MotionManager;

class TerminalManager {
 public:
  void begin(Logger& logger);
  void attachMesh(MeshManager& mesh);
  void attachMotion(MotionManager& motion);
  void loop();
  void clear();
  void printLine(const char* line);

  uint8_t lineCount() const { return _lineCount; }
  const char* line(uint8_t index) const;

 private:
  void handleCommand(const char* command);

  Logger* _logger = nullptr;
  MeshManager* _mesh = nullptr;
  MotionManager* _motion = nullptr;
  char _lines[MaxTerminalLines][80];
  uint8_t _lineCount = 0;
  char _input[96];
  uint8_t _inputLen = 0;
};

}  // namespace tabos
