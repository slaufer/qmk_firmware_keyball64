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

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

static void render_mods(bool left_side) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    bool win, ctl, alt, sft;
    if (left_side) {
        win = mods & MOD_BIT(KC_LGUI);
        ctl = mods & MOD_BIT(KC_LCTL);
        alt = mods & MOD_BIT(KC_LALT);
        sft = mods & MOD_BIT(KC_LSFT);
    } else {
        win = mods & MOD_BIT(KC_RGUI);
        ctl = mods & MOD_BIT(KC_RCTL);
        alt = mods & MOD_BIT(KC_RALT);
        sft = mods & MOD_BIT(KC_RSFT);
    }
    oled_write_P(win ? PSTR("WIN") : PSTR("   "), win);
    oled_write_P(ctl ? PSTR("CTRL") : PSTR("    "), ctl);
    oled_advance_page(true);
    oled_write_P(alt ? PSTR("ALT") : PSTR("   "), alt);
    oled_write_P(sft ? PSTR("SHFT") : PSTR("    "), sft);
    oled_advance_page(true);
}

void oledkit_render_info_user(void) {
    // Layer
    oled_write_P(PSTR("LAYER"), false);
    oled_advance_page(true);
    oled_write(get_u8_str(get_highest_layer(layer_state), ' '), false);
    oled_advance_page(true);

    // Lock indicators
    led_t leds = host_keyboard_led_state();
    oled_write_P(leds.caps_lock ? PSTR("CAPS") : PSTR("    "), leds.caps_lock);
    oled_write_P(leds.num_lock  ? PSTR("NUM")  : PSTR("   "),  leds.num_lock);
    oled_advance_page(true);

    // Padding
    oled_advance_page(true);
    oled_advance_page(true);
    oled_advance_page(true);

    // Modifiers for this half (no blank row between locks and mods)
    render_mods(is_keyboard_left());
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oledkit_render_info_user();
    } else {
        // WPM
        oled_write_P(PSTR("WPM:"), false);
        oled_advance_page(true);
        oled_write(get_u8_str(get_current_wpm(), ' '), false);
        oled_advance_page(true);

        // Padding
        oled_advance_page(true);
        oled_advance_page(true);
        oled_advance_page(true);
        oled_advance_page(true);
        oled_advance_page(true);

        // Modifiers for this half
        render_mods(is_keyboard_left());
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
