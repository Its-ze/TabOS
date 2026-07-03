#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "logger.h"

namespace tabos {

constexpr uint8_t MaxHardwareEvents = 10;
constexpr uint8_t MaxGpioPins = 4;

enum class GpioMode : uint8_t {
  Input,
  Output
};

struct HardwareEvent {
  uint32_t at = 0;
  char text[72] = {0};
};

struct GpioPinState {
  uint8_t pin = 0;
  GpioMode mode = GpioMode::Input;
  bool value = false;
};

struct SensorSnapshot {
  float temperatureC = 24.0f;
  float humidity = 41.0f;
  float light = 62.0f;
  float voltage = 5.0f;
  bool motion = false;
  uint32_t updatedAt = 0;
};

class HardwareManager {
 public:
  void begin(Logger& logger);
  void loop();

  bool atomDetected() const { return _atomDetected; }
  const char* atomStatus() const { return _atomStatus; }
  const char* lastAtomCommand() const { return _lastAtomCommand; }
  void detectAtom();
  void sendAtomCommand(const char* command);

  const char* irProfile() const;
  const char* lastIrCommand() const { return _lastIrCommand; }
  void nextIrProfile();
  void sendIrCommand(const char* command);

  bool gpioArmed() const;
  uint8_t gpioArmSecondsLeft() const;
  void armGpio();
  void setGpioMode(uint8_t index, GpioMode mode);
  void toggleGpioValue(uint8_t index);
  const GpioPinState& gpioPin(uint8_t index) const;

  bool relayOn() const { return _relayOn; }
  void setRelay(bool on);

  const char* ledColor() const { return _ledColor; }
  void setLedColor(const char* color);

  const SensorSnapshot& sensors() const { return _sensors; }
  void refreshSensors();

  uint8_t eventCount() const { return _eventCount; }
  const HardwareEvent& event(uint8_t index) const;

 private:
  void addEvent(const char* text);

  Logger* _logger = nullptr;
  bool _atomDetected = false;
  char _atomStatus[24] = "Not detected";
  char _lastAtomCommand[24] = "--";

  uint8_t _irProfile = 0;
  char _lastIrCommand[24] = "--";

  GpioPinState _pins[MaxGpioPins];
  uint32_t _gpioArmedUntil = 0;

  bool _relayOn = false;
  char _ledColor[16] = "Off";
  SensorSnapshot _sensors;
  uint32_t _lastSensorTick = 0;
  uint8_t _sensorStep = 0;

  HardwareEvent _events[MaxHardwareEvents];
  uint8_t _eventCount = 0;
};

}  // namespace tabos
