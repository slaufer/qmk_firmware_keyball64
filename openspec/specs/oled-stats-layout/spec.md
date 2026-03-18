## ADDED Requirements

### Requirement: Master half renders portrait-mode stats layout
The master half's OLED SHALL render a portrait-mode layout (top to bottom) containing: current active layer number, Caps Lock and Num Lock state indicators, empty padding rows, and two rows of side-appropriate held-modifier indicators.

#### Scenario: Layer number shown on master
- **WHEN** the master OLED renders
- **THEN** the first section displays "LAYER" followed by the current highest active layer number

#### Scenario: Lock indicators shown on master
- **WHEN** the master OLED renders
- **THEN** the second section displays "CAPS" and "NUM" indicators, each rendered with inverted colors (white background, black foreground) when the corresponding lock is active, and hidden when inactive

#### Scenario: Modifier indicators shown on master
- **WHEN** the master OLED renders
- **THEN** the bottom section displays "WIN", "CTRL", "ALT", and "SHFT" indicators for the physical side of the keyboard that is master, each rendered with inverted colors when the corresponding modifier is held, and hidden when not held

#### Scenario: Modifiers include one-shot state
- **WHEN** a one-shot modifier is active on the master half
- **THEN** the corresponding modifier indicator is rendered with inverted colors

### Requirement: Slave half renders portrait-mode WPM and modifier layout
The slave half's OLED SHALL render a portrait-mode layout containing: current WPM, empty padding rows, and two rows of side-appropriate held-modifier indicators.

#### Scenario: WPM shown on slave
- **WHEN** the slave OLED renders
- **THEN** the first section displays "WPM" followed by the current words-per-minute value

#### Scenario: WPM reads zero at idle
- **WHEN** no keys have been typed recently
- **THEN** the slave OLED displays WPM as 0

#### Scenario: Modifier indicators shown on slave
- **WHEN** the slave OLED renders
- **THEN** the bottom section displays "WIN", "CTRL", "ALT", and "SHFT" indicators for the physical side of the keyboard that is slave, each rendered with inverted colors when the corresponding modifier is held, and hidden when not held

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
- **THEN** the WIN/CTRL/ALT/SHFT indicators reflect the state of KC_LGUI, KC_LCTL, KC_LALT, KC_LSFT respectively

#### Scenario: Right OLED shows right modifiers only
- **WHEN** the right-half OLED renders modifier indicators
- **THEN** the WIN/CTRL/ALT/SHFT indicators reflect the state of KC_RGUI, KC_RCTL, KC_RALT, KC_RSFT respectively
