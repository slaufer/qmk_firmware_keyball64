#include "quantum.h"

#if defined(OLED_ENABLE) && !defined(OLED_BITMAP_DISABLE)

// ---------------------------------------------------------------------------
// Bitmap data (PROGMEM)
// ---------------------------------------------------------------------------

#include "oled_bitmap_data.h"

// ---------------------------------------------------------------------------
// Bouncing ball state
// ---------------------------------------------------------------------------

static uint8_t dynmacro_recording = 0;

bool dynamic_macro_record_start_user(int8_t direction) {
    dynmacro_recording = (direction == 1) ? 1 : 2;
    return true;
}

bool dynamic_macro_record_end_user(int8_t direction) {
    dynmacro_recording = 0;
    return true;
}

// Ball position (top-left corner). Valid range: x ∈ [0,23], y ∈ [0,87].
static uint8_t ball_x  = 0;
static uint8_t ball_y  = 0;
static int8_t  ball_dx = 4;
static int8_t  ball_dy = 4;

// Trail history: index 0 = most recent previous position, index 2 = oldest.
static uint8_t ball_hist_x[3] = {0, 0, 0};
static uint8_t ball_hist_y[3] = {0, 0, 0};

// Simple LCG PRNG — avoids lib8tion dependency.
static uint8_t ball_prng_state = 42;
static uint8_t ball_rand8(void) {
    ball_prng_state = ball_prng_state * 109 + 89;
    return ball_prng_state;
}


void oled_ball_tick(void) {
    // Shift history before updating position
    ball_hist_x[2] = ball_hist_x[1]; ball_hist_y[2] = ball_hist_y[1];
    ball_hist_x[1] = ball_hist_x[0]; ball_hist_y[1] = ball_hist_y[0];
    ball_hist_x[0] = ball_x;         ball_hist_y[0] = ball_y;

    int16_t nx = (int16_t)ball_x + ball_dx;
    int16_t ny = (int16_t)ball_y + ball_dy;

    if (nx < 0 || nx > 23) {
        ball_dx = -ball_dx;
        nx = nx < 0 ? 0 : 23;
    }
    if (ny < 0 || ny > 87) {
        ball_dy = -ball_dy;
        ny = ny < 0 ? 0 : 87;
    }

    ball_x = (uint8_t)nx;
    ball_y = (uint8_t)ny;
}

// ---------------------------------------------------------------------------
// Rendering helpers
// ---------------------------------------------------------------------------

void draw_bitmap(uint8_t x0, uint8_t y0,
                 const uint8_t *data, uint8_t w, uint8_t h) {
    uint8_t bpr = (w + 7) / 8;
    for (uint8_t row = 0; row < h; row++) {
        for (uint8_t col = 0; col < w; col++) {
            uint8_t b  = pgm_read_byte(data + row * bpr + col / 8);
            bool    on = (b >> (7 - col % 8)) & 1;
            oled_write_pixel(x0 + col, y0 + row, on);
        }
    }
}

void draw_bitmap_masked(uint8_t x0, uint8_t y0,
                        const uint8_t *data, const uint8_t *mask,
                        uint8_t w, uint8_t h) {
    uint8_t bpr = (w + 7) / 8;
    for (uint8_t row = 0; row < h; row++) {
        for (uint8_t col = 0; col < w; col++) {
            uint8_t m = pgm_read_byte(mask + row * bpr + col / 8);
            if (!((m >> (7 - col % 8)) & 1)) continue;
            uint8_t b  = pgm_read_byte(data + row * bpr + col / 8);
            bool    on = (b >> (7 - col % 8)) & 1;
            oled_write_pixel(x0 + col, y0 + row, on);
        }
    }
}

void draw_number(uint16_t value, uint8_t box_x, uint8_t box_y) {
    uint16_t v = value % 1000;
    uint8_t  d[3];
    uint8_t  n;
    if (v < 10) {
        n = 1; d[0] = (uint8_t)v;
    } else if (v < 100) {
        n = 2; d[0] = v / 10; d[1] = v % 10;
    } else {
        n = 3; d[0] = v / 100; d[1] = (v / 10) % 10; d[2] = v % 10;
    }
    // total_w: 1→9, 2→20, 3→31 — all fit within 32px
    uint8_t total_w = n * 9 + (n - 1) * 2;
    uint8_t x       = box_x + (32 - total_w) / 2;
    for (uint8_t i = 0; i < n; i++) {
        draw_bitmap(x + i * 11, box_y, bmp_digits[d[i]], 9, 15);
    }
}

