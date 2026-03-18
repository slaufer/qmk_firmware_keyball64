## MODIFIED Requirements

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
