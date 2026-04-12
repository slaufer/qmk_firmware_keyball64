## 1. Enable Swap-Hands in Build System

- [x] 1.1 Add `SWAP_HANDS_ENABLE = yes` to `keyboards/keyball64/rules.mk`

## 2. Define SWAP_HANDS_MAP in config.h

- [x] 2.1 Add `SWAP_HANDS_MAP` macro to `keyboards/keyball64/config.h` covering all 80 matrix positions (10 rows × 8 cols, row-major order), mapping rows 0–4 ↔ rows 5–9 at the same column, and unused positions to themselves

## 3. Update via Keymap

- [x] 3.1 Replace one bottom-row key in Layer 0 of `keyboards/keyball64/keymaps/via/keymap.c` with `SH_TOGG` to expose the toggle

## 4. Build Verification

- [x] 4.1 Build the `via` keymap (`make keyball64:via`) and confirm it compiles without errors
- [x] 4.2 Build the `default` keymap (`make keyball64:default`) and confirm it compiles without errors
