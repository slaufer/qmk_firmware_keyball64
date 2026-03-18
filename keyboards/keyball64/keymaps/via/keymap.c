/*
Copyright 2021 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "transactions.h"
#include <string.h>

// ---------------------------------------------------------------------------
// Held-key tracking
// ---------------------------------------------------------------------------

static uint16_t held_keycodes[5]  = {0};
static char     held_display[6]   = {' ', ' ', ' ', ' ', ' ', '\0'};

static char keycode_to_char(uint16_t kc) {
    if (kc >= KC_A  && kc <= KC_Z)  return 'A' + (kc - KC_A);
    if (kc >= KC_1  && kc <= KC_9)  return '1' + (kc - KC_1);
    if (kc == KC_0)                 return '0';
    return '?';
}

static void update_held_display(void) {
    for (int i = 0; i < 5; i++) {
        held_display[i] = held_keycodes[i] ? keycode_to_char(held_keycodes[i]) : ' ';
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Skip modifiers
    if (keycode >= KC_LCTL && keycode <= KC_RGUI) return true;

    if (record->event.pressed) {
        // Avoid duplicates
        for (int i = 0; i < 5; i++) {
            if (held_keycodes[i] == keycode) return true;
        }
        // Find empty slot; if full, evict oldest (index 0) and shift left
        int slot = -1;
        for (int i = 0; i < 5; i++) {
            if (!held_keycodes[i]) { slot = i; break; }
        }
        if (slot == -1) {
            for (int i = 0; i < 4; i++) held_keycodes[i] = held_keycodes[i + 1];
            slot = 4;
        }
        held_keycodes[slot] = keycode;
    } else {
        for (int i = 0; i < 5; i++) {
            if (held_keycodes[i] == keycode) {
                for (int j = i; j < 4; j++) held_keycodes[j] = held_keycodes[j + 1];
                held_keycodes[4] = 0;
                break;
            }
        }
    }
    update_held_display();
    return true;
}

static void held_keys_slave_handler(uint8_t in_buflen, const void *in_data,
                                    uint8_t out_buflen, void *out_data) {
    if (in_buflen == 5) memcpy(held_display, in_data, 5);
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(HELD_KEYS_SYNC, held_keys_slave_handler);
}

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        transaction_rpc_exec(HELD_KEYS_SYNC, 5, held_display, 0, NULL);
    }
}

// ---------------------------------------------------------------------------
// OLED
// ---------------------------------------------------------------------------

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

void oledkit_render_info_user(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    bool    left = is_keyboard_left();
    bool    sft  = mods & (left ? MOD_BIT(KC_LSFT) : MOD_BIT(KC_RSFT));
    bool    ctl  = mods & (left ? MOD_BIT(KC_LCTL) : MOD_BIT(KC_RCTL));
    bool    gui  = mods & (left ? MOD_BIT(KC_LGUI) : MOD_BIT(KC_RGUI));
    bool    alt  = mods & (left ? MOD_BIT(KC_LALT) : MOD_BIT(KC_RALT));
    led_t   leds = host_keyboard_led_state();

    oled_write_P(PSTR("LAYER"), false);                                             // row  0
    oled_write_P(PSTR("  "), false);                                                // row  1
    oled_write(get_u8_str(get_highest_layer(layer_state), ' '), false);
    oled_write_P(PSTR("     "), false);                                             // row  2
    oled_write_P(leds.caps_lock ? PSTR("CAPSK") : PSTR("     "), leds.caps_lock);  // row  3
    oled_write_P(leds.num_lock  ? PSTR("NUMLK") : PSTR("     "), leds.num_lock);   // row  4
    oled_write_P(PSTR("     "), false);                                             // row  5
    oled_write_P(PSTR("     "), false);                                             // row  6
    oled_write_P(PSTR("     "), false);                                             // row  7
    oled_write_P(PSTR("     "), false);                                             // row  8
    oled_write_P(PSTR("     "), false);                                             // row  9
    oled_write_P(PSTR("     "), false);                                             // row 10
    oled_write_P(PSTR("     "), false);                                             // row 11
    oled_write_P(sft ? PSTR("SHIFT") : PSTR("     "), sft);                        // row 12
    oled_write_P(ctl ? PSTR(" CTRL") : PSTR("     "), ctl);                        // row 13
    oled_write_P(gui ? PSTR("SUPER") : PSTR("     "), gui);                        // row 14
    oled_write_P(alt ? PSTR(" ALT ") : PSTR("     "), alt);                        // row 15
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oledkit_render_info_user();
    } else {
        uint8_t mods = get_mods() | get_oneshot_mods();
        bool    left = is_keyboard_left();
        bool    sft  = mods & (left ? MOD_BIT(KC_LSFT) : MOD_BIT(KC_RSFT));
        bool    ctl  = mods & (left ? MOD_BIT(KC_LCTL) : MOD_BIT(KC_RCTL));
        bool    gui  = mods & (left ? MOD_BIT(KC_LGUI) : MOD_BIT(KC_RGUI));
        bool    alt  = mods & (left ? MOD_BIT(KC_LALT) : MOD_BIT(KC_RALT));

        oled_write_P(PSTR(" WPM "), false);                                         // row  0
        oled_write_P(PSTR("  "), false);                                            // row  1
        oled_write(get_u8_str(get_current_wpm(), ' '), false);
        oled_write_P(PSTR("     "), false);                                         // row  2
        oled_write(held_display, false);                                            // row  3
        oled_write_P(PSTR("     "), false);                                         // row  4
        oled_write_P(PSTR("     "), false);                                         // row  5
        oled_write_P(PSTR("     "), false);                                         // row  6
        oled_write_P(PSTR("     "), false);                                         // row  7
        oled_write_P(PSTR("     "), false);                                         // row  8
        oled_write_P(PSTR("     "), false);                                         // row  9
        oled_write_P(PSTR("     "), false);                                         // row 10
        oled_write_P(PSTR("     "), false);                                         // row 11
        oled_write_P(sft ? PSTR("SHIFT") : PSTR("     "), sft);                    // row 12
        oled_write_P(ctl ? PSTR("CTRL ") : PSTR("     "), ctl);                    // row 13
        oled_write_P(gui ? PSTR("SUPER") : PSTR("     "), gui);                    // row 14
        oled_write_P(alt ? PSTR(" ALT ") : PSTR("     "), alt);                    // row 15
    }
    return false;
}

#endif // OLED_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Layer 0: Base QWERTY
    //
    // Row 0 (12): Esc  1    2    3    4    5  |  6    7    8    9    0    Bksp
    // Row 1 (12): Tab  Q    W    E    R    T  |  Y    U    I    O    P    Del
    // Row 2 (12): Ctrl A    S    D    F    G  |  H    J    K    L    ;    '
    // Row 3 (14): Shft Z    X    C    V    B  [  |  ]  N    M    ,    .    /    Shft
    // Row 4 (14): Ctrl Alt  Del  GUI  Spc  _  _  |  _  _   Ent  Bksp GUI  RAlt RCtl
    //             (L40 L41  L42  L43  L44  L45 L46  R46 R45 R44  R43  R42  R41  R40)
    [0] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                          KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                          KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LBRC, KC_RBRC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_LCTL, KC_LALT, KC_DEL,  KC_LGUI, KC_SPC,  KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_ENT,  KC_BSPC, KC_RGUI, KC_RALT, KC_RCTL
    ),

    // Layers 1-3: transparent (for VIA remapping)
    [1] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [2] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [3] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
};
// clang-format on
