## Why

The Keyball64 has an OLED display on each half, but the `via` keymap currently builds with `OLED_ENABLE = no` (inherited from `keyboards/keyball64/rules.mk`), leaving both screens dark. The bundled `oledkit` library and logofont are already present and wired up; they just need to be activated.

## What Changes

- Add `OLED_ENABLE = yes` to `keymaps/via/rules.mk` to activate OLED for the VIA build
- Add `OLED_ENABLE = yes` to `keymaps/default/rules.mk` for consistency
- Add a `halconf.h` at the keyboard level enabling `HAL_USE_I2C` (required by ChibiOS for the I2C OLED bus on RP2040)
- Override `oledkit_render_info_user` in `keymaps/via/keymap.c` to show useful state info (current layer, modifier status) on the primary half's OLED
- Secondary half continues to display the Keyball logo via the existing `oledkit_render_logo_user` default

## Capabilities

### New Capabilities

- `keyball64-oled`: OLED display behaviour for the Keyball64 — what each half renders and under what conditions

### Modified Capabilities

- `keyball64-driver`: halconf.h addition is a build-level change to the driver

## Impact

- `keyboards/keyball64/halconf.h` — new file
- `keyboards/keyball64/keymaps/via/rules.mk` — add `OLED_ENABLE = yes`
- `keyboards/keyball64/keymaps/via/keymap.c` — add `oledkit_render_info_user` override
- `keyboards/keyball64/keymaps/default/rules.mk` — add `OLED_ENABLE = yes`
- No changes to core driver files, matrix, or split logic
