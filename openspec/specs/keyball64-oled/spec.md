## ADDED Requirements

### Requirement: Primary half renders layer and modifier info
The primary half's OLED SHALL display the current active layer number and the state of the four modifier keys (Shift, Ctrl, Alt, GUI) as text.

#### Scenario: Layer 0 displayed on boot
- **WHEN** the keyboard boots with no layer active
- **THEN** the primary OLED shows layer 0 as the active layer

#### Scenario: Layer changes are reflected
- **WHEN** the active layer changes (e.g., via MO() or layer toggle)
- **THEN** the primary OLED updates to show the new active layer number

#### Scenario: Active modifiers are shown
- **WHEN** a modifier key (Shift, Ctrl, Alt, or GUI) is held
- **THEN** the primary OLED shows that modifier as active

### Requirement: Secondary half renders the Keyball logo
The secondary half's OLED SHALL render the Keyball logofont logo using `oledkit_render_logo_user`.

#### Scenario: Logo displays on secondary boot
- **WHEN** the keyboard boots and the secondary half's OLED is enabled
- **THEN** the Keyball logofont logo is rendered on the secondary OLED

#### Scenario: Secondary logo is rotated 180 degrees
- **WHEN** the secondary OLED initializes
- **THEN** `oled_init_user` returns `OLED_ROTATION_180` so the logo appears right-side up given the physical mounting orientation

### Requirement: OLED is enabled for via and default keymaps
Both the `via` and `default` keymaps SHALL build with `OLED_ENABLE = yes` in their `rules.mk`.

#### Scenario: via keymap builds with OLED enabled
- **WHEN** `qmk compile -kb keyball64 -km via` is run
- **THEN** the build includes OLED support and produces firmware that activates both OLEDs on boot

#### Scenario: default keymap builds with OLED enabled
- **WHEN** `qmk compile -kb keyball64 -km default` is run
- **THEN** the build includes OLED support and produces firmware that activates both OLEDs on boot
