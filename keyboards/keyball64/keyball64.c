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

// clang-format off
#ifdef SWAP_HANDS_ENABLE
// Mirror left (rows 0-4) <-> right (rows 5-9) at the same column.
// Unused positions (col 3, col 7 on rows 0-2/5-7) map to themselves.
// keypos_t is {col, row} — col first, row second.
// Each entry {c, r} means: remap this key to col=c, row=r.
const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    {{0,5},{1,5},{2,5},{3,5},{4,5},{5,5},{6,5},{7,5}},  // row 0 (left)  -> row 5 (right)
    {{0,6},{1,6},{2,6},{3,6},{4,6},{5,6},{6,6},{7,6}},  // row 1 (left)  -> row 6 (right)
    {{0,7},{1,7},{2,7},{3,7},{4,7},{5,7},{6,7},{7,7}},  // row 2 (left)  -> row 7 (right)
    {{0,8},{1,8},{2,8},{3,8},{4,8},{5,8},{6,8},{7,8}},  // row 3 (left)  -> row 8 (right)
    {{0,9},{1,9},{2,9},{3,9},{4,9},{5,9},{6,9},{7,9}},  // row 4 (left)  -> row 9 (right)
    {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0}},  // row 5 (right) -> row 0 (left)
    {{0,1},{1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1}},  // row 6 (right) -> row 1 (left)
    {{0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{6,2},{7,2}},  // row 7 (right) -> row 2 (left)
    {{0,3},{1,3},{2,3},{3,3},{4,3},{5,3},{6,3},{7,3}},  // row 8 (right) -> row 3 (left)
    {{0,4},{1,4},{2,4},{3,4},{4,4},{5,4},{6,4},{7,4}},  // row 9 (right) -> row 4 (left)
};
#endif // SWAP_HANDS_ENABLE

matrix_row_t matrix_mask[MATRIX_ROWS] = {
    0b01110111,  // row 0 (left):  col 3 and col 7 masked (no switch pads)
    0b01110111,  // row 1 (left):  col 3 and col 7 masked
    0b01110111,  // row 2 (left):  col 3 and col 7 masked
    0b11110111,  // row 3 (left):  col 3 masked only
    0b11110111,  // row 4 (left):  col 3 masked only
    0b01110111,  // row 5 (right): col 3 and col 7 masked
    0b01110111,  // row 6 (right): col 3 and col 7 masked
    0b01110111,  // row 7 (right): col 3 and col 7 masked
    0b11110111,  // row 8 (right): col 3 masked only
    0b11110111,  // row 9 (right): col 3 masked only
};
// clang-format on
