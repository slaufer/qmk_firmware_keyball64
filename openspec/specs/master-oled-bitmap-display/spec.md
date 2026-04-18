### Requirement: Base image is always rendered
The master OLED SHALL render the full 32×128 base display bitmap (`display_base_master.png`) every frame as the background layer, clearing any previous content.

#### Scenario: Normal operation
- **WHEN** the master OLED task fires
- **THEN** the base image occupies all 32×128 pixels of the display before any glyphs are composited

---

### Requirement: Layer number displayed using digit glyphs
The master OLED SHALL render the current highest active layer number using the provided 15×9 digit glyphs, centered horizontally within bounding box (x: 0–31, y: 13–27).

The number SHALL be truncated to at most 3 digits (value mod 1000). Multi-digit numbers SHALL have 2 pixels of horizontal spacing between each digit glyph. Digits SHALL be horizontally centered within the 32px bounding box. At all supported digit counts (1–3), the total glyph width (9, 20, or 31px) fits within 32px without clipping.

#### Scenario: Single-digit layer
- **WHEN** the active layer is 0–9
- **THEN** one digit glyph is rendered, horizontally centered within x 0–31, vertically centered within y 13–27

#### Scenario: Two-digit layer
- **WHEN** the active layer is 10–99
- **THEN** two digit glyphs are rendered with 2px gap between them, centered within the bounding box

#### Scenario: Layer ≥ 100
- **WHEN** the active layer is 100 or greater
- **THEN** the value is taken mod 1000 and up to 3 digit glyphs are rendered, centered within the bounding box

#### Scenario: Bounding box vertical position
- **WHEN** any layer number is rendered
- **THEN** glyphs are placed at y=13 with no vertical offset (15px glyph fills the 15px-tall bounding box exactly)

---

### Requirement: WPM displayed using digit glyphs
The master OLED SHALL render the current WPM value using the provided 15×9 digit glyphs, centered horizontally within bounding box (x: 0–31, y: 43–57), with the same centering and truncation rules as the layer number.

#### Scenario: WPM renders in correct region
- **WHEN** the OLED task fires
- **THEN** WPM digits appear between y=43 and y=57, not overlapping the layer region (y 13–27)

#### Scenario: WPM ≥ 100
- **WHEN** WPM is 100 or greater
- **THEN** 3 digit glyphs are rendered with 2px gaps (total 31px), centered within the 32px bounding box without clipping

#### Scenario: Zero WPM
- **WHEN** WPM is 0
- **THEN** a single "0" digit glyph is rendered, centered

---

### Requirement: Caps Lock indicator glyph
The master OLED SHALL render the `glyph_capslock_16x16.png` bitmap at bounding box (x: 0–15, y: 78–93) when Caps Lock is enabled, and render nothing (blank) in that area when Caps Lock is disabled.

#### Scenario: Caps Lock on
- **WHEN** `host_keyboard_led_state().caps_lock` is true
- **THEN** the 16×16 Caps Lock glyph is rendered at x=0, y=78

#### Scenario: Caps Lock off
- **WHEN** `host_keyboard_led_state().caps_lock` is false
- **THEN** the region x 0–15, y 78–93 is left to the base image (no glyph overlay)

---

### Requirement: Num Lock indicator glyph
The master OLED SHALL render the `glyph_numlock_16x16.png` bitmap at bounding box (x: 16–31, y: 78–93) when Num Lock is enabled, and render nothing in that area when disabled.

#### Scenario: Num Lock on
- **WHEN** `host_keyboard_led_state().num_lock` is true
- **THEN** the 16×16 Num Lock glyph is rendered at x=16, y=78

#### Scenario: Num Lock off
- **WHEN** `host_keyboard_led_state().num_lock` is false
- **THEN** the region x 16–31, y 78–93 is left to the base image

---

### Requirement: Swap-hands indicator glyph
The master OLED SHALL render the `glyph_swaphands_16x16.png` bitmap at bounding box (x: 0–15, y: 60–75) when swap-hands is active, and render nothing in that area when inactive.

#### Scenario: Swap-hands active
- **WHEN** `is_swap_hands_on()` is true
- **THEN** the 16×16 swap-hands glyph is rendered at x=0, y=60

#### Scenario: Swap-hands inactive
- **WHEN** `is_swap_hands_on()` is false
- **THEN** the region x 0–15, y 60–75 is left to the base image

---

### Requirement: Dynamic macro recording indicator glyph
The master OLED SHALL render the `glyph_record_16x16.png` bitmap at bounding box (x: 16–31, y: 60–75) when any dynamic macro slot is being recorded, and render nothing in that area otherwise. Recording state SHALL be tracked via `dynamic_macro_record_start_user` and `dynamic_macro_record_end_user` callbacks.

#### Scenario: Recording active
- **WHEN** a dynamic macro recording is in progress (slot 1 or slot 2)
- **THEN** the 16×16 record glyph is rendered at x=16, y=60