// ---------------------------------------------------------------------------
// OLED task
// ---------------------------------------------------------------------------

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

static void oledkit_render_info_user(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    led_t   leds = host_keyboard_led_state();

    // Base image (always present)
    draw_bitmap(0, 0, bmp_base, 32, 128);

    // Layer number — bounding box x 0–31, y 13–27
    draw_number(get_highest_layer(layer_state), 0, 13);

    // WPM — bounding box x 0–31, y 43–57
    draw_number(get_current_wpm(), 0, 43);

    // Swap hands — x 0–15, y 60–75
    if (is_swap_hands_on()) draw_bitmap(0, 60, bmp_swaphands, 16, 16);

    // Dynamic macro recording — x 16–31, y 60–75
    if (dynmacro_recording) draw_bitmap(16, 60, bmp_record, 16, 16);

    // Caps Lock — x 0–15, y 78–93
    if (leds.caps_lock) draw_bitmap(0, 78, bmp_capslock, 16, 16);

    // Num Lock — x 16–31, y 78–93
    if (leds.num_lock) draw_bitmap(16, 78, bmp_numlock, 16, 16);

    // Show modifiers for whichever physical half this OLED is on
    bool left = is_keyboard_left();

    // Shift — x 0–15, y 96–111
    if (mods & (left ? MOD_BIT(KC_LSFT) : MOD_BIT(KC_RSFT))) draw_bitmap(0, 96, bmp_shift, 16, 16);

    // Control — x 16–31, y 96–111
    if (mods & (left ? MOD_BIT(KC_LCTL) : MOD_BIT(KC_RCTL))) draw_bitmap(16, 96, bmp_control, 16, 16);

    // Super — x 0–15, y 112–127
    if (mods & (left ? MOD_BIT(KC_LGUI) : MOD_BIT(KC_RGUI))) draw_bitmap(0, 112, bmp_super, 16, 16);

    // Alt — x 16–31, y 112–127
    if (mods & (left ? MOD_BIT(KC_LALT) : MOD_BIT(KC_RALT))) draw_bitmap(16, 112, bmp_alt, 16, 16);
}

bool oled_task_user(void) {
    if (last_input_activity_elapsed() >= 300000UL) {
        oled_off();
        return false;
    }
    oled_on();
    if (is_keyboard_master()) {
        oledkit_render_info_user();
    } else {
        uint8_t mods = get_mods() | get_oneshot_mods();
        bool    left = is_keyboard_left();

        // Base image (always present)
        draw_bitmap(0, 0, bmp_base_slave, 32, 128);

        // Bouncing ball trail — draw oldest segment first so closer ones paint on top
        uint16_t wpm = get_current_wpm();
        uint8_t trailcount = (wpm > 90) ? 3 : (wpm > 60) ? 2 : (wpm > 30) ? 1 : 0;
        for (int8_t i = (int8_t)trailcount - 1; i >= 0; i--) {
            const uint8_t *tmask = bmp_ball_trail_masks[i + (3 - trailcount)][ball_rand8() % 4];
            draw_bitmap_masked(ball_hist_x[i], ball_hist_y[i], bmp_ball_trail, tmask, 8, 8);
        }

        // Bouncing ball — x 0–31, y 0–95
        draw_bitmap_masked(ball_x, ball_y, bmp_ball, bmp_ball_mask, 8, 8);

        // Shift — x 0–15, y 96–111
        if (mods & (left ? MOD_BIT(KC_LSFT) : MOD_BIT(KC_RSFT))) draw_bitmap(0, 96, bmp_shift, 16, 16);

        // Control — x 16–31, y 96–111
        if (mods & (left ? MOD_BIT(KC_LCTL) : MOD_BIT(KC_RCTL))) draw_bitmap(16, 96, bmp_control, 16, 16);

        // Super — x 0–15, y 112–127
        if (mods & (left ? MOD_BIT(KC_LGUI) : MOD_BIT(KC_RGUI))) draw_bitmap(0, 112, bmp_super, 16, 16);

        // Alt — x 16–31, y 112–127
        if (mods & (left ? MOD_BIT(KC_LALT) : MOD_BIT(KC_RALT))) draw_bitmap(16, 112, bmp_alt, 16, 16);
    }
    return false;
}

#endif // OLED_ENABLE
