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

### Requirement: Shift indicator glyph
The master OLED SHALL render the `glyph_shift_16x16.png` bitmap at bounding box (x: 0–15, y: 96–111) when either Shift modifier or one-shot Shift is active, and render nothing in that area otherwise.

#### Scenario: Shift held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes any Shift bit
- **THEN** the 16×16 Shift glyph is rendered at x=0, y=96

#### Scenario: Shift not held
- **WHEN** no Shift modifier is active
- **THEN** the region x 0–15, y 96–111 is left to the base image

---

### Requirement: Control indicator glyph
The master OLED SHALL render the `glyph_control_16x16.png` bitmap at bounding box (x: 16–31, y: 96–111) when either Control modifier or one-shot Control is active.

#### Scenario: Control held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes any Control bit
- **THEN** the 16×16 Control glyph is rendered at x=16, y=96

#### Scenario: Control not held
- **WHEN** no Control modifier is active
- **THEN** the region x 16–31, y 96–111 is left to the base image

---

### Requirement: Super/Win indicator glyph
The master OLED SHALL render the `glyph_super_16x16.png` bitmap at bounding box (x: 0–15, y: 112–127) when either Super/GUI modifier or one-shot Super is active.

#### Scenario: Super held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes any GUI bit
- **THEN** the 16×16 Super glyph is rendered at x=0, y=112

#### Scenario: Super not held
- **WHEN** no Super modifier is active
- **THEN** the region x 0–15, y 112–127 is left to the base image

---

### Requirement: Alt indicator glyph
The master OLED SHALL render the `glyph_alt_16x16.png` bitmap at bounding box (x: 16–31, y: 112–127) when either Alt modifier or one-shot Alt is active.

#### Scenario: Alt held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes any Alt bit
- **THEN** the 16×16 Alt glyph is rendered at x=16, y=112

#### Scenario: Alt not held
- **WHEN** no Alt modifier is active
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
