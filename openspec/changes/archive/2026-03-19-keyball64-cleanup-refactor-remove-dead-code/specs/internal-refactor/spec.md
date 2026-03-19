## ADDED Requirements

### Requirement: Keymap common logic is not duplicated
The ball-position sync handler, QMK hook implementations, and shared includes common to the `default` and `via` keymaps SHALL be defined in a single location (`lib/keymap_common/keymap_common.h`) and included by both keymaps.

#### Scenario: Default keymap builds from shared header
- **WHEN** `qmk compile -kb keyball64 -km default` is run
- **THEN** the build succeeds with no errors or warnings

#### Scenario: Via keymap builds from shared header
- **WHEN** `qmk compile -kb keyball64 -km via` is run
- **THEN** the build succeeds with no errors or warnings
