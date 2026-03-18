## Context

The Keyball64 has a 128×32 OLED display on each half, driven over I2C. The driver already bundles the `oledkit` library and `logofont` font, and `config.h` already defines `OLED_FONT_H`. The only missing pieces are:

1. `OLED_ENABLE = yes` in each keymap's `rules.mk`
2. A `halconf.h` at the keyboard level enabling `HAL_USE_I2C` in ChibiOS (required for I2C on RP2040)
3. A primary-half render override in the `via` keymap to show useful runtime state

The `oledkit` library's `oled_task_user` implementation already routes calls based on `is_keyboard_master()`: primary half calls `oledkit_render_info_user`, secondary calls `oledkit_render_logo_user`. Both are `__attribute__((weak))` — easily overridden per keymap.

## Goals / Non-Goals

**Goals:**
- Both OLEDs light up when `via` or `default` keymap is flashed
- Primary half shows current layer number and active modifier indicators (Shift, Ctrl, Alt, GUI)
- Secondary half shows the Keyball logofont logo (existing `oledkit_render_logo_user` default)
- No changes to core driver files or split logic

**Non-Goals:**
- WPM display on secondary half (deferred — requires custom split RPC transaction)
- OLED brightness/timeout control
- Custom graphics beyond the existing logofont

## Decisions

### halconf.h at keyboard level, not keymap level
**Decision:** Place `halconf.h` in `keyboards/keyball64/` rather than inside each keymap directory.

**Rationale:** Both keymaps need I2C enabled. A single keyboard-level file avoids duplication and ensures any future keymap also gets I2C without extra steps.

**Alternative considered:** Per-keymap `halconf.h` — rejected because it requires copying to every keymap and is error-prone.

### Layer info on primary, logo on secondary
**Decision:** Primary half renders current layer name/number and modifier state. Secondary half keeps the logo default.

**Rationale:** The primary half is the USB-connected half with access to full keyboard state (layers, modifiers). The secondary half only has matrix data available without custom split transactions. Showing the logo there is the simplest correct choice.

**Alternative considered:** Showing WPM on secondary — deferred as it requires a custom split RPC transaction (`SPLIT_TRANSACTION_IDS_KB`), which is out of scope here.

### Override in keymap.c, not a new library file
**Decision:** Implement `oledkit_render_info_user` directly in `keymaps/via/keymap.c`.

**Rationale:** The content is keymap-specific (layer names, modifier display). Keeping it in `keymap.c` makes it easy for users to customize after flashing VIA. The `default` keymap will use the `oledkit` default (logo on both halves) since users of the default keymap are expected to customize their own display.

## Risks / Trade-offs

- **OLED I2C address**: Assumes the standard SSD1306 address `0x3C`. If a board ships with `0x3D`, the display won't initialize. No mitigation needed — all known Keyball61/64 units use `0x3C`.
- **Flash size**: Enabling OLED increases firmware size. RP2040 has 2MB flash; with the current build the firmware is well under 256KB, so no concern.
- **Secondary OLED content is static**: Secondary half only shows the logo. Users who want dynamic content on both halves will need to implement split RPC — that's the WPM-sync follow-on proposal.
