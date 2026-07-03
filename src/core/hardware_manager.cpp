#include "hardware_manager.h"

#include <stdio.h>
#include <string.h>

namespace tabos {

namespace {
HardwareEvent gEmptyEvent;
GpioPinState gEmptyPin;
const char* const kIrProfiles[] = {"TV", "Projector", "LED Strip", "Soundbar", "Custom"};
constexpr uint8_t kIrProfileCount = sizeof(kIrProfiles) / sizeof(kIrProfiles[0]);
constexpr uint32_t GpioArmMs = 15000;
}  // namespace

void HardwareManager::begin(Logger& logger) {
  _logger = &logger;
  for (uint8_t i = 0; i < MaxGpioPins; ++i) {
    _pins[i].pin = i;
    _pins[i].mode = GpioMode::Input;
    _pins[i].value = false;
  }
  refreshSensors();
  addEvent("Hardware manager ready");
}

void HardwareManager::loop() {
  if (millis() - _lastSensorTick > 5000) {
    refreshSensors();
  }
}

void HardwareManager::detectAtom() {
  _atomDetected = true;
  strlcpy(_atomStatus, "Virtual Atom", sizeof(_atomStatus));
  addEvent("Atom unit detected in safe stub mode");
}

void HardwareManager::sendAtomCommand(const char* command) {
  if (!_atomDetected) {
    detectAtom();
  }
  strlcpy(_lastAtomCommand, command ? command : "--", sizeof(_lastAtomCommand));
  char event[80];
  snprintf(event, sizeof(event), "Atom command queued: %s", _lastAtomCommand);
  addEvent(event);
}

const char* HardwareManager::irProfile() const {
  return kIrProfiles[_irProfile % kIrProfileCount];
}

void HardwareManager::nextIrProfile() {
  _irProfile = (_irProfile + 1) % kIrProfileCount;
  char event[72];
  snprintf(event, sizeof(event), "IR profile selected: %s", irProfile());
  addEvent(event);
}

void HardwareManager::sendIrCommand(const char* command) {
  strlcpy(_lastIrCommand, command ? command : "--", sizeof(_lastIrCommand));
  char event[80];
  snprintf(event, sizeof(event), "IR command stubbed: %s/%s", irProfile(),
           _lastIrCommand);
  addEvent(event);
}

bool HardwareManager::gpioArmed() const {
  return millis() < _gpioArmedUntil;
}

uint8_t HardwareManager::gpioArmSecondsLeft() const {
  if (!gpioArmed()) {
    return 0;
  }
  return static_cast<uint8_t>((_gpioArmedUntil - millis() + 999) / 1000);
}

void HardwareManager::armGpio() {
  _gpioArmedUntil = millis() + GpioArmMs;
  addEvent("GPIO changes armed for 15 seconds");
}

void HardwareManager::setGpioMode(uint8_t index, GpioMode mode) {
  if (index >= MaxGpioPins) {
    return;
  }
  if (!gpioArmed()) {
    addEvent("GPIO mode change blocked; not armed");
    return;
  }
  _pins[index].mode = mode;
  char event[64];
  snprintf(event, sizeof(event), "GPIO %u mode set to %s", _pins[index].pin,
           mode == GpioMode::Output ? "output" : "input");
  addEvent(event);
}

void HardwareManager::toggleGpioValue(uint8_t index) {
  if (index >= MaxGpioPins) {
    return;
  }
  if (!gpioArmed()) {
    addEvent("GPIO write blocked; not armed");
    return;
  }
  _pins[index].mode = GpioMode::Output;
  _pins[index].value = !_pins[index].value;
  char event[64];
  snprintf(event, sizeof(event), "GPIO %u value set to %s", _pins[index].pin,
           _pins[index].value ? "high" : "low");
  addEvent(event);
}

const GpioPinState& HardwareManager::gpioPin(uint8_t index) const {
  if (index >= MaxGpioPins) {
    return gEmptyPin;
  }
  return _pins[index];
}

void HardwareManager::setRelay(bool on) {
  _relayOn = on;
  addEvent(on ? "Relay set ON in safe stub mode" : "Relay set OFF in safe stub mode");
}

void HardwareManager::setLedColor(const char* color) {
  strlcpy(_ledColor, color ? color : "Off", sizeof(_ledColor));
  char event[64];
  snprintf(event, sizeof(event), "LED color set to %s", _ledColor);
  addEvent(event);
}

void HardwareManager::refreshSensors() {
  _lastSensorTick = millis();
  ++_sensorStep;
  _sensors.temperatureC = 23.5f + (_sensorStep % 6) * 0.4f;
  _sensors.humidity = 39.0f + (_sensorStep % 8) * 1.3f;
  _sensors.light = 55.0f + (_sensorStep % 5) * 7.5f;
  _sensors.voltage = 4.8f + (_sensorStep % 4) * 0.05f;
  _sensors.motion = (_sensorStep % 3) == 0;
  _sensors.updatedAt = millis() / 1000;
}

const HardwareEvent& HardwareManager::event(uint8_t index) const {
  if (index >= _eventCount) {
    return gEmptyEvent;
  }
  return _events[index];
}

void HardwareManager::addEvent(const char* text) {
  if (_eventCount >= MaxHardwareEvents) {
    for (uint8_t i = 1; i < MaxHardwareEvents; ++i) {
      _events[i - 1] = _events[i];
    }
    _eventCount = MaxHardwareEvents - 1;
  }

  HardwareEvent& event = _events[_eventCount++];
  event.at = millis() / 1000;
  strlcpy(event.text, text ? text : "", sizeof(event.text));

  if (_logger) {
    _logger->logf(LogLevel::Info, "HW: %s", event.text);
  }
}

}  // namespace tabos
