#include "terminal_manager.h"

#include "mesh_manager.h"

#include <string.h>
#include <strings.h>

namespace tabos {

void TerminalManager::begin(Logger& logger) {
  _logger = &logger;
  clear();
  printLine("TabOS terminal ready");
  printLine("Type help in the USB serial monitor");
  _logger->info("Terminal manager started");
}

void TerminalManager::attachMesh(MeshManager& mesh) {
  _mesh = &mesh;
}

void TerminalManager::loop() {
  while (Serial.available() > 0) {
    const char c = static_cast<char>(Serial.read());
    if (c == '\r') {
      continue;
    }
    if (c == '\n') {
      _input[_inputLen] = '\0';
      if (_inputLen > 0) {
        char echo[104];
        snprintf(echo, sizeof(echo), "> %s", _input);
        printLine(echo);
        handleCommand(_input);
        _inputLen = 0;
      }
      continue;
    }

    if ((c == '\b' || c == 127) && _inputLen > 0) {
      --_inputLen;
      continue;
    }

    if (_inputLen + 1 < sizeof(_input) && c >= 32 && c <= 126) {
      _input[_inputLen++] = c;
    }
  }
}

void TerminalManager::clear() {
  memset(_lines, 0, sizeof(_lines));
  _lineCount = 0;
}

void TerminalManager::printLine(const char* line) {
  if (_lineCount < MaxTerminalLines) {
    strlcpy(_lines[_lineCount++], line ? line : "", sizeof(_lines[0]));
  } else {
    for (uint8_t i = 1; i < MaxTerminalLines; ++i) {
      strlcpy(_lines[i - 1], _lines[i], sizeof(_lines[0]));
    }
    strlcpy(_lines[MaxTerminalLines - 1], line ? line : "", sizeof(_lines[0]));
  }

  if (line) {
    Serial.println(line);
  }
}

const char* TerminalManager::line(uint8_t index) const {
  if (index >= _lineCount) {
    return "";
  }
  return _lines[index];
}

void TerminalManager::handleCommand(const char* command) {
  if (_mesh != nullptr && _mesh->handleTextLine(command)) {
    printLine("Mesh command accepted");
  } else if (strcasecmp(command, "help") == 0) {
    printLine("help, status, version, clear, logs");
    printLine("mesh connect, mesh demo, mesh msg text");
  } else if (strcasecmp(command, "status") == 0) {
    printLine("UI alive; touch and background tasks running");
  } else if (strcasecmp(command, "version") == 0) {
    printLine(TABOS_VERSION);
  } else if (strcasecmp(command, "clear") == 0) {
    clear();
    printLine("Terminal cleared");
  } else if (strcasecmp(command, "logs") == 0) {
    printLine("Open Logs app for the ring buffer");
  } else {
    printLine("Unknown command");
  }

  if (_logger) {
    _logger->logf(LogLevel::Info, "Terminal command: %s", command);
  }
}

}  // namespace tabos
