## Why

The `default` keymap's layer 0 diverges from the actual layout used in practice (captured in `keyball64_via.json`), and it lacks layers 1–3 entirely. The `via` keymap already matches the VIA JSON across all four layers, but the `default` keymap has never been synchronized. This makes the compiled default firmware useless as a starting point for anyone who wants to build without VIA support.

## What Changes

- Rewrite `keyboards/keyball64/keymaps/default/keymap.c` layer 0 to match layer 0 from `keyball64_via.json` (currently matches `via/keymap.c` layer 0)
- Add layers 1–3 to `default/keymap.c`, identical to `via/keymap.c` layers 1–3
- Update `via/keymap.c`: remove GPL license header and row-header comments from layer 0; fix three key position errors found during sync:
  - Layer 1 row 4: `KC_APP` and `KC_RGUI` were swapped (correct order: `KC_RALT, KC_RGUI, KC_APP, KC_RCTL`)
  - Layers 2 and 3 row 4: right-side `KC_TRNS` replaced with `OSM(MOD_LGUI|MOD_RGUI)`, and `OSM(MOD_LGUI|MOD_RGUI)` slot replaced with `KC_APP`

Specific layer 0 corrections (derived from programmatic VIA JSON → LAYOUT mapping):
- Row 0: remove leading `KC_ESC`; number row starts at `KC_1`; right side gains `KC_MINS`, `KC_EQL` (removes `KC_BSPC`)
- Row 1: right-hand end changes from `KC_DEL` to `KC_BSPC`
- Row 2: leftmost key changes from `KC_LCTL` to `KC_ESC`
- Row 3: inner thumb keys change from `KC_LBRC`/`KC_RBRC` to `SH_TOGG`/`SH_TOGG`; row ends with `KC_ENT` instead of `KC_RSFT`
- Row 4: full redesign — `KC_LGUI` duplicated, `MO(1)`/`MO(2)`/`MO(3)` added, right side gets arrow keys; removes `KC_DEL`, standalone `KC_RCTL`/`KC_RALT`/`KC_RGUI` from bottom row

New layers added to `default`:
- Layer 1: navigation (arrows, home/end/pgup/pgdn), F-keys, media keys, `[`, `]`, `\`
- Layer 2: window management (Cmd+arrows, tab switching), one-shot mods, dynamic macro play
- Layer 3: dynamic macro record/stop, EEPROM reset, one-shot mods

## Capabilities

### New Capabilities

- `keyball64-default-keymap`: Full four-layer keymap for the default build target, synchronized with the VIA JSON reference layout

### Modified Capabilities

<!-- None -->

## Impact

- `keyboards/keyball64/keymaps/default/keymap.c`: layer 0 rewritten, layers 1–3 added
- `via/keymap.c`: GPL header and row comments removed; three key position bugs fixed in layers 1–3
