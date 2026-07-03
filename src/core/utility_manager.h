#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <stdint.h>

#include "logger.h"

namespace tabos {

constexpr uint8_t MaxNotes = 6;
constexpr uint8_t MaxTasks = 8;
constexpr uint8_t MaxMacroEvents = 8;

struct NoteItem {
  char title[28] = {0};
  char body[90] = {0};
  uint32_t updatedAt = 0;
};

struct TaskItem {
  char text[48] = {0};
  bool done = false;
};

struct MacroEvent {
  uint32_t at = 0;
  char text[64] = {0};
};

enum class FlashlightMode : uint8_t {
  Off,
  White,
  Red
};

class UtilityManager {
 public:
  void begin(Logger& logger);
  void loop();

  uint8_t noteCount() const { return _noteCount; }
  const NoteItem& note(uint8_t index) const;
  void addNote();
  void deleteLastNote();

  uint8_t taskCount() const { return _taskCount; }
  const TaskItem& task(uint8_t index) const;
  void addTask();
  void toggleTask(uint8_t index);
  void clearDoneTasks();

  void startTimer(uint32_t seconds);
  void stopTimer();
  void resetTimer();
  bool timerRunning() const { return _timerRunning; }
  uint32_t timerRemainingSeconds() const;

  void startStopwatch();
  void stopStopwatch();
  void resetStopwatch();
  bool stopwatchRunning() const { return _stopwatchRunning; }
  uint32_t stopwatchSeconds() const;

  FlashlightMode flashlightMode() const { return _flashlightMode; }
  uint8_t flashlightBrightness() const { return _flashlightBrightness; }
  void setFlashlightMode(FlashlightMode mode);
  void cycleFlashlightBrightness();
  const char* flashlightModeText() const;

  void addMacroEvent(const char* text);
  uint8_t macroEventCount() const { return _macroEventCount; }
  const MacroEvent& macroEvent(uint8_t index) const;

  uint8_t virtualFileCount() const { return 4; }
  const char* virtualFileName(uint8_t index) const;
  void virtualFileInfo(uint8_t index, char* out, size_t outLen) const;
  bool storageAvailable() const { return _storageAvailable; }

 private:
  void load();
  void saveNotes();
  void saveTasks();
  void log(const char* text);

  Preferences _prefs;
  Logger* _logger = nullptr;
  bool _storageAvailable = false;

  NoteItem _notes[MaxNotes];
  uint8_t _noteCount = 0;
  uint8_t _nextNoteId = 1;

  TaskItem _tasks[MaxTasks];
  uint8_t _taskCount = 0;
  uint8_t _nextTaskId = 0;

  bool _timerRunning = false;
  uint32_t _timerEndAt = 0;
  uint32_t _timerLastDuration = 300;

  bool _stopwatchRunning = false;
  uint32_t _stopwatchStartedAt = 0;
  uint32_t _stopwatchAccumulatedMs = 0;

  FlashlightMode _flashlightMode = FlashlightMode::Off;
  uint8_t _flashlightBrightness = 192;

  MacroEvent _macroEvents[MaxMacroEvents];
  uint8_t _macroEventCount = 0;
};

}  // namespace tabos
