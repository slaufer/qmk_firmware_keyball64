## ADDED Requirements

### Requirement: Base image is always rendered
The slave OLED SHALL render the full 32×128 slave base display bitmap (`display_base_slave.png`) every frame as the background layer, clearing any previous content. The base image is intentionally blank.

#### Scenario: Normal operation
- **WHEN** the slave OLED task fires
- **THEN** the base image occupies all 32×128 pixels of the display before any glyphs are composited

---

### Requirement: Modifier indicators are side-aware
The slave OLED SHALL display modifier indicators only for the modifier keys physically located on the same half of the keyboard. The physical side SHALL be determined via `is_keyboard_left()`. Left-side OLEDs SHALL check `MOD_BIT(KC_LSFT)`, `MOD_BIT(KC_LCTL)`, `MOD_BIT(KC_LGUI)`, and `MOD_BIT(KC_LALT)`. Right-side OLEDs SHALL check the corresponding `KC_R*` bits. `MOD_MASK_*` macros (which match both sides) SHALL NOT be used for modifier indicator logic.

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
The slave OLED SHALL render the `glyph_shift_16x16.png` bitmap at bounding box (x: 0–15, y: 96–111) when the Shift modifier key on the same physical half is held (left half: `KC_LSFT`; right half: `KC_RSFT`), including one-shot Shift, and render nothing in that area otherwise.

#### Scenario: Shift held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes the Shift bit for this half (`KC_LSFT` if left, `KC_RSFT` if right)
- **THEN** the 16×16 Shift glyph is rendered at x=0, y=96

#### Scenario: Shift not held
- **WHEN** no same-side Shift modifier is active
- **THEN** the region x 0–15, y 96–111 is left to the base image

---

### Requirement: Control indicator glyph
The slave OLED SHALL render the `glyph_control_16x16.png` bitmap at bounding box (x: 16–31, y: 96–111) when the Control modifier key on the same physical half is held (left half: `KC_LCTL`; right half: `KC_RCTL`), including one-shot Control, and render nothing in that area otherwise.

#### Scenario: Control held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes the Control bit for this half
- **THEN** the 16×16 Control glyph is rendered at x=16, y=96

#### Scenario: Control not held
- **WHEN** no same-side Control modifier is active
- **THEN** the region x 16–31, y 96–111 is left to the base image

---

### Requirement: Super/Win indicator glyph
The slave OLED SHALL render the `glyph_super_16x16.png` bitmap at bounding box (x: 0–15, y: 112–127) when the Super/GUI modifier key on the same physical half is held (left half: `KC_LGUI`; right half: `KC_RGUI`), including one-shot Super, and render nothing in that area otherwise.

#### Scenario: Super held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes the GUI bit for this half
- **THEN** the 16×16 Super glyph is rendered at x=0, y=112

#### Scenario: Super not held
- **WHEN** no same-side Super modifier is active
- **THEN** the region x 0–15, y 112–127 is left to the base image

---

### Requirement: Alt indicator glyph
The slave OLED SHALL render the `glyph_alt_16x16.png` bitmap at bounding box (x: 16–31, y: 112–127) when the Alt modifier key on the same physical half is held (left half: `KC_LALT`; right half: `KC_RALT`), including one-shot Alt, and render nothing in that area otherwise.

#### Scenario: Alt held
- **WHEN** `(get_mods() | get_oneshot_mods())` includes the Alt bit for this half
- **THEN** the 16×16 Alt glyph is rendered at x=16, y=112

#### Scenario: Alt not held
- **WHEN** no same-side Alt modifier is active
- **THEN** the region x 16–31, y 112–127 is left to the base image

---

### Requirement: Bitmap data in PROGMEM
All glyph and base image byte arrays for the slave display SHALL be declared with `PROGMEM` and read via `pgm_read_byte`. No glyph data SHALL be stored in RAM.

#### Scenario: Bitmap declaration
- **WHEN** the firmware is compiled
- **THEN** all slave bitmap arrays use the `PROGMEM` attribute and are accessed only through `pgm_read_byte`
