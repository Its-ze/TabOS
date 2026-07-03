#include "mesh_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace tabos {

namespace {
MeshNode gEmptyNode;
MeshMessage gEmptyMessage;
MeshPacket gEmptyPacket;

void shortHex(uint32_t value, char* out, size_t outLen) {
  snprintf(out, outLen, "%04lX", static_cast<unsigned long>(value & 0xFFFF));
}
}  // namespace

void MeshManager::begin(Logger& logger) {
  _logger = &logger;
  _logger->info("Mesh manager started");
}

void MeshManager::loop() {
  if (!connected()) {
    return;
  }

  const uint32_t now = millis();
  if (now - _lastHeartbeatAt > 30000) {
    _lastHeartbeatAt = now;
    addPacket(0x00000001, "heartbeat", -42, 8.5f, "Mesh adapter heartbeat");
  }
}

void MeshManager::connectSerialText() {
  if (connected()) {
    return;
  }

  _state = MeshConnectionState::Connected;
  strlcpy(_connectionText, "Serial text", sizeof(_connectionText));
  strlcpy(_channelText, "Primary", sizeof(_channelText));
  _lastHeartbeatAt = millis();

  upsertNode(0x00000001, "TabOS Local", "TAB");
  addPacket(0x00000001, "connect", -35, 10.0f, "Serial text adapter connected");
  if (_logger) {
    _logger->info("Mesh serial text adapter connected");
  }
}

void MeshManager::disconnect() {
  if (_state == MeshConnectionState::Disconnected) {
    return;
  }
  _state = MeshConnectionState::Disconnected;
  strlcpy(_connectionText, "None", sizeof(_connectionText));
  strlcpy(_channelText, "--", sizeof(_channelText));
  if (_logger) {
    _logger->info("Mesh disconnected");
  }
}

const char* MeshManager::stateText() const {
  switch (_state) {
    case MeshConnectionState::Disconnected:
      return "Disconnected";
    case MeshConnectionState::Connecting:
      return "Connecting";
    case MeshConnectionState::Connected:
      return "Connected";
    case MeshConnectionState::Error:
      return "Error";
  }
  return "Unknown";
}

const MeshNode& MeshManager::node(uint8_t index) const {
  if (index >= _nodeCount) {
    return gEmptyNode;
  }
  return _nodes[index];
}

const MeshNode* MeshManager::findNode(uint32_t nodeId) const {
  for (uint8_t i = 0; i < _nodeCount; ++i) {
    if (_nodes[i].nodeId == nodeId) {
      return &_nodes[i];
    }
  }
  return nullptr;
}

void MeshManager::clearNewFlags() {
  for (uint8_t i = 0; i < _nodeCount; ++i) {
    _nodes[i].newNode = false;
  }
}

const MeshMessage& MeshManager::message(uint8_t index) const {
  if (index >= _messageCount) {
    return gEmptyMessage;
  }
  return _messages[index];
}

void MeshManager::sendBroadcast(const char* text) {
  if (!connected()) {
    if (_logger) {
      _logger->warn("Mesh message skipped; not connected");
    }
    return;
  }

  addMessage(0x00000001, 0xFFFFFFFF, text, true, true);
  addPacket(0x00000001, "text", -32, 9.2f, text);
  if (_logger) {
    _logger->logf(LogLevel::Info, "Mesh broadcast: %s", text);
  }
}

const MeshPacket& MeshManager::packet(uint8_t index) const {
  if (index >= _packetCount) {
    return gEmptyPacket;
  }
  return _packets[index];
}

void MeshManager::clearPacketLog() {
  memset(_packets, 0, sizeof(_packets));
  _packetCount = 0;
  if (_logger) {
    _logger->info("Mesh packet log cleared");
  }
}

void MeshManager::injectDemoTraffic() {
  connectSerialText();

  if (_demoStep == 0) {
    MeshNode& node = upsertNode(0x00A10001, "Base Alpha", "BA");
    node.battery = 91;
    node.rssi = -48;
    node.snr = 7.5f;
    node.hopCount = 1;
    addPacket(node.nodeId, "nodeinfo", node.rssi, node.snr, "Base Alpha joined");
    addMessage(node.nodeId, 0xFFFFFFFF, "Base Alpha online", false, true);
  } else if (_demoStep == 1) {
    MeshNode& node = upsertNode(0x00B20002, "Field Kit", "FK");
    node.battery = 73;
    node.rssi = -64;
    node.snr = 4.2f;
    node.hopCount = 2;
    addPacket(node.nodeId, "position", node.rssi, node.snr, "Field Kit position update");
  } else {
    MeshNode& node = upsertNode(0x00C30003, "Relay Roof", "RR");
    node.battery = 100;
    node.rssi = -59;
    node.snr = 6.8f;
    node.hopCount = 1;
    addPacket(node.nodeId, "telemetry", node.rssi, node.snr, "Relay battery full");
    addMessage(node.nodeId, 0xFFFFFFFF, "Relay Roof heard TabOS", false, true);
  }

  _demoStep = (_demoStep + 1) % 3;
}

