## 1. Build Config

- [x] 1.1 Add `WPM_ENABLE = yes`, `SPLIT_WPM_ENABLE = yes`, and `SPLIT_LAYER_STATE_ENABLE = yes` to `keyboards/keyball64/keymaps/default/rules.mk`
- [x] 1.2 Add the same three flags to `keyboards/keyball64/keymaps/via/rules.mk`

## 2. OLED Render Logic (default keymap)

- [x] 2.1 In `keymaps/default/keymap.c`, implement a static helper `render_mods(bool left_side)` that writes WIN/CTRL/ALT/SHIFT in two rows using `oled_write_P` with `invert=true` when the corresponding modifier is held and `invert=false` otherwise; uses `is_keyboard_left()` to select L vs R modifier bitmasks
- [x] 2.2 Implement `oledkit_render_info_user` override in `keymaps/default/keymap.c` for the master half: write "LAYER: N", then CAPS/NUM lock indicators (inverted when active), padding rows, then call `render_mods(is_keyboard_left())`
- [x] 2.3 Override `oled_task_user` in `keymaps/default/keymap.c`: call `oledkit_render_info_user()` on master, and on slave write "WPM: N", padding rows, then call `render_mods(!is_keyboard_left())`
- [x] 2.4 Override `oled_init_user` in `keymaps/default/keymap.c` to return `OLED_ROTATION_270` unconditionally

## 3. OLED Render Logic (via keymap)

- [x] 3.1 Copy the three OLED functions and the `render_mods` helper from `keymaps/default/keymap.c` into `keymaps/via/keymap.c` verbatim (identical logic)

## 4. Verification

- [x] 4.1 Confirm both keymaps compile without errors: `qmk compile -kb keyball64 -km default` and `qmk compile -kb keyball64 -km via`
- [x] 4.2 Flash and verify on hardware: master OLED shows layer, CAPS/NUM, and modifier indicators with correct inversion behavior
- [x] 4.3 Flash and verify on hardware: slave OLED shows WPM and modifier indicators with correct inversion behavior
- [x] 4.4 Verify portrait-mode orientation is correct on both OLEDs (text reads top-to-bottom with keyboard in normal use position)
