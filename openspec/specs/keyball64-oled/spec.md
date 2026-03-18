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
The secondary half's OLED SHALL render a base display bitmap every frame, with modifier indicator glyphs (Shift, Control, Super, Alt) composited at their designated bounding boxes for the physical side of that half. No WPM value or held-key characters are displayed. The display SHALL use portrait-mode orientation (`OLED_ROTATION_270`).

#### Scenario: Slave OLED renders base image
- **WHEN** the secondary OLED renders
- **THEN** the full 32×128 slave base bitmap is drawn as the background layer

#### Scenario: Secondary active modifiers shown as glyphs
- **WHEN** a modifier key on the secondary half's physical side is held
- **THEN** the corresponding 16×16 modifier glyph is composited at its bounding box

#### Scenario: Secondary inactive modifiers are blank
- **WHEN** a modifier key on the secondary half's physical side is not held
- **THEN** the corresponding bounding box region is left to the base image

### Requirement: OLED is enabled for via and default keymaps
Both the `via` and `default` keymaps SHALL build with `OLED_ENABLE = yes` in their `rules.mk`.

#### Scenario: via keymap builds with OLED enabled
- **WHEN** `qmk compile -kb keyball64 -km via` is run
- **THEN** the build includes OLED support and produces firmware that activates both OLEDs on boot

#### Scenario: default keymap builds with OLED enabled
- **WHEN** `qmk compile -kb keyball64 -km default` is run
- **THEN** the build includes OLED support and produces firmware that activates both OLEDs on boot
