# TabOS for M5Stack Tab5

Milestone 5 firmware scaffold for a MeshCore-style, touch-first handheld field OS on the M5Stack Tab5.

## What is included

- Splash screen with project name and firmware version.
- Permanent top status bar with Wi-Fi, Bluetooth, mesh, battery, and uptime indicators.
- Five swipe launcher pages with six large app tiles each.
- Bottom navigation bar with Back, Home, Apps, and Settings.
- Basic app lifecycle and app manager.
- Persistent settings using ESP32 Preferences/NVS.
- Ring-buffer logger with on-device Logs app.
- Battery monitor using M5Unified power APIs.
- Auto dim, auto screen off, wake-on-touch, and stay-awake-while-externally-powered behavior.
- HAL wrappers for display, touch, and battery access.
- Wi-Fi manager with radio toggle and async scan results.
- Bluetooth manager with BLE scan support when the board package/runtime supports BLE.
- Terminal manager with USB serial command input and on-device terminal view.
- Swipe-down quick settings shade with large toggles.
- Mesh manager with connection state, node database, message list, and packet log.
- Mesh, Nodes, Messages, and Packet Log apps.
- Hardware manager with safe stub state for Atom, IR, GPIO, Relay, LED, and Sensors.
- Atom Control app with detect/status/LED/relay command buttons.
- IR Remote app with profiles and canned command buttons.
- GPIO app with explicit timed arming before simulated mode/value changes.
- Relay app with safe ON/OFF state.
- LED Control app with color selection.
- Sensors app with simulated environmental readings.
- Utility manager with saved notes, saved tasks, timer, stopwatch, flashlight state, macro history, and virtual storage index.
- Notes app with persistent local note creation and delete-last action.
- Tasks app with persistent local tasks, completion toggles, and clear-done action.
- Timer app with 1 minute, 5 minute, stop, and reset controls.
- Stopwatch app with start, stop, and reset controls.
- Flashlight app that uses the Tab5 screen as white/red light and cycles brightness.
- Macro Pad app with quick actions for Wi-Fi, Bluetooth, mesh demo traffic, relay, LED blue, and mesh ping.
- Storage app showing a safe NVS-backed virtual file index for settings, notes, tasks, and logs.

The current hardware-control layer is intentionally safe: it records actions and updates app state, but it does not write real GPIO pins, trigger physical relays, emit IR, or send I2C/Grove commands yet. Board-safe pin profiles, unit detection, actual HAL-backed hardware output, and raw filesystem export/delete are deferred.

## File tree

```text
TabOS/
  README.md
  TabOS.ino
  src/
    config.h
    main.cpp
    main.h
    apps/
      app_about.cpp
      app_about.h
      app_atom_control.cpp
      app_atom_control.h
      app_base.h
      app_battery.cpp
      app_battery.h
      app_bluetooth.cpp
      app_bluetooth.h
      app_flashlight.cpp
      app_flashlight.h
      app_gpio.cpp
      app_gpio.h
      app_ir_remote.cpp
      app_ir_remote.h
      app_led_control.cpp
      app_led_control.h
      app_logs.cpp
      app_logs.h
      app_macro.cpp
      app_macro.h
      app_mesh.cpp
      app_mesh.h
      app_messages.cpp
      app_messages.h
      app_nodes.cpp
      app_nodes.h
      app_notes.cpp
      app_notes.h
      app_packet_log.cpp
      app_packet_log.h
      app_relay.cpp
      app_relay.h
      app_sensors.cpp
      app_sensors.h
      app_settings.cpp
      app_settings.h
      app_stopwatch.cpp
      app_stopwatch.h
      app_storage.cpp
      app_storage.h
      app_tasks.cpp
      app_tasks.h
      app_terminal.cpp
      app_terminal.h
      app_timer.cpp
      app_timer.h
      app_wifi.cpp
      app_wifi.h
      placeholder_app.cpp
      placeholder_app.h
    core/
      app_manager.cpp
      app_manager.h
      battery.cpp
      battery.h
      bluetooth_manager.cpp
      bluetooth_manager.h
      hardware_manager.cpp
      hardware_manager.h
      input_event.h
      logger.cpp
      logger.h
      mesh_manager.cpp
      mesh_manager.h
      power_manager.cpp
      power_manager.h
      settings.cpp
      settings.h
      system.cpp
      system.h
      terminal_manager.cpp
      terminal_manager.h
      utility_manager.cpp
      utility_manager.h
      wifi_manager.cpp
      wifi_manager.h
    hal/
      hal_battery.cpp
      hal_battery.h
      hal_display.cpp
      hal_display.h
      hal_touch.cpp
      hal_touch.h
    ui/
      bottom_nav.cpp
      bottom_nav.h
      launcher.cpp
      launcher.h
      quick_settings.cpp
      quick_settings.h
      status_bar.cpp
      status_bar.h
      theme.cpp
      theme.h
      ui_manager.cpp
      ui_manager.h
```

## Arduino IDE build and flash

1. Install Arduino IDE.
2. Add the M5Stack board manager URL:
   `https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json`
