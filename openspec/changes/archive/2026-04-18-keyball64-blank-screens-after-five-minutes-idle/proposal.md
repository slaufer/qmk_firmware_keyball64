## Why

The OLED displays on both halves show static or near-static content when the keyboard is idle, which causes OLED burn-in over time and wastes power. Blanking the screens after 5 minutes of inactivity protects the hardware with no user-visible downside — screens wake immediately on the next keypress or mouse movement.

## What Changes

- Add `OLED_TIMEOUT 300000` to the `via` keymap's `config.h` (and `default` keymap's `config.h`), enabling QMK's built-in OLED auto-off after 300 000 ms (5 minutes) of inactivity.

## Capabilities

### New Capabilities
- `oled-idle-timeout`: Both OLED screens blank after 5 minutes of keyboard/mouse inactivity and wake on the next input event.

### Modified Capabilities
<!-- none -->

## Impact

- `keyboards/keyball64/keymaps/via/config.h` — add `OLED_TIMEOUT` define
- `keyboards/keyball64/keymaps/default/config.h` — add `OLED_TIMEOUT` define
- No changes to `oled_bitmap.c` or display logic; QMK's driver handles the power-down internally.
