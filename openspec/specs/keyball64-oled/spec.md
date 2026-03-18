## ADDED Requirements

### Requirement: Primary half renders layer and modifier info
The primary half's OLED SHALL display the current active layer number, the state of Caps Lock and Num Lock, and the state of the four modifier keys (GUI, Ctrl, Alt, Shift) for its physical side. Active modifier and lock indicators SHALL be rendered with inverted colors (white background, black foreground). The display SHALL use portrait-mode orientation (`OLED_ROTATION_270`).

#### Scenario: Layer 0 displayed on boot
- **WHEN** the keyboard boots with no layer active
- **THEN** the primary OLED shows layer 0 as the active layer

#### Scenario: Layer changes are reflected
- **WHEN** the active layer changes (e.g., via MO() or layer toggle)
- **THEN** the primary OLED updates to show the new active layer number

#### Scenario: Active modifiers are shown with inverted colors
- **WHEN** a modifier key (GUI, Ctrl, Alt, or Shift) is held on the primary half's physical side
- **THEN** the primary OLED shows that modifier indicator with inverted colors (white background, black foreground)

#### Scenario: Inactive modifiers are hidden
- **WHEN** a modifier key is not held
- **THEN** the corresponding indicator is not shown

#### Scenario: Caps Lock active indicator
- **WHEN** Caps Lock is enabled
- **THEN** the CAPS indicator on the primary OLED is rendered with inverted colors

#### Scenario: Num Lock active indicator
- **WHEN** Num Lock is enabled
- **THEN** the NUM indicator on the primary OLED is rendered with inverted colors

### Requirement: Secondary half renders WPM and modifier info
The secondary half's OLED SHALL display the current words-per-minute (WPM) value, a row showing currently held non-modifier keys (up to 5), and the state of the four modifier keys (GUI, Ctrl, Alt, Shift) for its physical side. Active modifier indicators SHALL be rendered with full-row inversion. The display SHALL use portrait-mode orientation (`OLED_ROTATION_270`).

#### Scenario: WPM displayed on secondary
- **WHEN** the secondary OLED renders
- **THEN** it shows the current WPM value

#### Scenario: Held keys displayed on secondary
- **WHEN** one or more non-modifier keys are held
- **THEN** the secondary OLED shows up to 5 characters representing the held keys

#### Scenario: Secondary active modifiers shown with full-row inversion
- **WHEN** a modifier key on the secondary half's physical side is held
- **THEN** the corresponding modifier indicator occupies a full row with inverted colors

#### Scenario: Secondary inactive modifiers are hidden
- **WHEN** a modifier key on the secondary half's physical side is not held
- **THEN** the corresponding row is blank

### Requirement: OLED is enabled for via and default keymaps
Both the `via` and `default` keymaps SHALL build with `OLED_ENABLE = yes` in their `rules.mk`.

#### Scenario: via keymap builds with OLED enabled
- **WHEN** `qmk compile -kb keyball64 -km via` is run
- **THEN** the build includes OLED support and produces firmware that activates both OLEDs on boot

#### Scenario: default keymap builds with OLED enabled
- **WHEN** `qmk compile -kb keyball64 -km default` is run
- **THEN** the build includes OLED support and produces firmware that activates both OLEDs on boot
