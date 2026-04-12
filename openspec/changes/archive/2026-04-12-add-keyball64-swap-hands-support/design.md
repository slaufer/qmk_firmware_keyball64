## Context

The Keyball64 is a split keyboard built on RP2040 using a 10×8 duplex matrix (5 rows × 4 cols per side, doubled for duplex). Rows 0–4 are the left half; rows 5–9 are the right half. Columns 3 and unused matrix positions are masked out via `MATRIX_MASKED`.

QMK's swap-hands feature requires two things: `SWAP_HANDS_ENABLE = yes` in the build system, and a `SWAP_HANDS_MAP` in `config.h` — a flat array of `keypos_t` entries, one per matrix position, each pointing to the mirrored position.

## Goals / Non-Goals

**Goals:**
- Enable the `SH_TOGG`, `SH_T()`, `SH_ON`, and `SH_OFF` keycodes for all Keyball64 keymaps
- Define a correct `SWAP_HANDS_MAP` that mirrors left ↔ right across the full 10×8 matrix
- Demonstrate usage with `SH_TOGG` in the `via` keymap bottom row

**Non-Goals:**
- Changing the default keymap layout beyond the swap-hands toggle example
- Adding swap-hands support to the `default` keymap (it can be added by users)
- Integrating swap-hands state with the OLED display

## Decisions

### Decision: Place SWAP_HANDS_ENABLE in keyboard-level rules.mk, not per-keymap

Adding it at the keyboard level makes it available to all keymaps (default, via, and any user keymaps) without requiring each keymap to opt in. The feature has zero cost unless a swap-hands keycode is actually used.

_Alternatives considered_: Per-keymap opt-in — rejected because the feature is lightweight and universally useful.

### Decision: SWAP_HANDS_MAP mirrors by row offset (+5 / -5)

For the Keyball64 duplex matrix, left rows are 0–4 and right rows are 5–9 with the same column layout. The mirror mapping is simply `{row ± 5, col}` for every position. Unused matrix positions (masked out) map to themselves so they remain inert.

The resulting map covers all 80 positions (10 rows × 8 cols):

```
Row 0 (left)  ↔ Row 5 (right)
Row 1 (left)  ↔ Row 6 (right)
Row 2 (left)  ↔ Row 7 (right)
Row 3 (left)  ↔ Row 8 (right)
Row 4 (left)  ↔ Row 9 (right)
```

Column assignments are preserved across the swap (col 3 is masked and maps to itself).

_Alternatives considered_: Explicit per-key mapping — not needed; the matrix is symmetric so the offset formula is correct and maintainable.

### Decision: Add SH_TOGG to via keymap as example, not default keymap

The `via` keymap is the user-facing keymap meant for remapping via VIA. Adding `SH_TOGG` there demonstrates the feature without disrupting the `default` layout, which is a reference implementation.

## Risks / Trade-offs

- [SWAP_HANDS_MAP must cover all 80 positions exactly] → The macro must list entries for every `row × col` combination in row-major order. Off-by-one errors silently produce wrong key mappings. Mitigation: verify count = `MATRIX_ROWS × MATRIX_COLS = 80`.
- [Unused matrix positions must not be omitted] → Masked positions still need a valid entry in the map. They map to themselves. Mitigation: fill all 80 slots, including col 3.
- [SH_TOGG placement in via keymap] → Placing it on an existing key changes the layer 0 default; the `via` keymap is already intended for customization so this is acceptable. Mitigation: place it on a bottom-row key that is currently `KC_TRNS` or an auxiliary modifier.