3. Install the `M5Stack` board package.
4. Install the `M5Unified` library. Let Arduino install `M5GFX` when prompted.
5. Open `TabOS.ino`.
6. Select board `M5Stack > M5Tab5`.
7. Recommended board options:
   - Chip Variant: `Before v3.00` for the connected test unit. If your Tab5 is marked v3.00 or newer and the `Before v3.00` option does not boot correctly, use `v3.00 or newer`.
   - PSRAM: `Enabled`
   - USB Mode: `Hardware CDC and JTAG`
   - USB CDC On Boot: `Enabled`
8. Connect the Tab5 by USB-C, select the port, then Upload.

## Arduino CLI build

Install the M5Stack core and libraries:

```powershell
arduino-cli core update-index --additional-urls https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
arduino-cli core install m5stack:esp32@3.3.7 --additional-urls https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
arduino-cli lib install M5Unified
```

Compile:

```powershell
$cppTarget = "$env:LOCALAPPDATA\Arduino15\packages\m5stack\tools\esp-rv32\2511\riscv32-esp-elf\include\c++\14.2.0\riscv32-esp-elf"
arduino-cli compile --fqbn "m5stack:esp32:m5stack_tab5:ChipVariant=postv3,PSRAM=enabled,USBMode=hwcdc,CDCOnBoot=cdc" --build-property "compiler.cpp.extra_flags=-I$cppTarget" .
```

The extra include path works around a Windows Arduino CLI library-detection issue observed with M5Stack core `3.3.7`. If a later core fixes it, the same command should work without `--build-property`.

For the connected test unit, use the `Before v3.00` chip variant:

```powershell
$cppTarget = "$env:LOCALAPPDATA\Arduino15\packages\m5stack\tools\esp-rv32\2511\riscv32-esp-elf\include\c++\14.2.0\riscv32-esp-elf"
arduino-cli compile --fqbn "m5stack:esp32:m5stack_tab5:ChipVariant=prev3,PSRAM=enabled,USBMode=hwcdc,CDCOnBoot=cdc" --build-property "compiler.cpp.extra_flags=-I$cppTarget" .
```

Upload:

```powershell
$cppTarget = "$env:LOCALAPPDATA\Arduino15\packages\m5stack\tools\esp-rv32\2511\riscv32-esp-elf\include\c++\14.2.0\riscv32-esp-elf"
arduino-cli compile --upload --verify --port COMx --fqbn "m5stack:esp32:m5stack_tab5:ChipVariant=prev3,PSRAM=enabled,USBMode=hwcdc,CDCOnBoot=cdc" --build-property "compiler.cpp.extra_flags=-I$cppTarget" .
```

Replace `COMx` with the Tab5 serial port shown by Arduino IDE or `arduino-cli board list`.

## Verification performed

Compiled locally with:

- Arduino CLI `1.5.1`
- M5Stack board package `m5stack:esp32@3.3.7`
- Board FQBN `m5stack:esp32:m5stack_tab5`
- M5Unified `0.2.17`
- M5GFX `0.2.24`

Compile result:

```text
Sketch uses 1141892 bytes (17%) of program storage space. Maximum is 6553600 bytes.
Global variables use 52160 bytes (15%) of dynamic memory, leaving 275520 bytes for local variables.
```

## Test checklist

- Device shows the TabOS `0.5.12-m5` splash screen on boot.
- Launcher appears after splash.
- Launcher has Core, IT Tools, Hardware, System, and Utilities pages.
- IT Tools page launches real Ping, Port Check, DNS, and QR Code apps instead of placeholder tiles.
- System page launches real Power and Developer apps instead of placeholder tiles.
- Utilities page shows Notes, Tasks, Timer, Stopwatch, Flashlight, and Macro.
- Notes app creates saved local notes and can delete the last note.
- Tasks app creates saved local tasks, toggles completion by row, and clears completed tasks.
- Timer app starts 1 minute and 5 minute timers, then stops/resets cleanly.
- Stopwatch app starts, stops, resumes from accumulated time, and resets.
- Flashlight app switches between white, red, and off, and brightness cycles without leaving the display stuck bright after Off.
- Macro Pad toggles Wi-Fi and Bluetooth, injects mesh demo traffic, toggles relay state, sets LED blue, sends a mesh ping, and logs recent macro events.
- Storage app reports NVS availability and shows virtual settings, notes, tasks, and logs entries.
- Existing Mesh, Nodes, Messages, Packet Log, Wi-Fi, Bluetooth, Terminal, Logs, Settings, Battery, and power behavior still works.
- Hardware launcher page still has Atom Control, IR Remote, GPIO, Relay, LED Control, and Sensors.
- GPIO screen starts locked.
- GPIO Arm enables simulated GPIO changes for 15 seconds.
- GPIO mode/value taps are blocked when not armed and logged.
- Relay ON/OFF changes relay state and logs action.
- Sensors app shows temperature, humidity, light, voltage, motion, and update time.
- Screen stays awake while external USB power is present.
- Screen dims and turns off after configured idle timeout when external power is not present.
- Touch wakes the screen without launching an app on the same touch.
