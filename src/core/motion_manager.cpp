#include "motion_manager.h"

#include <M5Unified.h>
#include <math.h>

namespace tabos {

namespace {
constexpr uint32_t SampleIntervalMs = 100;
constexpr uint32_t MinRotationIntervalMs = 1800;
constexpr float TiltThresholdG = 0.52f;
constexpr float AxisMarginG = 0.08f;
constexpr uint8_t StableSamplesRequired = 5;
}

void MotionManager::begin(Logger& logger) {
  _logger = &logger;
  if (!M5.Imu.isEnabled() && M5.In_I2C.isEnabled()) {
    M5.Imu.begin(&M5.In_I2C, M5.getBoard());
  }
  _snapshot.available = M5.Imu.isEnabled();
  if (_snapshot.available) {
    _snapshot.displayRotation = 1;
    _candidateRotation = 1;
    _logger->logf(LogLevel::Info, "Motion IMU ready: %s", imuName());
  } else {
    _logger->warn("Motion IMU unavailable");
  }
}

void MotionManager::loop() {
  if (!_snapshot.available) {
    return;
  }

  const uint32_t now = millis();
  if (now - _lastSampleMs < SampleIntervalMs) {
    return;
  }
  _lastSampleMs = now;

  auto updated = M5.Imu.update();
  if (!updated) {
    return;
  }

  const auto data = M5.Imu.getImuData();
  _snapshot.updatedAt = now;
  _snapshot.accelX = data.accel.x;
  _snapshot.accelY = data.accel.y;
  _snapshot.accelZ = data.accel.z;
  _snapshot.gyroX = data.gyro.x;
  _snapshot.gyroY = data.gyro.y;
  _snapshot.gyroZ = data.gyro.z;

  MotionOrientation orientation = _snapshot.orientation;
  const uint8_t target = targetRotationFromAccel(data.accel.x, data.accel.y,
                                                 orientation);
  if (orientation == MotionOrientation::Unknown) {
    _candidateCount = 0;
    _snapshot.orientation = orientation;
    return;
  }
  _snapshot.orientation = orientation;

  if (target != _candidateRotation) {
    _candidateRotation = target;
    _candidateCount = 1;
    return;
  }

  if (_candidateCount < StableSamplesRequired) {
    ++_candidateCount;
    return;
  }

  if (target != _snapshot.displayRotation &&
      now - _lastRotationMs >= MinRotationIntervalMs) {
    _snapshot.displayRotation = target;
    _rotationPending = true;
    _lastRotationMs = now;
    if (_logger) {
      _logger->logf(LogLevel::Info, "Motion rotation target %u (%s)", target,
                    orientationName(_snapshot.orientation));
    }
  }
}

bool MotionManager::consumeRotationChange(uint8_t& rotation) {
  if (!_rotationPending) {
    return false;
  }
  rotation = _snapshot.displayRotation;
  _rotationPending = false;
  return true;
}

uint8_t MotionManager::targetRotationFromAccel(float ax, float ay,
                                               MotionOrientation& orientation) const {
  const float absX = fabsf(ax);
  const float absY = fabsf(ay);
  if (absX < TiltThresholdG && absY < TiltThresholdG) {
    orientation = MotionOrientation::Unknown;
    return _snapshot.displayRotation;
  }

  if (absX >= absY + AxisMarginG) {
    if (ax >= 0.0f) {
      orientation = MotionOrientation::Landscape;
      return 1;
    }
    orientation = MotionOrientation::LandscapeFlip;
    return 3;
  }

  if (absY >= absX + AxisMarginG) {
    if (ay >= 0.0f) {
      orientation = MotionOrientation::Portrait;
      return 0;
    }
    orientation = MotionOrientation::PortraitFlip;
    return 2;
  }

  orientation = MotionOrientation::Unknown;
  return _snapshot.displayRotation;
}

const char* MotionManager::orientationName(MotionOrientation orientation) {
  switch (orientation) {
    case MotionOrientation::Portrait:
      return "portrait";
    case MotionOrientation::Landscape:
      return "landscape";
    case MotionOrientation::PortraitFlip:
      return "portrait flip";
    case MotionOrientation::LandscapeFlip:
      return "landscape flip";
    case MotionOrientation::Unknown:
    default:
      return "unknown";
  }
}

const char* MotionManager::imuName() const {
  switch (M5.Imu.getType()) {
    case m5::imu_none:
      return "none";
    case m5::imu_sh200q:
      return "sh200q";
    case m5::imu_mpu6050:
      return "mpu6050";
    case m5::imu_mpu6886:
      return "mpu6886";
    case m5::imu_mpu9250:
      return "mpu9250";
    case m5::imu_bmi270:
      return "bmi270";
    case m5::imu_unknown:
    default:
      return "unknown";
  }
}

}  // namespace tabos
