#include "system.h"

#include <Arduino.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../hal/hal_touch.h"
#include "../ui/theme.h"

namespace tabos {

System& System::instance() {
  static System system;
  return system;
}

System::System()
    : _meshApp(_mesh),
      _messagesApp(_mesh),
      _nodesApp(_mesh),
      _terminalApp(_terminal),
      _settingsApp(_settings, _logger),
      _batteryApp(_battery),
      _wifiApp(_settings, _wifi, _logger),
      _bluetoothApp(_bluetooth),
      _pingApp(_wifi, _logger),
      _portCheckApp(_wifi, _logger),
      _dnsApp(_wifi, _logger),
      _qrApp(),
      _atomApp(_hardware),
      _irApp(_hardware),
      _gpioApp(_hardware),
      _relayApp(_hardware),
      _ledApp(_hardware),
      _sensorsApp(_hardware, _motion),
      _logsApp(_logger),
      _storageApp(_utilities),
      _powerApp(_settings, _power, _battery, _logger),
      _packetLogApp(_mesh),
      _developerApp(_apps, _logger),
      _notesApp(_utilities),
      _tasksApp(_utilities),
      _timerApp(_utilities),
      _stopwatchApp(_utilities),
      _flashlightApp(_utilities, _settings),
      _macroApp(_utilities, _wifi, _bluetooth, _mesh, _hardware) {}

void System::begin() {
  display().begin(_logger);
  _logger.begin();
  _logger.info("Boot started");
  showSplash();

  touch().begin(_logger);
  _settings.begin(_logger);
  display().setBrightness(_settings.data().brightness);

  _battery.begin(_logger);
  _motion.begin(_logger);
  _wifi.begin(_settings, _logger);
  _bluetooth.begin(_settings, _logger);
  _mesh.begin(_logger);
  _hardware.begin(_logger);
  _utilities.begin(_logger);
  _terminal.begin(_logger);
  _terminal.attachMesh(_mesh);
  _terminal.attachMotion(_motion);
  _power.begin(_settings, _logger);
  _apps.begin(_logger);
  registerApps();

  _ui.begin(_apps, _settings, _battery, _wifi, _bluetooth, _mesh, _logger);
  _apps.home();
  _ui.requestRedraw();
  _logger.info("Boot complete");
}

void System::loop() {
  display().update();

  InputEvent event;
  if (touch().poll(event)) {
    const bool woke = _power.handleInput(event);
    if (woke) {
      _ui.requestRedraw();
      return;
    }

    if (event.type != InputEventType::TouchActivity) {
      _ui.handleInput(event);
    }
  }

  _battery.loop();
  _wifi.loop();
  _bluetooth.loop();
  _mesh.loop();
  _motion.loop();
  _hardware.loop();
  _utilities.loop();
  _terminal.loop();

  AppBase* active = _apps.activeApp();
  if (active) {
    active->loop();
  }

  const bool keepAwake = active && active->wantsKeepAwake();
  _power.loop(keepAwake, _battery.isExternalPowered());

  if (!_power.isScreenOff()) {
    uint8_t rotation = display().rotation();
    if (_settings.data().autoRotate &&
        _motion.consumeRotationChange(rotation) &&
        rotation != display().rotation()) {
      display().setRotation(rotation);
      _ui.requestRedraw();
    }
    _ui.loop();
  }

  delay(5);
}

void System::showSplash() {
  HalDisplay& d = display();
  d.fillScreen(theme::Background);
  d.drawText(TABOS_NAME, d.width() / 2, d.height() / 2 - 44, 4, theme::Accent,
             theme::Background, TextAlign::MiddleCenter);
  d.drawText("Touch field OS for M5Stack Tab5", d.width() / 2,
             d.height() / 2 + 12, 2, theme::Text, theme::Background,
             TextAlign::MiddleCenter);
  d.drawText(TABOS_VERSION, d.width() / 2, d.height() / 2 + 56, 2,
             theme::MutedText, theme::Background, TextAlign::MiddleCenter);
  delay(850);
}

void System::registerApps() {
  _apps.registerApp(_meshApp, 0);
  _apps.registerApp(_messagesApp, 0);
  _apps.registerApp(_nodesApp, 0);
  _apps.registerApp(_terminalApp, 0);
  _apps.registerApp(_settingsApp, 0);
  _apps.registerApp(_batteryApp, 0);

  _apps.registerApp(_wifiApp, 1);
  _apps.registerApp(_bluetoothApp, 1);
  _apps.registerApp(_pingApp, 1);
  _apps.registerApp(_portCheckApp, 1);
  _apps.registerApp(_dnsApp, 1);
  _apps.registerApp(_qrApp, 1);

  _apps.registerApp(_atomApp, 2);
  _apps.registerApp(_irApp, 2);
  _apps.registerApp(_gpioApp, 2);
  _apps.registerApp(_relayApp, 2);
  _apps.registerApp(_ledApp, 2);
  _apps.registerApp(_sensorsApp, 2);

  _apps.registerApp(_logsApp, 3);
  _apps.registerApp(_storageApp, 3);
  _apps.registerApp(_aboutApp, 3);
  _apps.registerApp(_powerApp, 3);
  _apps.registerApp(_packetLogApp, 3);
  _apps.registerApp(_developerApp, 3);

  _apps.registerApp(_notesApp, 4);
  _apps.registerApp(_tasksApp, 4);
  _apps.registerApp(_timerApp, 4);
  _apps.registerApp(_stopwatchApp, 4);
  _apps.registerApp(_flashlightApp, 4);
  _apps.registerApp(_macroApp, 4);
}

}  // namespace tabos