#### Scenario: Recording inactive
- **WHEN** no dynamic macro recording is in progress
- **THEN** the region x 16–31, y 60–75 is left to the base image

---

### Requirement: Modifier indicators are side-aware
Each OLED SHALL display modifier indicators only for the modifier keys physically located on the same half of the keyboard. The physical side SHALL be determined via `is_keyboard_left()`. Left-side OLEDs SHALL check `MOD_BIT(KC_LSFT)`, `MOD_BIT(KC_LCTL)`, `MOD_BIT(KC_LGUI)`, and `MOD_BIT(KC_LALT)`. Right-side OLEDs SHALL check the corresponding `KC_R*` bits. `MOD_MASK_*` macros (which match both sides) SHALL NOT be used for modifier indicator logic.

#### Scenario: Left OLED shows only left modifiers
- **WHEN** `is_keyboard_left()` is true and only a right-side modifier (e.g. `KC_RSFT`) is held
- **THEN** the Shift indicator glyph is NOT rendered

#### Scenario: Right OLED shows only right modifiers
- **WHEN** `is_keyboard_left()` is false and only a left-side modifier (e.g. `KC_LSFT`) is held
- **THEN** the Shift indicator glyph is NOT rendered

#### Scenario: Correct side modifier triggers indicator
- **WHEN** the modifier key on the same side as the OLED is held
- **THEN** the corresponding indicator glyph is rendered

---

### Requirement: Shift indicator glyph
The master OLED SHALL render the `glyph_shift_16x16.png` bitmap at bounding box (x: 0–15, y: 96–111) when the Shift modifier key on the same physical half is held (left half: `KC_LSFT`; right half: `KC_RSFT`), including one-shot Shift, and render nothing in that area otherwise.

#### Scenario: Shift held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes the Shift bit for this half (`KC_LSFT` if left, `KC_RSFT` if right)
- **THEN** the 16×16 Shift glyph is rendered at x=0, y=96

#### Scenario: Shift not held
- **WHEN** no same-side Shift modifier is active
- **THEN** the region x 0–15, y 96–111 is left to the base image

---

### Requirement: Control indicator glyph
The master OLED SHALL render the `glyph_control_16x16.png` bitmap at bounding box (x: 16–31, y: 96–111) when the Control modifier key on the same physical half is held (left half: `KC_LCTL`; right half: `KC_RCTL`), including one-shot Control, and render nothing in that area otherwise.

#### Scenario: Control held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes the Control bit for this half
- **THEN** the 16×16 Control glyph is rendered at x=16, y=96

#### Scenario: Control not held
- **WHEN** no same-side Control modifier is active
- **THEN** the region x 16–31, y 96–111 is left to the base image

---

### Requirement: Super/Win indicator glyph
The master OLED SHALL render the `glyph_super_16x16.png` bitmap at bounding box (x: 0–15, y: 112–127) when the Super/GUI modifier key on the same physical half is held (left half: `KC_LGUI`; right half: `KC_RGUI`), including one-shot Super, and render nothing in that area otherwise.

#### Scenario: Super held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes the GUI bit for this half
- **THEN** the 16×16 Super glyph is rendered at x=0, y=112

#### Scenario: Super not held
- **WHEN** no same-side Super modifier is active
- **THEN** the region x 0–15, y 112–127 is left to the base image

---

### Requirement: Alt indicator glyph
The master OLED SHALL render the `glyph_alt_16x16.png` bitmap at bounding box (x: 16–31, y: 112–127) when the Alt modifier key on the same physical half is held (left half: `KC_LALT`; right half: `KC_RALT`), including one-shot Alt, and render nothing in that area otherwise.

#### Scenario: Alt held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes the Alt bit for this half
- **THEN** the 16×16 Alt glyph is rendered at x=16, y=112

#### Scenario: Alt not held
- **WHEN** no same-side Alt modifier is active
- **THEN** the region x 16–31, y 112–127 is left to the base image

---

### Requirement: Bitmap data in PROGMEM
All glyph and base image byte arrays SHALL be declared with `PROGMEM` and read via `pgm_read_byte`. No glyph data SHALL be stored in RAM.

#### Scenario: Bitmap declaration
- **WHEN** the firmware is compiled
- **THEN** all bitmap arrays use the `PROGMEM` attribute and are accessed only through `pgm_read_byte`

---

### Requirement: Shared implementation module
All bitmap data, rendering helpers, and master OLED render functions SHALL live in a single keyboard-level library module (not duplicated per keymap). Both the `default` and `via` keymaps SHALL share this implementation identically with no copy-pasted OLED code in either `keymap.c`.

#### Scenario: New or modified OLED code
- **WHEN** any bitmap data or rendering logic is added or changed
- **THEN** the change is made in one place and applies to all keymaps automatically

#### Scenario: Both keymaps compile with shared module
- **WHEN** both `keyball64/default` and `keyball64/via` are compiled
- **THEN** both produce correct firmware using the shared module with no OLED code duplicated in their `keymap.c` files
