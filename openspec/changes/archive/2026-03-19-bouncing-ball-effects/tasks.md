## 1. New Bitmap Assets

- [x] 1.1 Add `glyph_ball_8x8_mask.png`, `glyph_ball_trail_8x8.png`, and all 12 `glyph_ball_trail{1,2,3}_8x8_mask{1,2,3,4}.png` entries to `png_to_c.py`
- [x] 1.2 Run `png_to_c.py` and copy the generated C arrays for `bmp_ball_mask`, `bmp_ball_trail`, and `bmp_ball_trail{1,2,3}_mask[4]` into `oled_bitmap.c`

## 2. Masked Bitmap Draw Primitive

- [x] 2.1 Add `draw_bitmap_masked()` declaration to `oled_bitmap.h`
- [x] 2.2 Implement `draw_bitmap_masked()` in `oled_bitmap.c` — same loop as `draw_bitmap()` but skip `oled_write_pixel()` when the mask bit is OFF

## 3. Ball Position History

- [x] 3.1 Add `ball_hist_x[3]` and `ball_hist_y[3]` static arrays to `oled_bitmap.c`, initialized to `{0, 0, 0}`
- [x] 3.2 In `oled_ball_on_key_press()`, shift history before updating position: `hist[2]←hist[1]←hist[0]←(ball_x, ball_y)`
- [x] 3.3 Declare and implement `oled_ball_get_history(uint8_t *hx, uint8_t *hy)` in `oled_bitmap.h/.c` (writes 3 x-values and 3 y-values into caller-provided arrays)
- [x] 3.4 Declare and implement `oled_ball_set_history(const uint8_t *hx, const uint8_t *hy)` in `oled_bitmap.h/.c` (overwrites all 3 history slots)

## 4. Sync Payload Expansion

- [x] 4.1 In `keymaps/via/keymap.c`, update `ball_pos_slave_handler` to accept `in_buflen == 8` and call `oled_ball_set_pos()` + `oled_ball_set_history()` from the payload
- [x] 4.2 In `keymaps/via/keymap.c`, update `housekeeping_task_user()` to build an 8-byte buffer `{ball_x, ball_y, hx[0], hy[0], hx[1], hy[1], hx[2], hy[2]}` and pass it to `transaction_rpc_exec(BALL_POS_SYNC, 8, ...)`
- [x] 4.3 Apply the same sync changes to `keymaps/default/keymap.c`

## 5. Slave OLED Rendering

- [x] 5.1 Replace the `draw_bitmap(ball_x, ball_y, bmp_ball, 8, 8)` call in `oled_task_user()` with `draw_bitmap_masked(ball_x, ball_y, bmp_ball, bmp_ball_mask, 8, 8)`
- [x] 5.2 Add a `bmp_ball_trail_masks` lookup table (3 sets × 4 variants) in `oled_bitmap.c` for the trail mask arrays
- [x] 5.3 In `oled_task_user()` (slave branch), compute `trailcount` from `get_current_wpm()` using the WPM tier thresholds (0–30→0, 31–60→1, 61–90→2, 91+→3)
- [x] 5.4 Render trail segments back-to-front (highest visible index to 0): for each segment call `draw_bitmap_masked()` with `bmp_ball_trail` and a mask selected via `random8() % 4` from the appropriate mask set
- [x] 5.5 Verify ball glyph is rendered after all trail segments (already the case once trail code is inserted before the ball draw call)