bool MeshManager::handleTextLine(const char* line) {
  if (line == nullptr || strncmp(line, "mesh ", 5) != 0) {
    return false;
  }

  const char* cmd = line + 5;
  if (strcmp(cmd, "connect") == 0) {
    connectSerialText();
    return true;
  }

  if (strcmp(cmd, "disconnect") == 0) {
    disconnect();
    return true;
  }

  if (strcmp(cmd, "demo") == 0) {
    injectDemoTraffic();
    return true;
  }

  if (strncmp(cmd, "msg ", 4) == 0) {
    connectSerialText();
    sendBroadcast(cmd + 4);
    return true;
  }

  if (strncmp(cmd, "node ", 5) == 0) {
    connectSerialText();
    char name[32] = {0};
    uint32_t id = strtoul(cmd + 5, nullptr, 16);
    const char* nameStart = strchr(cmd + 5, ' ');
    if (nameStart != nullptr) {
      strlcpy(name, nameStart + 1, sizeof(name));
    } else {
      strlcpy(name, "Serial Node", sizeof(name));
    }
    char shortName[8];
    shortHex(id, shortName, sizeof(shortName));
    MeshNode& node = upsertNode(id, name, shortName);
    node.rssi = -55;
    node.snr = 5.5f;
    node.battery = 80;
    addPacket(id, "nodeinfo", node.rssi, node.snr, name);
    return true;
  }

  return false;
}

MeshNode& MeshManager::upsertNode(uint32_t nodeId, const char* longName,
                                  const char* shortName) {
  for (uint8_t i = 0; i < _nodeCount; ++i) {
    if (_nodes[i].nodeId == nodeId) {
      _nodes[i].lastSeen = millis() / 1000;
      if (longName && longName[0]) {
        strlcpy(_nodes[i].longName, longName, sizeof(_nodes[i].longName));
      }
      if (shortName && shortName[0]) {
        strlcpy(_nodes[i].shortName, shortName, sizeof(_nodes[i].shortName));
      }
      return _nodes[i];
    }
  }

  if (_nodeCount >= MaxMeshNodes) {
    _nodes[0].lastSeen = millis() / 1000;
    return _nodes[0];
  }

  MeshNode& node = _nodes[_nodeCount++];
  node = MeshNode{};
  node.nodeId = nodeId;
  node.firstSeen = millis() / 1000;
  node.lastSeen = node.firstSeen;
  node.battery = -1;
  node.newNode = true;
  strlcpy(node.longName, longName && longName[0] ? longName : "Unknown Node",
          sizeof(node.longName));
  strlcpy(node.shortName, shortName && shortName[0] ? shortName : "NODE",
          sizeof(node.shortName));
  logNewNode(node);
  return node;
}

void MeshManager::addMessage(uint32_t from, uint32_t to, const char* text,
                             bool outgoing, bool delivered) {
  if (_messageCount >= MaxMeshMessages) {
    for (uint8_t i = 1; i < MaxMeshMessages; ++i) {
      _messages[i - 1] = _messages[i];
    }
    _messageCount = MaxMeshMessages - 1;
  }

  MeshMessage& msg = _messages[_messageCount++];
  msg = MeshMessage{};
  msg.at = millis() / 1000;
  msg.from = from;
  msg.to = to;
  msg.outgoing = outgoing;
  msg.delivered = delivered;
  strlcpy(msg.text, text ? text : "", sizeof(msg.text));
}

void MeshManager::addPacket(uint32_t from, const char* type, int16_t rssi,
                            float snr, const char* summary) {
  if (_packetCount >= MaxMeshPackets) {
    for (uint8_t i = 1; i < MaxMeshPackets; ++i) {
      _packets[i - 1] = _packets[i];
    }
    _packetCount = MaxMeshPackets - 1;
  }

  MeshPacket& packet = _packets[_packetCount++];
  packet = MeshPacket{};
  packet.at = millis() / 1000;
  packet.from = from;
  packet.rssi = rssi;
  packet.snr = snr;
  strlcpy(packet.type, type ? type : "packet", sizeof(packet.type));
  strlcpy(packet.summary, summary ? summary : "", sizeof(packet.summary));
}

void MeshManager::logNewNode(const MeshNode& node) {
  if (_logger) {
    _logger->logf(LogLevel::Info, "Mesh node found: %s", node.longName);
  }
}

}  // namespace tabos
