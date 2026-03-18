## 1. Digit Glyph Refresh

- [x] 1.1 Run `png_to_c.py` on each of the 10 digit PNGs (`glyph_0_15x9.png` – `glyph_9_15x9.png`) and update the `bmp_digits` array data in `oled_bitmap.c`

## 2. Slave Base Image

- [x] 2.1 Run `png_to_c.py` on `display_base_slave.png` to generate the 512-byte PROGMEM array
- [x] 2.2 Add `bmp_base_slave[512]` PROGMEM declaration to `oled_bitmap.c`

## 3. Slave Bitmap Rendering

- [x] 3.1 Replace the text-based slave branch in `oled_task_user()` with bitmap rendering: draw `bmp_base_slave` as background, then composite Shift/Control/Super/Alt glyphs at their bounding boxes using `is_keyboard_left()`-gated `MOD_BIT()` checks

## 4. Build Verification

- [x] 4.1 Compile both `default` and `via` keymaps and confirm no errors or warnings

## 5. Hardware Verification

- [x] 5.1 Confirm slave OLED renders a clean display (base image only, no text artifacts)
- [x] 5.2 Hold a same-side modifier on the right half and confirm the correct glyph appears on the slave OLED
- [x] 5.3 Hold a modifier on the opposite side and confirm no glyph appears on the slave OLED
