## 1. Ball Bitmap

- [x] 1.1 Add `('glyph_ball_8x8.png', 'bmp_ball')` to the `files` list in `keyball_display/png_to_c.py`, run the script, and copy the generated `bmp_ball` array into `oled_bitmap.c`

## 2. Ball State and Movement Logic

- [x] 2.1 Add static variables `ball_x`, `ball_y` (uint8_t, init 0) and `ball_dx`, `ball_dy` (int8_t, init +2) in `oled_bitmap.c`
- [x] 2.2 Implement `oled_ball_on_key_press()` in `oled_bitmap.c`: apply dx/dy, clamp x to [0, 23] reversing dx on overshoot, clamp y to [0, 87] reversing dy on overshoot (check both axes independently to handle corner collisions)
- [x] 2.3 Declare `void oled_ball_on_key_press(void)` in `oled_bitmap.h` inside the `OLED_ENABLE` guard

## 3. Slave OLED Rendering

- [x] 3.1 In the slave branch of `oled_task_user()` in `oled_bitmap.c`, call `draw_bitmap(ball_x, ball_y, bmp_ball, 8, 8)` after drawing the base image and before the modifier glyphs

## 4. Keymap Integration

- [x] 4.1 In `keyboards/keyball64/keymaps/default/keymap.c`, add a call to `oled_ball_on_key_press()` inside `process_record_user()` when `record->event.pressed` is true
- [x] 4.2 In `keyboards/keyball64/keymaps/via/keymap.c`, add the same call to `oled_ball_on_key_press()` on key press

## 5. Verification

- [x] 5.1 Confirm the firmware compiles without warnings for both `default` and `via` keymaps
- [x] 5.2 Verify ball starts at (0,0) traveling down-right, bounces at all four walls, and handles corner collisions correctly
