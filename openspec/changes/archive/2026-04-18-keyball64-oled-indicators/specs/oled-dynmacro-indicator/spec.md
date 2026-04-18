## ADDED Requirements

### Requirement: Master OLED displays dynamic macro recording indicator
The master OLED SHALL draw the `bmp_record` 16×16 bitmap at x=16, y=60 when any dynamic macro slot is being recorded. When no recording is active the position SHALL be blank (left to the base image).

#### Scenario: Recording indicator shown when recording starts
- **WHEN** the user activates `DM_REC1` or `DM_REC2` and recording begins
- **THEN** the master OLED renders the record glyph at x=16–31, y=60–75

#### Scenario: Recording indicator is cleared after stop
- **WHEN** recording is stopped (via `DM_RSTP` or pressing a record key again)
- **THEN** x=16–31, y=60–75 is blank

#### Scenario: No indicator shown when not recording
- **WHEN** no dynamic macro recording is in progress
- **THEN** x=16–31, y=60–75 does not show the record glyph

### Requirement: Recording state is tracked via QMK callbacks
The firmware SHALL maintain a module-level `dynmacro_recording` flag (0=idle, 1=slot 1, 2=slot 2) updated by implementing `dynamic_macro_record_start_user()` and `dynamic_macro_record_end_user()` in `oled_bitmap.c`.

#### Scenario: Flag set on record start
- **WHEN** `dynamic_macro_record_start_user(slot)` is called with slot 1 or 2
- **THEN** `dynmacro_recording` is set to 1 or 2 respectively

#### Scenario: Flag cleared on record end
- **WHEN** `dynamic_macro_record_end_user(slot)` is called
- **THEN** `dynmacro_recording` is set to 0
