// Shared keymap logic included by both default and via keymaps.
// Each keymap compiles independently, so function definitions here are safe.

#include QMK_KEYBOARD_H
#include "transactions.h"
#include "lib/oled_bitmap/oled_bitmap.h"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= KC_LCTL && keycode <= KC_RGUI) return true;
    if (record->event.pressed) oled_ball_on_key_press();
    return true;
}

static void ball_pos_slave_handler(uint8_t in_buflen, const void *in_data,
                                   uint8_t out_buflen, void *out_data) {
    if (in_buflen == 8) {
        const uint8_t *d = (const uint8_t *)in_data;
        oled_ball_set_pos(d[0], d[1]);
        uint8_t hx[3] = {d[2], d[4], d[6]};
        uint8_t hy[3] = {d[3], d[5], d[7]};
        oled_ball_set_history(hx, hy);
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(BALL_POS_SYNC, ball_pos_slave_handler);
}

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        uint8_t hx[3], hy[3];
        oled_ball_get_history(hx, hy);
        uint8_t ball_buf[8];
        oled_ball_get_pos(&ball_buf[0], &ball_buf[1]);
        ball_buf[2] = hx[0]; ball_buf[3] = hy[0];
        ball_buf[4] = hx[1]; ball_buf[5] = hy[1];
        ball_buf[6] = hx[2]; ball_buf[7] = hy[2];
        transaction_rpc_exec(BALL_POS_SYNC, 8, ball_buf, 0, NULL);
    }
}
