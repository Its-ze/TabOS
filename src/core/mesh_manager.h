#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "logger.h"

namespace tabos {

constexpr uint8_t MaxMeshNodes = 12;
constexpr uint8_t MaxMeshMessages = 12;
constexpr uint8_t MaxMeshPackets = 18;

enum class MeshConnectionState : uint8_t {
  Disconnected,
  Connecting,
  Connected,
  Error
};

struct MeshNode {
  uint32_t nodeId = 0;
  char longName[32] = {0};
  char shortName[8] = {0};
  uint32_t firstSeen = 0;
  uint32_t lastSeen = 0;
  int8_t battery = -1;
  int16_t rssi = 0;
  float snr = 0.0f;
  uint8_t hopCount = 0;
  bool favorite = false;
  bool alertEnabled = false;
  bool newNode = false;
};

struct MeshMessage {
  uint32_t at = 0;
  uint32_t from = 0;
  uint32_t to = 0;
  char text[80] = {0};
  bool outgoing = false;
  bool delivered = false;
};

struct MeshPacket {
  uint32_t at = 0;
  uint32_t from = 0;
  char type[18] = {0};
  int16_t rssi = 0;
  float snr = 0.0f;
  char summary[72] = {0};
};

class MeshManager {
 public:
  void begin(Logger& logger);
  void loop();

  void connectSerialText();
  void disconnect();
  bool connected() const { return _state == MeshConnectionState::Connected; }
  MeshConnectionState state() const { return _state; }
  const char* stateText() const;
  const char* connectionText() const { return _connectionText; }
  const char* channelText() const { return _channelText; }

  uint8_t nodeCount() const { return _nodeCount; }
  const MeshNode& node(uint8_t index) const;
  const MeshNode* findNode(uint32_t nodeId) const;
  void clearNewFlags();

  uint8_t messageCount() const { return _messageCount; }
  const MeshMessage& message(uint8_t index) const;
  void sendBroadcast(const char* text);

  uint8_t packetCount() const { return _packetCount; }
  const MeshPacket& packet(uint8_t index) const;
  void clearPacketLog();

  void injectDemoTraffic();
  bool handleTextLine(const char* line);

 private:
  MeshNode& upsertNode(uint32_t nodeId, const char* longName,
                       const char* shortName);
  void addMessage(uint32_t from, uint32_t to, const char* text, bool outgoing,
                  bool delivered);
  void addPacket(uint32_t from, const char* type, int16_t rssi, float snr,
                 const char* summary);
  void logNewNode(const MeshNode& node);

  Logger* _logger = nullptr;
  MeshConnectionState _state = MeshConnectionState::Disconnected;
  char _connectionText[24] = "None";
  char _channelText[24] = "--";

  MeshNode _nodes[MaxMeshNodes];
  uint8_t _nodeCount = 0;
  MeshMessage _messages[MaxMeshMessages];
  uint8_t _messageCount = 0;
  MeshPacket _packets[MaxMeshPackets];
  uint8_t _packetCount = 0;

  uint32_t _lastHeartbeatAt = 0;
  uint8_t _demoStep = 0;
};

}  // namespace tabos
