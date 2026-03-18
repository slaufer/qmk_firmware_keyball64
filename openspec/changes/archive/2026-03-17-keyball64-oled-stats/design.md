## Context

The Keyball64 uses SSD1306-based OLEDs on both halves, driven over I2C. The existing `oledkit.c` layer provides weak-symbol callbacks (`oledkit_render_info_user`, `oled_task_user`, `oled_init_user`) that keymaps override. The font is embedded in `logofont.c` as 6×8-pixel tiles covering ASCII 0x20–0x7E plus custom glyphs at 0x80–0xC3.

Both OLEDs are physically mounted 90° CCW (the panel's "left" edge points up in keyboard orientation). The current secondary-half code works around a different orientation issue with `OLED_ROTATION_180`; the primary half uses the implicit default rotation. Neither half currently renders a proper portrait-mode layout.

The RP2040 is used on both halves. QMK's split transport synchronizes keyboard state. WPM is tracked in firmware but not currently synced to the secondary half.

## Goals / Non-Goals

**Goals:**
- Master half: display current layer, side-appropriate held modifiers, Caps Lock / Num Lock state
- Slave half: display current WPM and side-appropriate held modifiers
- Active modifier/lock indicators rendered with inverted colors (white background, black foreground)
- Correct portrait-mode layout on both OLEDs given 90° CCW physical mounting

**Non-Goals:**
- Trackball statistics display
- RGB or LED status
- Custom bitmap artwork
- Layer name labels (only layer number)

## Decisions

### D1: Use `OLED_ROTATION_270` for both halves

**Chosen**: Set `oled_init_user` to return `OLED_ROTATION_270` unconditionally for both master and slave.

**Rationale**: QMK's OLED driver performs a software transpose of the frame buffer when rotation is 90° or 270°, producing a virtual canvas that matches the physical orientation. `OLED_ROTATION_270` maps the scan direction to portrait mode for panels mounted 90° CCW. No per-character font pre-rotation is required; the driver handles it transparently.

**Alternative considered**: Pre-rotate every font tile with a build-time script. This avoids QMK's in-firmware transpose but increases font PROGMEM usage and makes future font edits error-prone. Rejected since QMK's software transpose has negligible cost on the RP2040.

### D2: Inverted indicators via QMK's native `invert` parameter

**Chosen**: Pass `invert = true` to `oled_write_P` / `oled_write_char` when a modifier or lock is active; `false` when inactive.

**Rationale**: QMK's OLED API includes per-character inversion at no extra flash cost. Every character in the active-state label is written with inversion, producing a white-background block. This avoids adding dedicated inverted-glyph font tiles to `logofont.c`.

### D3: WPM sync via `SPLIT_WPM_ENABLE`

**Chosen**: Add `WPM_ENABLE = yes` and `SPLIT_WPM_ENABLE = yes` to both keymap `rules.mk` files.

**Rationale**: `SPLIT_WPM_ENABLE` is the QMK-standard mechanism for broadcasting the WPM counter from master to slave over the existing split transport. The slave calls `get_current_wpm()` directly at render time.

### D4: Side-aware modifier display

**Chosen**: Each OLED always shows modifiers for its own physical side. The left OLED shows `MOD_BIT(KC_LGUI)`, `MOD_BIT(KC_LCTL)`, `MOD_BIT(KC_LALT)`, `MOD_BIT(KC_LSFT)`. The right OLED shows the `R*` counterparts. Determined at render time with `is_keyboard_left()`. One-shot modifiers are included via `get_mods() | get_oneshot_mods()`.

**Rationale**: Each hand's OLED shows only the modifier keys physically present on that half, giving direct visual feedback for the keys being held.

### D5: Render callbacks remain in `keymap.c`

**Chosen**: Override `oled_task_user` and `oled_init_user` directly in both `keymap.c` files rather than modifying `oledkit.c`.

**Rationale**: `oledkit.c` is a shared library layer intended to be keyboard-agnostic. Keymap-specific display logic belongs in the keymap. Both keymaps (default and via) will contain identical OLED code.

### D6: `SPLIT_LAYER_STATE_ENABLE` for forward compatibility

**Chosen**: Add `SPLIT_LAYER_STATE_ENABLE = yes` to both keymap `rules.mk` files.

**Rationale**: Layer number is shown on master only, but enabling split sync is low-cost on RP2040 and ensures `layer_state` is valid everywhere, preventing subtle bugs if the USB side changes.

## Risks / Trade-offs

- **Flash / RAM increase**: WPM + split WPM sync adds ~500 bytes of flash. The software OLED transpose (`OLED_ROTATION_270`) requires a ~512 B–1 KB shadow buffer. Both are negligible on RP2040 with 2 MB flash / 264 KB RAM.
- **Caps Lock lag**: `host_keyboard_led_state()` reflects OS-reported LED state, lagging by one HID report cycle (~10 ms). Not perceptible in practice.
- **Code duplication**: The OLED render functions are identical in `default/keymap.c` and `via/keymap.c`. Acceptable; factoring into a shared header adds indirection with minimal benefit.
