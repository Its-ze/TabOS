#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "logger.h"

namespace tabos {

enum class MotionOrientation : uint8_t {
  Unknown,
  Landscape,
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
  uint8_t displayRotation = 1;
  MotionOrientation orientation = MotionOrientation::Unknown;
};

class MotionManager {
 public:
  void begin(Logger& logger);
  void loop();

  bool available() const { return _snapshot.available; }
  const MotionSnapshot& snapshot() const { return _snapshot; }
  bool consumeRotationChange(uint8_t& rotation);

  static const char* orientationName(MotionOrientation orientation);

 private:
  uint8_t targetRotationFromAccel(float ax, float ay,
                                  MotionOrientation& orientation) const;
  const char* imuName() const;

  MotionSnapshot _snapshot;
  Logger* _logger = nullptr;
  uint32_t _lastSampleMs = 0;
  uint32_t _lastRotationMs = 0;
  uint8_t _candidateRotation = 1;
  uint8_t _candidateCount = 0;
  bool _rotationPending = false;
};

}  // namespace tabos
