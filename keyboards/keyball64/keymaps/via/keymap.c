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

#include "../../lib/keymap_common/keymap_common.h"


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Layer 0: Base
    //
    // Row 0 (12): 1    2    3    4    5    6   |  7    8    9    0    -    =
    // Row 1 (12): Tab  Q    W    E    R    T   |  Y    U    I    O    P    Bksp
    // Row 2 (12): Esc  A    S    D    F    G   |  H    J    K    L    ;    '
    // Row 3 (14): Shft Z    X    C    V    B  SH |SH  N    M    ,    .    /    Ent
    // Row 4 (14): Ctrl GUI  GUI  Alt  MO1  Spc MO2|MO3 Spc  MO1  ←    ↓    ↑    →
    //             (L40 L41  L42  L43  L44  L45 L46  R46 R45  R44  R43  R42  R41  R40)
    [0] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                                          KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                          KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    SH_TOGG, SH_TOGG,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
        KC_LCTL, KC_LGUI, KC_LGUI, KC_LALT, MO(1),   KC_SPC,  MO(2),   MO(3),    KC_SPC,  MO(1),   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),

    // Layer 1: Navigation / F-keys / Media
    [1] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                                         KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        KC_GRV,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_VOLU,                                        KC_TRNS, KC_TRNS, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,
        KC_CAPS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_VOLD,                                        KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_INS,
        KC_TRNS, KC_MPRV, KC_MPLY, KC_MSTP, KC_MNXT, KC_MUTE, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_RSFT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_RALT, KC_APP,  KC_RGUI, KC_RCTL
    ),

    // Layer 2: Window management / One-shot mods / Dynamic macro play
    [2] = LAYOUT(
        DM_PLY1, DM_PLY2, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, G(KC_LBRC), C(KC_PGUP), C(KC_PGDN), G(KC_RBRC), G(KC_T),                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, G(KC_LEFT), G(KC_DOWN), G(KC_UP), G(KC_RGHT), KC_TRNS,                               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        OSM(MOD_LSFT), A(KC_LEFT), A(KC_DOWN), A(KC_UP), A(KC_RGHT), KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, OSM(MOD_LSFT|MOD_RSFT),
        OSM(MOD_LCTL), OSM(MOD_LGUI), OSM(MOD_LGUI), OSM(MOD_LALT), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, OSM(MOD_LALT|MOD_RALT), KC_TRNS, OSM(MOD_LGUI|MOD_RGUI), OSM(MOD_LCTL|MOD_RCTL)
    ),

    // Layer 3: Dynamic macro record / EEPROM reset / One-shot mods
    [3] = LAYOUT(
        DM_REC1, DM_REC2, DM_RSTP, KC_TRNS, KC_TRNS, KC_TRNS,                                        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_CLEAR_EEPROM,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        OSM(MOD_LSFT), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, OSM(MOD_LSFT|MOD_RSFT),
        OSM(MOD_LCTL), OSM(MOD_LGUI), OSM(MOD_LGUI), OSM(MOD_LALT), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, OSM(MOD_LALT|MOD_RALT), KC_TRNS, OSM(MOD_LGUI|MOD_RGUI), OSM(MOD_LCTL|MOD_RCTL)
    ),
};
// clang-format on
