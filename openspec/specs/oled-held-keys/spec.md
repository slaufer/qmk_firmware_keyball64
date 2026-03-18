## ADDED Requirements

### Requirement: Master tracks held non-modifier keys
The master half SHALL maintain a buffer of up to 5 currently held non-modifier keycodes, updated via `process_record_user`. Modifier keycodes (Shift, Ctrl, Alt, GUI) SHALL be excluded from the buffer. When the buffer is full and a new key is pressed, the oldest entry SHALL be evicted.

#### Scenario: Key press adds to buffer
- **WHEN** a non-modifier key is pressed
- **THEN** a display character representing that key is added to the held-keys buffer

#### Scenario: Key release removes from buffer
- **WHEN** a non-modifier key is released
- **THEN** that key's entry is removed from the held-keys buffer

#### Scenario: Buffer is capped at 5
- **WHEN** 5 non-modifier keys are already held and a 6th is pressed
- **THEN** the oldest entry is evicted and the new key is added

#### Scenario: Modifier keys excluded
- **WHEN** a modifier key (Shift, Ctrl, Alt, GUI — left or right) is pressed
- **THEN** it is not added to the held-keys buffer

### Requirement: Held-keys buffer is synced to slave
The held-keys buffer SHALL be synchronized from master to slave each split transport cycle using a custom split transaction.

#### Scenario: Slave receives updated buffer
- **WHEN** the held-keys buffer changes on the master
- **THEN** the slave's copy of the buffer is updated within one split transport cycle

#### Scenario: Buffer is empty at boot
- **WHEN** the keyboard boots
- **THEN** the held-keys buffer contains only null/space characters

### Requirement: Keycodes are mapped to display characters
Non-modifier keycodes SHALL be mapped to printable ASCII characters for display. `KC_A`–`KC_Z` map to `'A'`–`'Z'`. `KC_1`–`KC_9` map to `'1'`–`'9'`. `KC_0` maps to `'0'`. Unrecognized keycodes map to `'?'`.

#### Scenario: Alpha key maps to uppercase letter
- **WHEN** `KC_A` through `KC_Z` is held
- **THEN** the corresponding uppercase letter appears in the held-keys buffer

#### Scenario: Digit key maps to digit character
- **WHEN** `KC_0` through `KC_9` is held
- **THEN** the corresponding digit character appears in the held-keys buffer

#### Scenario: Unrecognized keycode maps to question mark
- **WHEN** a non-modifier keycode with no defined display mapping is held
- **THEN** `'?'` appears in the held-keys buffer for that slot
