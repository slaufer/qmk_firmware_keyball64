## ADDED Requirements

### Requirement: Keyboard builds successfully
The Keyball64 driver SHALL compile without errors using `qmk compile -kb keyball64 -km via` against QMK 0.32.x.

#### Scenario: Clean build
- **WHEN** `qmk compile -kb keyball64 -km via` is run on a system with QMK 0.32.x
- **THEN** the build succeeds and produces a `.uf2` firmware file with no errors or unresolved symbols

### Requirement: Key matrix produces 64 active key events
The driver SHALL scan a 10×8 duplex matrix (5 row pins × 4 col pins, both halves) and report exactly 64 active key positions via the matrix mask.

#### Scenario: Matrix mask excludes physically unpopulated positions
- **WHEN** a key is pressed at any of the 64 active positions defined in `LAYOUT`
- **THEN** QMK registers the keypress on the correct logical key
- **WHEN** the matrix is scanned at a masked position (col 3/7 on rows 0–2, col 3 on rows 3–4, mirrored for rows 5–9)
- **THEN** no keypress event is generated for that position

### Requirement: Split communication functions correctly
The two halves SHALL communicate over UART (GP1) using QMK's `vendor` serial driver, with left/right side detected via the matrix grid method.

#### Scenario: Left half detected
- **WHEN** the keyboard is powered and the grid detect cell (GP26, GP6) reads low
- **THEN** the MCU identifies itself as the left half

#### Scenario: Right half detected
- **WHEN** the keyboard is powered and the grid detect cell (GP26, GP6) reads high
- **THEN** the MCU identifies itself as the right half

#### Scenario: Key events from secondary half are forwarded
- **WHEN** a key on the non-USB half is pressed
- **THEN** the keystroke is reported to the host via the USB-connected half

### Requirement: No trackball, pointing device, or LED code is compiled
The driver SHALL NOT include PMW3360, SPI, `POINTING_DEVICE_ENABLE`, `SPLIT_POINTING_ENABLE`, `POINTING_DEVICE_COMBINED`, `KEYBALL_GET_INFO`, `RGBLIGHT_ENABLE`, or `RGB_MATRIX_ENABLE` in any build.

#### Scenario: Build contains no trackball or LED symbols
- **WHEN** the firmware is compiled
- **THEN** no `pmw33xx`, `pointing_device`, `keyball_get_info`, `rgblight`, or `rgb_matrix` symbols appear in the build output

### Requirement: VIA keymap is supported
The `via` keymap SHALL compile with VIA enabled, exposing all 64 keys for remapping across 4 layers.

#### Scenario: VIA remapping works
- **WHEN** the `via` keymap firmware is flashed and the keyboard is connected to a VIA-compatible configurator
- **THEN** the configurator recognizes the keyboard and allows remapping of all 64 keys

### Requirement: VIA keyboard definition is provided
A `via.json` keyboard definition file SHALL be present in the `keyboards/keyball64/` directory, describing the physical key layout for display in the VIA configurator.

#### Scenario: VIA configurator loads the definition
- **WHEN** `via.json` is loaded into the VIA configurator alongside the compiled `via` keymap firmware
- **THEN** the configurator displays all 64 keys in their correct physical positions with correct matrix coordinates

#### Scenario: No lighting section in VIA definition
- **WHEN** the `via.json` is inspected
- **THEN** `"lighting"` is set to `"none"` (the hardware has no LEDs)

### Requirement: OLED display works when enabled
When `OLED_ENABLE = yes` is set in a keymap's `rules.mk`, the OLED display SHALL initialize and render content using the Keyball logofont. The keyboard-level `halconf.h` SHALL enable `HAL_USE_I2C` so the ChibiOS I2C driver is available for RP2040 builds.

#### Scenario: OLED renders on boot
- **WHEN** firmware with `OLED_ENABLE = yes` is flashed
- **THEN** the OLED display shows content on keyboard startup without I2C errors

#### Scenario: ChibiOS I2C HAL is enabled
- **WHEN** any keymap with `OLED_ENABLE = yes` is compiled
- **THEN** the build includes `halconf.h` with `HAL_USE_I2C TRUE` (the ChibiOS I2C driver is compiled in)

### Requirement: RP2040 double-tap reset enters bootloader
Pressing reset twice within 1000ms SHALL cause the RP2040 to enter its USB bootloader (UF2 mode).

#### Scenario: Double-tap enters bootloader
- **WHEN** the reset button is double-tapped
- **THEN** the keyboard appears as a USB mass storage device for firmware flashing
