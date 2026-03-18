## MODIFIED Requirements

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
