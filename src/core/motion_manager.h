#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "logger.h"

namespace tabos {

enum class MotionOrientation : uint8_t {
  Unknown,
  Portrait,
  Landscape,
  PortraitFlip,
  LandscapeFlip
};

struct MotionSnapshot {
  bool available = false;
  uint32_t updatedAt = 0;
  float accelX = 0.0f;
  float accelY = 0.0f;
  float accelZ = 0.0f;
  float gyroX = 0.0f;
  float gyroY = 0.0f;
  float gyroZ = 0.0f;
  float flatYawDeg = 0.0f;
  uint8_t displayRotation = 1;
  MotionOrientation orientation = MotionOrientation::Unknown;
};

class MotionManager {
 public:
  void begin(Logger& logger);
  void loop();

  bool available() const { return _snapshot.available; }
  const MotionSnapshot& snapshot() const { return _snapshot; }
  uint8_t candidateRotation() const { return _candidateRotation; }
  uint8_t stableSampleCount() const { return _candidateCount; }
  bool rotationPending() const { return _rotationPending; }
  bool consumeRotationChange(uint8_t& rotation);

  static const char* orientationName(MotionOrientation orientation);

  const char* imuName() const;

 private:
  uint8_t targetRotationFromAccel(float ax, float ay,
                                  MotionOrientation& orientation) const;
  bool targetRotationFromFlatGyro(float az, float gz, float dtSeconds,
                                  uint8_t& rotation,
                                  MotionOrientation& orientation);
  MotionOrientation orientationForRotation(uint8_t rotation) const;
  uint8_t oppositeRotation(uint8_t rotation) const;

  MotionSnapshot _snapshot;
  Logger* _logger = nullptr;
  uint32_t _lastSampleMs = 0;
  uint32_t _lastRotationMs = 0;
  uint8_t _candidateRotation = 1;
  uint8_t _candidateCount = 0;
  bool _rotationPending = false;
};

}  // namespace tabos
