## 1. Code Fix

- [x] 1.1 Replace `MOD_MASK_*` with `is_keyboard_left()`-gated `MOD_BIT(KC_L*/KC_R*)` checks in `oledkit_render_info_user()` in `lib/oled_bitmap/oled_bitmap.c`
- [x] 1.2 Compile both `default` and `via` keymaps and confirm no errors

## 2. Verification

- [x] 2.1 Hold a right-side Shift key and confirm the master (left) OLED does NOT show the Shift indicator
- [x] 2.2 Hold a left-side Shift key and confirm the master (left) OLED shows the Shift indicator
