#include "app_network_tools.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

namespace {
struct PortTarget {
  const char* host;
  uint16_t port;
};

const PortTarget kTargets[] = {
    {"itsolutions.digital", 443},
    {"1.1.1.1", 53},
    {"github.com", 443},
};

const char* const kHosts[] = {
    "itsolutions.digital",
    "github.com",
    "one.one.one.one",
};

void drawHeader(const char* icon, const char* title) {
  HalDisplay& d = display();
  const int16_t top = config::StatusBarHeight;
  d.fillRect(0, top, d.width(), d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText(icon, 36, top + 52, 4, theme::Accent, theme::Background,
             TextAlign::MiddleLeft);
  d.drawText(title, 112, top + 52, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);
}

void drawInfoCard(const char* label, const char* value, const char* result) {
  HalDisplay& d = display();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 38;
  const int16_t y = top + 120;
  const int16_t w = d.width() - 76;
  d.fillRoundRect(x, y, w, 150, 8, theme::Panel);
  d.drawRoundRect(x, y, w, 150, 8, theme::Border);
  d.drawText(label, x + 24, y + 34, 2, theme::Accent, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText(value, x + 24, y + 74, 2, theme::Text, theme::Panel,
             TextAlign::MiddleLeft);
  d.drawText(result, x + 24, y + 116, 2, theme::MutedText, theme::Panel,
             TextAlign::MiddleLeft);
}

bool networkReady(WifiManager& wifi, char* result, size_t resultLen) {
  if (!wifi.enabled()) {
    snprintf(result, resultLen, "Wi-Fi is disabled.");
    return false;
  }
  if (WiFi.status() != WL_CONNECTED) {
    snprintf(result, resultLen, "Wi-Fi is on but not connected.");
    return false;
  }
  return true;
}

void drawActionButton(int16_t x, int16_t y, int16_t w, const char* label) {
  HalDisplay& d = display();
  d.fillRoundRect(x, y, w, 58, 8, theme::PanelAlt);
  d.drawRoundRect(x, y, w, 58, 8, theme::Border);
  d.drawText(label, x + w / 2, y + 29, 2, theme::Text, theme::PanelAlt,
             TextAlign::MiddleCenter);
}

bool hit(const InputEvent& event, int16_t x, int16_t y, int16_t w, int16_t h) {
  return event.type == InputEventType::TouchTap &&
         event.x >= x && event.x < x + w &&
         event.y >= y && event.y < y + h;
}
}  // namespace

PingApp::PingApp(WifiManager& wifi, Logger& logger)
    : _wifi(wifi), _logger(logger) {}

void PingApp::draw() {
  drawHeader(icon(), name());

  char status[72];
  snprintf(status, sizeof(status), "Radio: %s    IP: %s",
           _wifi.statusText(), _wifi.ipAddressText());
  drawInfoCard("Reachability", status, _result);
  drawButton("Check");
}

void PingApp::handleInput(const InputEvent& event) {
  const int16_t y = config::StatusBarHeight + 310;
  if (hit(event, 38, y, 190, 58)) {
    runCheck();
  }
}

void PingApp::drawButton(const char* label) {
  drawActionButton(38, config::StatusBarHeight + 310, 190, label);
}

void PingApp::runCheck() {
  _lastCheckAt = millis();
  if (!networkReady(_wifi, _result, sizeof(_result))) {
    _logger.warn("Ping check skipped; network unavailable");
    return;
  }

  WiFiClient client;
  client.setTimeout(700);
  const bool ok = client.connect(IPAddress(1, 1, 1, 1), 53);
  client.stop();
  snprintf(_result, sizeof(_result), ok ? "1.1.1.1:53 answered." :
                                           "No reply from 1.1.1.1:53.");
  _logger.logf(ok ? LogLevel::Info : LogLevel::Warn, "Ping check %s",
               ok ? "passed" : "failed");
}

PortCheckApp::PortCheckApp(WifiManager& wifi, Logger& logger)
    : _wifi(wifi), _logger(logger) {}

void PortCheckApp::draw() {
  drawHeader(icon(), name());
  const PortTarget& target = kTargets[_target];

  char value[72];
  snprintf(value, sizeof(value), "%s:%u", target.host, target.port);
  drawInfoCard("TCP Endpoint", value, _result);

  const int16_t y = config::StatusBarHeight + 310;
  drawActionButton(38, y, 190, "Check");
  drawActionButton(248, y, 190, "Next");
}

void PortCheckApp::handleInput(const InputEvent& event) {
  const int16_t y = config::StatusBarHeight + 310;
  if (hit(event, 38, y, 190, 58)) {
    runCheck();
  } else if (hit(event, 248, y, 190, 58)) {
    nextTarget();
  }
}

void PortCheckApp::runCheck() {
  if (!networkReady(_wifi, _result, sizeof(_result))) {
    _logger.warn("Port check skipped; network unavailable");
    return;
  }

  const PortTarget& target = kTargets[_target];
  WiFiClient client;
  client.setTimeout(900);
  const bool ok = client.connect(target.host, target.port);
  client.stop();
  snprintf(_result, sizeof(_result), ok ? "Open: %s:%u" : "Closed/no reply: %s:%u",
           target.host, target.port);
  _logger.logf(ok ? LogLevel::Info : LogLevel::Warn, "Port check %s %s:%u",
               ok ? "open" : "closed", target.host, target.port);
}

void PortCheckApp::nextTarget() {
  _target = (_target + 1) % (sizeof(kTargets) / sizeof(kTargets[0]));
  snprintf(_result, sizeof(_result), "Target changed.");
}

DnsApp::DnsApp(WifiManager& wifi, Logger& logger)
    : _wifi(wifi), _logger(logger) {}

void DnsApp::draw() {
  drawHeader(icon(), name());
  drawInfoCard("Host", kHosts[_host], _result);

  const int16_t y = config::StatusBarHeight + 310;
  drawActionButton(38, y, 190, "Resolve");
  drawActionButton(248, y, 190, "Next");
}

void DnsApp::handleInput(const InputEvent& event) {
  const int16_t y = config::StatusBarHeight + 310;
  if (hit(event, 38, y, 190, 58)) {
    resolveHost();
  } else if (hit(event, 248, y, 190, 58)) {
    nextHost();
  }
}

void DnsApp::resolveHost() {
  if (!networkReady(_wifi, _result, sizeof(_result))) {
    _logger.warn("DNS lookup skipped; network unavailable");
    return;
  }

  IPAddress ip;
  if (WiFi.hostByName(kHosts[_host], ip) == 1) {
    snprintf(_result, sizeof(_result), "%s -> %u.%u.%u.%u", kHosts[_host],
             ip[0], ip[1], ip[2], ip[3]);
    _logger.logf(LogLevel::Info, "DNS resolved %s", kHosts[_host]);
  } else {
    snprintf(_result, sizeof(_result), "DNS lookup failed.");
    _logger.warn("DNS lookup failed");
  }
}

void DnsApp::nextHost() {
  _host = (_host + 1) % (sizeof(kHosts) / sizeof(kHosts[0]));
  snprintf(_result, sizeof(_result), "Host changed.");
}

}  // namespace tabos
