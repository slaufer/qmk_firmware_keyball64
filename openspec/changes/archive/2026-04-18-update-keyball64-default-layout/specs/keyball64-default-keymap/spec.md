## ADDED Requirements

### Requirement: Default keymap layer 0 matches VIA JSON base layer
The `default` keymap layer 0 SHALL be identical to the base layer defined in `keyball64_via.json`, which is also the layer 0 of `via/keymap.c`.

#### Scenario: Row 0 number row
- **WHEN** the default keymap layer 0 is examined
- **THEN** row 0 SHALL be: `KC_1, KC_2, KC_3, KC_4, KC_5, KC_6` (left) and `KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL` (right)

#### Scenario: Row 1 QWERTY row with Backspace
- **WHEN** the default keymap layer 0 is examined
- **THEN** row 1 SHALL be: `KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T` (left) and `KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC` (right)

#### Scenario: Row 2 home row with Escape
- **WHEN** the default keymap layer 0 is examined
- **THEN** row 2 SHALL start with `KC_ESC` (not `KC_LCTL`) and continue: `KC_A, KC_S, KC_D, KC_F, KC_G` (left) and `KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT` (right)

#### Scenario: Row 3 bottom row with swap-hands thumb keys
- **WHEN** the default keymap layer 0 is examined
- **THEN** the inner thumb keys (positions L36 and R36) SHALL both be `SH_TOGG`, and the row SHALL end with `KC_ENT` (not `KC_RSFT`)

#### Scenario: Row 4 thumb cluster with layer momentary keys and arrows
- **WHEN** the default keymap layer 0 is examined
- **THEN** row 4 SHALL contain `MO(1)`, `MO(2)`, `MO(3)` as layer momentary keys, and the right side SHALL end with `KC_LEFT, KC_DOWN, KC_UP, KC_RGHT`

### Requirement: Default keymap includes layers 1–3
The `default` keymap SHALL define layers 1, 2, and 3, identical to the corresponding layers in `via/keymap.c`.

#### Scenario: Layer 1 navigation and F-keys
- **WHEN** layer 1 is active
- **THEN** the top row SHALL produce F1–F12, row 1 SHALL include Home/PgDn/PgUp/End/VolUp on the left and `[`, `]`, `\`, Del on the right, and row 2 SHALL include arrow keys and VolDn on the left with arrow keys and Ins on the right

#### Scenario: Layer 1 media and modifier keys
- **WHEN** layer 1 is active
- **THEN** row 3 left SHALL include Prev/Play/Stop/Next/Mute, and row 4 right SHALL have `KC_RALT, KC_RGUI, KC_APP, KC_RCTL`

#### Scenario: Layer 2 window management and dynamic macro play
- **WHEN** layer 2 is active
- **THEN** positions DM_PLY1 and DM_PLY2 SHALL be on the top-left two keys, row 1 left SHALL include browser tab switching (`C(KC_PGUP)`, `C(KC_PGDN)`) and Cmd+bracket navigation, and row 2 left SHALL include Cmd+arrow window movement

#### Scenario: Layer 2 one-shot modifiers
- **WHEN** layer 2 is active
- **THEN** row 3 left SHALL start with `OSM(MOD_LSFT)`, row 3 right SHALL end with `OSM(MOD_LSFT|MOD_RSFT)`, and row 4 SHALL include `OSM(MOD_LCTL)`, `OSM(MOD_LGUI)`, `OSM(MOD_LALT)` on the left and `OSM(MOD_LALT|MOD_RALT)`, `OSM(MOD_LGUI|MOD_RGUI)`, `KC_APP`, `OSM(MOD_LCTL|MOD_RCTL)` on the right

#### Scenario: Layer 3 dynamic macro record and EEPROM reset
- **WHEN** layer 3 is active
- **THEN** the top-left three keys SHALL be `DM_REC1, DM_REC2, DM_RSTP`, and the top-right last key SHALL be `QK_CLEAR_EEPROM`

#### Scenario: Layer 3 one-shot modifiers
- **WHEN** layer 3 is active
- **THEN** row 3 left SHALL start with `OSM(MOD_LSFT)`, row 3 right SHALL end with `OSM(MOD_LSFT|MOD_RSFT)`, and row 4 SHALL have `OSM(MOD_LCTL)`, `OSM(MOD_LGUI)`, `OSM(MOD_LALT)` on the left and `OSM(MOD_LALT|MOD_RALT)`, `OSM(MOD_LGUI|MOD_RGUI)`, `KC_APP`, `OSM(MOD_LCTL|MOD_RCTL)` on the right
