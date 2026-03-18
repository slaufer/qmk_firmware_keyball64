## MODIFIED Requirements

### Requirement: OLED display works when enabled
When `OLED_ENABLE = yes` is set in a keymap's `rules.mk`, the OLED display SHALL initialize and render content using the Keyball logofont. The keyboard-level `halconf.h` SHALL enable `HAL_USE_I2C` so the ChibiOS I2C driver is available for RP2040 builds.

#### Scenario: OLED renders on boot
- **WHEN** firmware with `OLED_ENABLE = yes` is flashed
- **THEN** the OLED display shows content on keyboard startup without I2C errors

#### Scenario: ChibiOS I2C HAL is enabled
- **WHEN** any keymap with `OLED_ENABLE = yes` is compiled
- **THEN** the build includes `halconf.h` with `HAL_USE_I2C TRUE` (the ChibiOS I2C driver is compiled in)
