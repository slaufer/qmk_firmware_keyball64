## 1. Bitmap Assets

- [x] 1.1 Run `png_to_c.py` on `glyph_swaphands_16x16.png` to generate C array bytes for `bmp_swaphands`
- [x] 1.2 Run `png_to_c.py` on `glyph_record_16x16.png` to generate C array bytes for `bmp_record`
- [x] 1.3 Append `bmp_swaphands` (16×16 PROGMEM array) to `oled_bitmap_data.h`
- [x] 1.4 Append `bmp_record` (16×16 PROGMEM array) to `oled_bitmap_data.h`

## 2. Recording State Tracking

- [x] 2.1 Add `static uint8_t dynmacro_recording = 0;` module-level variable to `oled_bitmap.c`
- [x] 2.2 Implement `dynamic_macro_record_start_user(int8_t direction)` callback in `oled_bitmap.c` to set `dynmacro_recording` to 1 or 2 based on slot
- [x] 2.3 Implement `dynamic_macro_record_end_user(int8_t direction)` callback in `oled_bitmap.c` to clear `dynmacro_recording` to 0

## 3. OLED Rendering

- [x] 3.1 In `oledkit_render_info_user()`, add swap-hands indicator: draw `bmp_swaphands` at x=0, y=60 when `is_swap_hands_on()` is true
- [x] 3.2 In `oledkit_render_info_user()`, add dynmacro slot 1 indicator: draw `bmp_record` at x=0, y=60 when `dynmacro_recording == 1`
- [x] 3.3 In `oledkit_render_info_user()`, add dynmacro slot 2 indicator: draw `bmp_record` at x=16, y=60 when `dynmacro_recording == 2`

## 4. Verification

- [x] 4.1 Build firmware with `qmk compile -kb keyball64 -km via` and confirm no errors
- [x] 4.2 Flash and visually verify swap-hands indicator appears when `SH_TOGG` is pressed and disappears when toggled off
- [x] 4.3 Flash and visually verify record indicator appears at left when `DM_REC1` is held and at right when `DM_REC2` is held, and clears after `DM_RSTP`
