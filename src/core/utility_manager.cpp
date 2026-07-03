#include "utility_manager.h"

#include <stdio.h>
#include <string.h>

namespace tabos {

namespace {
NoteItem gEmptyNote;
TaskItem gEmptyTask;
MacroEvent gEmptyMacro;
const char* const kTaskTemplates[] = {
    "Check mesh link", "Inspect battery", "Scan Wi-Fi", "Label new node"};
constexpr uint8_t kTaskTemplateCount =
    sizeof(kTaskTemplates) / sizeof(kTaskTemplates[0]);
}

void UtilityManager::begin(Logger& logger) {
  _logger = &logger;
  _storageAvailable = _prefs.begin("tabos_util", false);
  load();
  log("Utility manager started");
}

void UtilityManager::loop() {
  if (_timerRunning && timerRemainingSeconds() == 0) {
    _timerRunning = false;
    log("Timer complete");
  }
}

const NoteItem& UtilityManager::note(uint8_t index) const {
  if (index >= _noteCount) {
    return gEmptyNote;
  }
  return _notes[index];
}

void UtilityManager::addNote() {
  if (_noteCount >= MaxNotes) {
    for (uint8_t i = 1; i < MaxNotes; ++i) {
      _notes[i - 1] = _notes[i];
    }
    _noteCount = MaxNotes - 1;
  }

  NoteItem& note = _notes[_noteCount++];
  snprintf(note.title, sizeof(note.title), "Field note %u", _nextNoteId++);
  strlcpy(note.body, "Created from the TabOS touch UI.", sizeof(note.body));
  note.updatedAt = millis() / 1000;
  saveNotes();
  log("Note added");
}

void UtilityManager::deleteLastNote() {
  if (_noteCount == 0) {
    return;
  }
  --_noteCount;
  saveNotes();
  log("Note deleted");
}

const TaskItem& UtilityManager::task(uint8_t index) const {
  if (index >= _taskCount) {
    return gEmptyTask;
  }
  return _tasks[index];
}

void UtilityManager::addTask() {
  if (_taskCount >= MaxTasks) {
    return;
  }
  TaskItem& task = _tasks[_taskCount++];
  strlcpy(task.text, kTaskTemplates[_nextTaskId % kTaskTemplateCount],
          sizeof(task.text));
  task.done = false;
  ++_nextTaskId;
  saveTasks();
  log("Task added");
}

void UtilityManager::toggleTask(uint8_t index) {
  if (index >= _taskCount) {
    return;
  }
  _tasks[index].done = !_tasks[index].done;
  saveTasks();
  log("Task toggled");
}

void UtilityManager::clearDoneTasks() {
  uint8_t write = 0;
  for (uint8_t read = 0; read < _taskCount; ++read) {
    if (!_tasks[read].done) {
      _tasks[write++] = _tasks[read];
    }
  }
  _taskCount = write;
  saveTasks();
  log("Completed tasks cleared");
}

void UtilityManager::startTimer(uint32_t seconds) {
  _timerLastDuration = seconds;
  _timerEndAt = millis() + seconds * 1000UL;
  _timerRunning = true;
  log("Timer started");
}

void UtilityManager::stopTimer() {
  _timerRunning = false;
  log("Timer stopped");
}

void UtilityManager::resetTimer() {
  _timerRunning = false;
  _timerEndAt = 0;
  log("Timer reset");
}

uint32_t UtilityManager::timerRemainingSeconds() const {
  if (!_timerRunning) {
    return _timerLastDuration;
  }
  const uint32_t now = millis();
  if (now >= _timerEndAt) {
    return 0;
  }
  return (_timerEndAt - now + 999) / 1000;
}

void UtilityManager::startStopwatch() {
  if (_stopwatchRunning) {
    return;
  }
  _stopwatchStartedAt = millis();
  _stopwatchRunning = true;
  log("Stopwatch started");
}

void UtilityManager::stopStopwatch() {
  if (!_stopwatchRunning) {
    return;
  }
  _stopwatchAccumulatedMs += millis() - _stopwatchStartedAt;
  _stopwatchRunning = false;
  log("Stopwatch stopped");
}

void UtilityManager::resetStopwatch() {
  _stopwatchRunning = false;
  _stopwatchStartedAt = 0;
  _stopwatchAccumulatedMs = 0;
  log("Stopwatch reset");
}

uint32_t UtilityManager::stopwatchSeconds() const {
  uint32_t ms = _stopwatchAccumulatedMs;
  if (_stopwatchRunning) {
    ms += millis() - _stopwatchStartedAt;
  }
  return ms / 1000;
}

void UtilityManager::setFlashlightMode(FlashlightMode mode) {
  _flashlightMode = mode;
  log("Flashlight mode changed");
}

void UtilityManager::cycleFlashlightBrightness() {
  if (_flashlightBrightness < 128) {
    _flashlightBrightness = 192;
  } else if (_flashlightBrightness < 230) {
    _flashlightBrightness = 255;
  } else {
    _flashlightBrightness = 64;
  }
  log("Flashlight brightness changed");
}

const char* UtilityManager::flashlightModeText() const {
  switch (_flashlightMode) {
    case FlashlightMode::Off:
      return "Off";
    case FlashlightMode::White:
      return "White";
    case FlashlightMode::Red:
      return "Red";
  }
  return "Off";
}

void UtilityManager::addMacroEvent(const char* text) {
  if (_macroEventCount >= MaxMacroEvents) {
    for (uint8_t i = 1; i < MaxMacroEvents; ++i) {
      _macroEvents[i - 1] = _macroEvents[i];
    }
    _macroEventCount = MaxMacroEvents - 1;
  }
  MacroEvent& event = _macroEvents[_macroEventCount++];
  event.at = millis() / 1000;
  strlcpy(event.text, text ? text : "", sizeof(event.text));
  log(event.text);
}

const MacroEvent& UtilityManager::macroEvent(uint8_t index) const {
  if (index >= _macroEventCount) {
    return gEmptyMacro;
  }
  return _macroEvents[index];
}

const char* UtilityManager::virtualFileName(uint8_t index) const {
  switch (index) {
    case 0:
      return "settings.nvs";
    case 1:
      return "notes.db";
    case 2:
      return "tasks.db";
    case 3:
      return "logs.ring";
  }
  return "--";
}

void UtilityManager::virtualFileInfo(uint8_t index, char* out, size_t outLen) const {
  switch (index) {
    case 0:
      snprintf(out, outLen, "Persistent system settings");
      break;
    case 1:
      snprintf(out, outLen, "%u notes", _noteCount);
      break;
    case 2:
      snprintf(out, outLen, "%u tasks", _taskCount);
      break;
    case 3:
      snprintf(out, outLen, "Runtime ring buffer");
      break;
    default:
      snprintf(out, outLen, "--");
      break;
  }
}

void UtilityManager::load() {
  if (!_storageAvailable) {
    return;
  }

  _noteCount = _prefs.getUChar("noteCount", 0);
  if (_noteCount > MaxNotes) {
    _noteCount = MaxNotes;
  }
  _taskCount = _prefs.getUChar("taskCount", 0);
  if (_taskCount > MaxTasks) {
    _taskCount = MaxTasks;
  }
  _nextNoteId = _prefs.getUChar("nextNote", 1);
  _nextTaskId = _prefs.getUChar("nextTask", 0);

  for (uint8_t i = 0; i < _noteCount; ++i) {
    char key[12];
    snprintf(key, sizeof(key), "nt%ut", i);
    strlcpy(_notes[i].title, _prefs.getString(key, "").c_str(),
            sizeof(_notes[i].title));
    snprintf(key, sizeof(key), "nt%ub", i);
    strlcpy(_notes[i].body, _prefs.getString(key, "").c_str(),
            sizeof(_notes[i].body));
    snprintf(key, sizeof(key), "nt%ua", i);
    _notes[i].updatedAt = _prefs.getUInt(key, 0);
  }

  for (uint8_t i = 0; i < _taskCount; ++i) {
    char key[12];
    snprintf(key, sizeof(key), "tk%ut", i);
    strlcpy(_tasks[i].text, _prefs.getString(key, "").c_str(),
            sizeof(_tasks[i].text));
    snprintf(key, sizeof(key), "tk%ud", i);
    _tasks[i].done = _prefs.getBool(key, false);
  }
}

void UtilityManager::saveNotes() {
  if (!_storageAvailable) {
    return;
  }
  _prefs.putUChar("noteCount", _noteCount);
  _prefs.putUChar("nextNote", _nextNoteId);
  for (uint8_t i = 0; i < _noteCount; ++i) {
    char key[12];
    snprintf(key, sizeof(key), "nt%ut", i);
    _prefs.putString(key, _notes[i].title);
    snprintf(key, sizeof(key), "nt%ub", i);
    _prefs.putString(key, _notes[i].body);
    snprintf(key, sizeof(key), "nt%ua", i);
    _prefs.putUInt(key, _notes[i].updatedAt);
  }
}

void UtilityManager::saveTasks() {
  if (!_storageAvailable) {
    return;
  }
  _prefs.putUChar("taskCount", _taskCount);
  _prefs.putUChar("nextTask", _nextTaskId);
  for (uint8_t i = 0; i < _taskCount; ++i) {
    char key[12];
    snprintf(key, sizeof(key), "tk%ut", i);
    _prefs.putString(key, _tasks[i].text);
    snprintf(key, sizeof(key), "tk%ud", i);
    _prefs.putBool(key, _tasks[i].done);
  }
}

void UtilityManager::log(const char* text) {
  if (_logger) {
    _logger->logf(LogLevel::Info, "UTIL: %s", text ? text : "");
  }
}

}  // namespace tabos
