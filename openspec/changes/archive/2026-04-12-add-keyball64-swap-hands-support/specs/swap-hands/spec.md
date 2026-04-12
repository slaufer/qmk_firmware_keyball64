## ADDED Requirements

### Requirement: Swap-hands feature is enabled at the keyboard level
The firmware SHALL enable `SWAP_HANDS_ENABLE = yes` in `keyboards/keyball64/rules.mk` so that the swap-hands keycodes and hold behavior are compiled into every Keyball64 keymap.

#### Scenario: Swap-hands keycodes compile without error
- **WHEN** the firmware is built for any Keyball64 keymap
- **THEN** the build SHALL succeed and the `SH_TOGG`, `SH_ON`, `SH_OFF`, and `SH_T()` keycodes SHALL be available for use

### Requirement: SWAP_HANDS_MAP covers the full 10×8 matrix
`config.h` SHALL define `SWAP_HANDS_MAP` as a flat array of exactly 80 `keypos_t` entries (one per matrix position, in row-major order). Each left-half position (rows 0–4) SHALL map to its mirror on the right half (rows 5–9) at the same column, and vice versa. Unused (masked) positions SHALL map to themselves.

#### Scenario: Left key triggers right-side keycode when swap is active
- **WHEN** swap-hands is active (via `SH_TOGG` or `SH_ON`)
- **WHEN** a key on the left half is pressed
- **THEN** the firmware SHALL report the keycode assigned to the mirror position on the right half

#### Scenario: Right key triggers left-side keycode when swap is active
- **WHEN** swap-hands is active
- **WHEN** a key on the right half is pressed
- **THEN** the firmware SHALL report the keycode assigned to the mirror position on the left half

#### Scenario: Normal operation is unaffected when swap is inactive
- **WHEN** swap-hands is not active
- **WHEN** any key is pressed
- **THEN** the firmware SHALL report the keycode assigned to that key's own position (no mirroring)

### Requirement: via keymap exposes a swap-hands toggle key
The `via` keymap SHALL include `SH_TOGG` on Layer 0 to demonstrate and enable the swap-hands feature without requiring a VIA remap.

#### Scenario: SH_TOGG toggles swap-hands state
- **WHEN** the user presses the key mapped to `SH_TOGG`
- **THEN** swap-hands state SHALL toggle (inactive → active, or active → inactive)
