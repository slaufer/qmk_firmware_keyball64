## MODIFIED Requirements

### Requirement: Master half renders portrait-mode stats layout
The master half's OLED SHALL render a 16-row portrait-mode layout (5 chars wide). From top to bottom: "LAYER" label (row 0), right-justified layer number (row 1), one blank row, "CAPSK" indicator (row 3), "NUMLK" indicator (row 4), seven blank rows, then one row each for SHIFT, CTRL, SUPER, and ALT modifier indicators (rows 12–15). Each indicator row SHALL display a 5-char padded label with full-row inversion when active, and SHALL be blank (empty row) when inactive.

#### Scenario: Layer label and number shown on master
- **WHEN** the master OLED renders
- **THEN** row 0 shows "LAYER" and row 1 shows the active layer number right-justified in 5 chars

#### Scenario: CAPSK indicator active
- **WHEN** Caps Lock is enabled
- **THEN** row 3 displays "CAPSK" padded to 5 chars with full-row inversion

#### Scenario: CAPSK indicator inactive
- **WHEN** Caps Lock is disabled
- **THEN** row 3 is blank

#### Scenario: NUMLK indicator active
- **WHEN** Num Lock is enabled
- **THEN** row 4 displays "NUMLK" padded to 5 chars with full-row inversion

#### Scenario: NUMLK indicator inactive
- **WHEN** Num Lock is disabled
- **THEN** row 4 is blank

#### Scenario: SHIFT modifier active on master
- **WHEN** Shift is held on the master's physical side
- **THEN** row 12 displays "SHIFT" padded to 5 chars with full-row inversion

#### Scenario: CTRL modifier active on master
- **WHEN** Ctrl is held on the master's physical side
- **THEN** row 13 displays " CTRL" (1 leading space) padded to 5 chars with full-row inversion

#### Scenario: SUPER modifier active on master
- **WHEN** GUI/Super is held on the master's physical side
- **THEN** row 14 displays "SUPER" padded to 5 chars with full-row inversion

#### Scenario: ALT modifier active on master
- **WHEN** Alt is held on the master's physical side
- **THEN** row 15 displays " ALT " (1 leading space, 1 trailing space) padded to 5 chars with full-row inversion

#### Scenario: Inactive modifier rows are blank on master
- **WHEN** a modifier is not held
- **THEN** the corresponding row is blank

#### Scenario: Modifiers include one-shot state
- **WHEN** a one-shot modifier is active
- **THEN** the corresponding indicator row shows with full-row inversion

### Requirement: Slave half renders portrait-mode WPM and modifier layout
The slave half's OLED SHALL render a 16-row portrait-mode layout (5 chars wide). From top to bottom: " WPM" label (row 0), right-justified WPM value (row 1), one blank row, held-keys row (row 3, up to 5 chars), eight blank rows, then one row each for SHIFT, CTRL, SUPER, and ALT modifier indicators (rows 12–15). Each indicator row SHALL display a 5-char padded label with full-row inversion when active, and SHALL be blank when inactive.

#### Scenario: WPM label and value shown on slave
- **WHEN** the slave OLED renders
- **THEN** row 0 shows " WPM" (1 leading space) and row 1 shows the current WPM value right-justified in 5 chars

#### Scenario: Held-keys row shown on slave
- **WHEN** the slave OLED renders and at least one non-modifier key is held
- **THEN** row 3 displays up to 5 characters representing the currently held keys

#### Scenario: Held-keys row blank at idle
- **WHEN** no non-modifier keys are held
- **THEN** row 3 is blank

#### Scenario: SHIFT modifier active on slave
- **WHEN** Shift is held on the slave's physical side
- **THEN** row 12 displays "SHIFT" padded to 5 chars with full-row inversion

#### Scenario: CTRL modifier active on slave
- **WHEN** Ctrl is held on the slave's physical side
- **THEN** row 13 displays "CTRL " (1 trailing space) padded to 5 chars with full-row inversion

#### Scenario: SUPER modifier active on slave
- **WHEN** GUI/Super is held on the slave's physical side
- **THEN** row 14 displays "SUPER" padded to 5 chars with full-row inversion

#### Scenario: ALT modifier active on slave
- **WHEN** Alt is held on the slave's physical side
- **THEN** row 15 displays " ALT " (1 leading space, 1 trailing space) padded to 5 chars with full-row inversion

#### Scenario: Inactive modifier rows are blank on slave
- **WHEN** a modifier is not held
- **THEN** the corresponding row is blank

### Requirement: Portrait-mode orientation on both halves
Both OLEDs SHALL be initialized with `OLED_ROTATION_270` so that the text layout matches the 90° CCW physical mounting orientation of the panels.

#### Scenario: Master OLED uses rotation 270
- **WHEN** `oled_init_user` is called on the master half
- **THEN** it returns `OLED_ROTATION_270`

#### Scenario: Slave OLED uses rotation 270
- **WHEN** `oled_init_user` is called on the slave half
- **THEN** it returns `OLED_ROTATION_270`

### Requirement: Side-aware modifier detection
Each OLED SHALL display only the modifier keys that are physically located on its own half of the keyboard. The left OLED displays left-hand modifiers (LGUI, LCTL, LALT, LSFT); the right OLED displays right-hand modifiers (RGUI, RCTL, RALT, RSFT).

#### Scenario: Left OLED shows left modifiers only
- **WHEN** the left-half OLED renders modifier indicators
- **THEN** the SHIFT/CTRL/SUPER/ALT indicators reflect KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT respectively

#### Scenario: Right OLED shows right modifiers only
- **WHEN** the right-half OLED renders modifier indicators
- **THEN** the SHIFT/CTRL/SUPER/ALT indicators reflect KC_RSFT, KC_RCTL, KC_RGUI, KC_RALT respectively
