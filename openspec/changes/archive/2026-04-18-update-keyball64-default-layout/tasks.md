## 1. Update default keymap

- [x] 1.1 Rewrite `keyboards/keyball64/keymaps/default/keymap.c` layer 0 to match `via/keymap.c` layer 0 (update the comment header to reflect the new layout, replace all key definitions)
- [x] 1.2 Add layer 1 (navigation / F-keys / media) to `default/keymap.c`, copied verbatim from `via/keymap.c`
- [x] 1.3 Add layer 2 (window management / one-shot mods / dynamic macro play) to `default/keymap.c`, copied verbatim from `via/keymap.c`
- [x] 1.4 Add layer 3 (dynamic macro record / EEPROM reset / one-shot mods) to `default/keymap.c`, copied verbatim from `via/keymap.c`

## 2. Fix via keymap bugs discovered during sync

- [x] 2.1 Fix layer 1 row 4 in `via/keymap.c`: swap `KC_APP` and `KC_RGUI` to correct physical position ordering (`KC_RALT, KC_RGUI, KC_APP, KC_RCTL`)
- [x] 2.2 Fix layers 2 and 3 row 4 in `via/keymap.c`: replace `KC_TRNS` with `OSM(MOD_LGUI|MOD_RGUI)` and `KC_APP` at their correct positions
- [x] 2.3 Remove GPL license header and row-header comments from `via/keymap.c` to match `default/keymap.c` style

## 3. Verify

- [x] 3.1 Compile the `default` keymap (`qmk compile -kb keyball64 -km default`) and confirm it succeeds with no errors
- [x] 3.2 Diff `default/keymap.c` against `via/keymap.c` and confirm the files are now identical
