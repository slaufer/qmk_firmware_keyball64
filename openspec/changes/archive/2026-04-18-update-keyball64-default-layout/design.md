## Context

The `default` and `via` keymaps are the two build targets for the keyball64. The `via` keymap is kept in sync with `keyball64_via.json` (the VIA configurator export) and is the canonical source of truth for the actual layout in use. The `default` keymap was never updated after the layout evolved, leaving it with a single-layer layout that differs significantly from the `via` keymap.

The VIA JSON → LAYOUT mapping is non-trivial: the flat key array in the VIA JSON is ordered row-major across the 10×8 matrix, while the LAYOUT macro takes keys in physical left-to-right, top-to-bottom order, with the right half's columns reversed (right-hand keys are stored in the matrix right-to-left but presented left-to-right in the layout). A script (`/tmp/via_to_layout.py`) was used to derive the correct LAYOUT arguments programmatically, confirming that `via/keymap.c` already matches the VIA JSON exactly.

## Goals / Non-Goals

**Goals:**
- `default/keymap.c` layer 0 matches the VIA JSON layer 0 (= `via/keymap.c` layer 0)
- `default/keymap.c` gains layers 1–3 identical to `via/keymap.c` layers 1–3
- `default` can be compiled without modification and produce firmware behaviorally equivalent to `via` for the four defined layers

**Non-Goals:**
- Adding VIA support to the `default` keymap
- Changing any behavior beyond the keymap layer definitions

## Decisions

**Copy layers 1–3 verbatim from `via/keymap.c`**: Since the via keymap is the authoritative implementation that is already validated against the VIA JSON, copying its layer definitions avoids re-deriving them and eliminates any risk of transcription error.

**Fix `via/keymap.c` key position bugs found during sync**: During the sync, three errors were discovered in `via/keymap.c` that were not caught by the prior VIA JSON mapping script:
- Layer 1 row 4: `KC_APP` and `KC_RGUI` were in the wrong order (physical position mismatch)
- Layers 2 and 3 row 4: two right-side keys were `KC_TRNS` where they should have been `OSM(MOD_LGUI|MOD_RGUI)` and `KC_APP`

Since the `default` and `via` keymaps are now being brought to parity, fixing these in `via/keymap.c` is in scope. The GPL license header and row-header comments were also removed from `via/keymap.c` to match the style of `default/keymap.c`.

**Keep `default/keymap.c` as a single-file keymap with no includes beyond `keymap_common.h`**: The default keymap already includes `keymap_common.h` which provides `SH_TOGG`, `DM_PLY1/2`, `DM_REC1/2`, `DM_RSTP`, and `OSM` — no new includes are needed.

## Risks / Trade-offs

- [Behavior change for anyone using the compiled `default` firmware] → The new default layer 0 is intentionally different from the old one. This is the desired outcome.
- [Layer 0 loses `KC_ESC` as the first key] → `KC_ESC` moves to the leftmost key of row 2, which is the actual layout in use.

## Migration Plan

1. Rewrite `keyboards/keyball64/keymaps/default/keymap.c` with the correct four-layer layout
2. Verify the file compiles (`qmk compile -kb keyball64 -km default`)
3. No EEPROM migration needed — the `default` keymap does not use EEPROM for layer storage
