## MODIFIED Requirements

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

#### Scenario: Inactive modifiers are shown with normal colors
- **WHEN** a modifier key is not held
- **THEN** the corresponding indicator is shown with normal colors (white foreground, black background)

#### Scenario: Caps Lock active indicator
- **WHEN** Caps Lock is enabled
- **THEN** the CAPS indicator on the primary OLED is rendered with inverted colors

#### Scenario: Num Lock active indicator
- **WHEN** Num Lock is enabled
- **THEN** the NUM indicator on the primary OLED is rendered with inverted colors

## MODIFIED Requirements

### Requirement: Secondary half renders WPM and modifier info
The secondary half's OLED SHALL display the current words-per-minute (WPM) value and the state of the four modifier keys (GUI, Ctrl, Alt, Shift) for its physical side. Active modifier indicators SHALL be rendered with inverted colors. The display SHALL use portrait-mode orientation (`OLED_ROTATION_270`).

#### Scenario: WPM displayed on secondary
- **WHEN** the secondary OLED renders
- **THEN** it shows the current WPM value prefixed with "WPM: "

#### Scenario: Secondary active modifiers shown with inverted colors
- **WHEN** a modifier key on the secondary half's physical side is held
- **THEN** the corresponding modifier indicator is rendered with inverted colors

#### Scenario: Secondary inactive modifiers shown with normal colors
- **WHEN** a modifier key on the secondary half's physical side is not held
- **THEN** the corresponding indicator is rendered with normal colors

## REMOVED Requirements

### Requirement: Secondary half renders the Keyball logo
**Reason**: The secondary OLED is repurposed to show live WPM and modifier state, replacing the static decorative logo.
**Migration**: Remove `oledkit_render_logo_user` calls from the secondary half's render path. The logofont glyph data remains in flash and is still available for future use.
