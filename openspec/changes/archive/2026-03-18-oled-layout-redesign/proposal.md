## Why

The current OLED layout uses short 5-char-wide text that doesn't make good use of the available rows, and the slave display lacks any indication of which non-modifier keys are currently held. This redesign establishes a clean, full-width layout for both halves and adds a live held-key indicator to the slave.

## What Changes

- Master display layout revised: label and value on separate rows, full-row inverted indicators for CAPS LOCK / NUM LOCK, single-modifier-per-row display with full-row inversion when active
- Slave display layout revised: label and value on separate rows, a 6-character "currently held keys" row showing active non-modifier keypresses, single-modifier-per-row display with full-row inversion when active
- All inactive indicators are completely hidden (no placeholder text)
- A `process_record_user` hook tracks currently held non-modifier keys and exposes them for the slave OLED render path

## Capabilities

### New Capabilities

- `oled-held-keys`: Tracks currently held non-modifier keys (alphanumeric, symbol) via `process_record_user` and exposes a buffer for display on the slave OLED

### Modified Capabilities

- `oled-stats-layout`: All layout requirements change — row structure, label placement, full-row inversion, and the addition of the held-key row on the slave half
- `keyball64-oled`: Inactive modifier and lock indicators are now hidden rather than shown in normal color; layout changes for both halves

## Impact

- `keyboards/keyball64/keymaps/default/keymap.c` — OLED render functions and new `process_record_user`
- `keyboards/keyball64/keymaps/via/keymap.c` — same
