// Shared keymap logic included by both default and via keymaps.
// Each keymap compiles independently, so function definitions here are safe.

#include QMK_KEYBOARD_H
#include "lib/oled_bitmap/oled_bitmap.h"

void housekeeping_task_user(void) {
    if (!is_keyboard_master()) {
        static uint16_t ball_timer = 0;
        uint8_t wpm = get_current_wpm();
        if (wpm > 0) {
            uint16_t interval = (wpm >= 90) ? 50 : 500 - wpm * 5;
            if (timer_elapsed(ball_timer) >= interval) {
                oled_ball_tick();
                ball_timer = timer_read();
            }
        }
    }
}
