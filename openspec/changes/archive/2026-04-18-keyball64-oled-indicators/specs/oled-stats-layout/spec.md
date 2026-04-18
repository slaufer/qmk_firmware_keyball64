## MODIFIED Requirements

### Requirement: Master half renders portrait-mode stats layout
The master half's OLED SHALL render a 32×128 px bitmap display. From top to bottom: a "KEYBALL" logo (rows 0–11, y=0–11), layer number centered in y=13–27, WPM centered in y=43–57, Caps Lock indicator at x=0–15 y=78–93, Num Lock indicator at x=16–31 y=78–93, swap-hands active indicator at x=0–15 y=60–75, dynamic macro recording indicator at x=16–31 y=60–75, Shift indicator at x=0–15 y=96–111, Control at x=16–31 y=96–111, Super at x=0–15 y=112–127, Alt at x=16–31 y=112–127. Each indicator SHALL display its 16×16 glyph when active and leave the region blank otherwise. The display SHALL use portrait-mode orientation (`OLED_ROTATION_270`).

#### Scenario: Swap-hands indicator zone rendered correctly
- **WHEN** the master OLED renders
- **THEN** x=0–15, y=60–75 shows the swap-hands glyph if active, or is blank if inactive

#### Scenario: Dynmacro recording indicator zone rendered correctly
- **WHEN** the master OLED renders
- **THEN** x=16–31, y=60–75 shows the record glyph if any slot is recording, and is blank otherwise

#### Scenario: Layer label and number shown on master
- **WHEN** the master OLED renders
- **THEN** the active layer number is drawn centered in the bounding box x=0–31, y=13–27

#### Scenario: WPM shown on master
- **WHEN** the master OLED renders
- **THEN** the current WPM value is drawn centered in the bounding box x=0–31, y=43–57

#### Scenario: CAPSK indicator active
- **WHEN** Caps Lock is enabled
- **THEN** the caps lock glyph is drawn at x=0–15, y=78–93

#### Scenario: CAPSK indicator inactive
- **WHEN** Caps Lock is disabled
- **THEN** x=0–15, y=78–93 is blank

#### Scenario: NUMLK indicator active
- **WHEN** Num Lock is enabled
- **THEN** the num lock glyph is drawn at x=16–31, y=78–93

#### Scenario: NUMLK indicator inactive
- **WHEN** Num Lock is disabled
- **THEN** x=16–31, y=78–93 is blank

#### Scenario: Modifier indicators active on master
- **WHEN** a modifier (Shift, Control, Super, Alt) is held or one-shot active on the master's physical side
- **THEN** the corresponding 16×16 glyph is drawn at its bounding box

#### Scenario: Inactive modifier rows are blank on master
- **WHEN** a modifier is not held and no one-shot is active
- **THEN** the corresponding bounding box is blank
