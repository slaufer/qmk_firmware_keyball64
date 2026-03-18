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
