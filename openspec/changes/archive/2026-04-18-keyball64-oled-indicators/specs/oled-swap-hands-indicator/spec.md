## ADDED Requirements

### Requirement: Master OLED displays swap-hands active indicator
The master OLED SHALL draw the `bmp_swaphands` 16×16 bitmap at x=0, y=60 when swap-hands is currently active, and SHALL leave that region blank otherwise.

#### Scenario: Swap-hands indicator visible when active
- **WHEN** swap-hands is enabled (e.g., after pressing `SH_TOGG`)
- **THEN** the master OLED renders the swap-hands glyph at x=0–15, y=60–75

#### Scenario: Swap-hands indicator blank when inactive
- **WHEN** swap-hands is not active
- **THEN** the master OLED does not render any glyph at x=0–15, y=60–75 (the region is left to the base image, which is blank there)
