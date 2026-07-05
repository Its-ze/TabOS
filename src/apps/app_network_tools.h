#pragma once

#include "../core/logger.h"
#include "../core/wifi_manager.h"
#include "app_base.h"

namespace tabos {

class PingApp : public AppBase {
 public:
  PingApp(WifiManager& wifi, Logger& logger);

  const char* id() const override { return "ping"; }
  const char* name() const override { return "Ping"; }
  const char* icon() const override { return "PG"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  void runCheck();
  void drawButton(const char* label);

  WifiManager& _wifi;
  Logger& _logger;
  char _result[80] = "Tap Check to test basic internet reachability.";
  uint32_t _lastCheckAt = 0;
};

class PortCheckApp : public AppBase {
 public:
  PortCheckApp(WifiManager& wifi, Logger& logger);

  const char* id() const override { return "port"; }
  const char* name() const override { return "Port Check"; }
  const char* icon() const override { return "PC"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  void runCheck();
  void nextTarget();

  WifiManager& _wifi;
  Logger& _logger;
  uint8_t _target = 0;
  char _result[80] = "Tap Check to probe the selected TCP endpoint.";
};

class DnsApp : public AppBase {
 public:
  DnsApp(WifiManager& wifi, Logger& logger);

  const char* id() const override { return "dns"; }
  const char* name() const override { return "DNS"; }
  const char* icon() const override { return "DN"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  void resolveHost();
  void nextHost();

  WifiManager& _wifi;
  Logger& _logger;
  uint8_t _host = 0;
  char _result[80] = "Tap Resolve to run a DNS lookup.";
};

}  // namespace tabos
