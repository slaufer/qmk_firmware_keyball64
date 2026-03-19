#pragma once

#if defined(OLED_ENABLE) && !defined(OLED_BITMAP_DISABLE)

#include <stdint.h>
#include <stdbool.h>

// Draw a PROGMEM bitmap at pixel (x0, y0).
// Data is row-major with (w+7)/8 bytes per row, MSB = leftmost pixel.
void draw_bitmap(uint8_t x0, uint8_t y0, const uint8_t *data, uint8_t w, uint8_t h);

// Draw a PROGMEM bitmap at pixel (x0, y0) using a PROGMEM mask of the same dimensions.
// Only pixels where the mask bit is ON are written; mask-OFF pixels are skipped.
void draw_bitmap_masked(uint8_t x0, uint8_t y0, const uint8_t *data, const uint8_t *mask, uint8_t w, uint8_t h);

// Draw up to 3 digits of value (mod 1000) using the 9×15 digit glyphs,
// centered horizontally in the 32px-wide region starting at (box_x, box_y).
void draw_number(uint16_t value, uint8_t box_x, uint8_t box_y);

// Advance the bouncing ball by one step. Call from process_record_user()
// on key press events to animate the ball on the slave OLED.
void oled_ball_on_key_press(void);

// Read the current ball position (used on the master to send synced state).
void oled_ball_get_pos(uint8_t *x, uint8_t *y);

// Overwrite the ball position (used on the slave to apply synced state).
void oled_ball_set_pos(uint8_t x, uint8_t y);

// Read the 3-entry position history into caller-provided arrays of length 3.
// Index 0 = most recent previous position, index 2 = oldest.
void oled_ball_get_history(uint8_t *hx, uint8_t *hy);

// Overwrite all 3 history slots (used on the slave to apply synced state).
void oled_ball_set_history(const uint8_t *hx, const uint8_t *hy);

#endif // OLED_ENABLE
