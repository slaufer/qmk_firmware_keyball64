## Why

The Keyball64 firmware lacks QMK's built-in swap-hands feature, which mirrors left and right halves of the keyboard when a hold key is pressed (or toggle is activated). This enables one-handed typing, gaming layouts, and ergonomic experiments without requiring separate keymaps.

## What Changes

- Enable `SWAP_HANDS_ENABLE` in the keyball64 keyboard-level `rules.mk`
- Define a `SWAP_HANDS_MAP` in `config.h` that mirrors the full 64-key split layout (left ↔ right)
- Expose swap-hands keycodes (`SH_TOGG`, `SH_T()`, `SH_ON`, `SH_OFF`) for use in keymaps
- Update the `via` keymap to include a swap-hands toggle key as an example

## Capabilities

### New Capabilities

- `swap-hands`: QMK swap-hands support for the Keyball64 — enables the `SH_*` keycodes and the hold-to-swap behavior by wiring up `SWAP_HANDS_ENABLE` and a correct `SWAP_HANDS_MAP` for the keyboard's matrix layout.

### Modified Capabilities

<!-- None — this is purely additive. No existing spec-level requirements change. -->

## Impact

- `keyboards/keyball64/rules.mk`: add `SWAP_HANDS_ENABLE = yes`
- `keyboards/keyball64/config.h`: add `SWAP_HANDS_MAP` macro covering all 64 matrix positions
- `keyboards/keyball64/keymaps/via/keymap.c`: add `SH_TOGG` to Layer 0 bottom row as a usage example
- No breaking changes; existing keymaps that do not use swap-hands keycodes are unaffected
