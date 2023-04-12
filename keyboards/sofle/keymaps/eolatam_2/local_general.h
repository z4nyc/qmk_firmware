/* Copyright 2023 eossesa
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

#pragma once

// Layer IDs.
enum sofle_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _ES_LA, // std layer
    _EN_US,
    _NUMPAD,
    _LOWER,
    _RAISE_ES_LA,
    _RAISE_EN_US,
    _ADJUST,
};

// Keycodes associated with layers, mapped 1:1 to values in sofle layers.
enum layer_keycodes {
    LAYER_KC_FIRST = SAFE_RANGE,
    LAYER_KC_ES_LA = LAYER_KC_FIRST,
    LAYER_KC_EN_US,
    LAYER_KC_NUMPAD,
    LAYER_KC_LOWER,
    LAYER_KC_RAISE_ES_LA,
    LAYER_KC_RAISE_EN_US,
    LAYER_KC_ADJUST,

    // Special layer keycode, if used will return to the layer before the numpad.
    LAYER_KC_NUMPAD_CALLER,

    LAYER_KC_POST_LAST,
    LAYER_KC_LAST = LAYER_KC_POST_LAST - 1,
};

// UUSST: Unshifted keycode translates to an unshifted keycode, shifted keycode translates to a different shifted keycode.
enum {
    UUSST_KC_FIRST = LAYER_KC_POST_LAST,
    UUSST_KC_2 = UUSST_KC_FIRST,
    UUSST_KC_6,
    UUSST_KC_8,
    UUSST_KC_9,
    UUSST_KC_QUOT,
    UUSST_KC_DOT,
    UUSST_KC_NTIL,
    UUSST_KC_Q,
    UUSST_KC_ACUT,

    UUSST_KC_POST_LAST,
    UUSST_KC_LAST = UUSST_KC_POST_LAST - 1,
};

#define UUSST_KC_COUNT (UUSST_KC_POST_LAST - UUSST_KC_FIRST)

// UUSUT: Unshifted keycode translates to an unshifted keycode, shifted keycode translates to a different unshifted keycode.
enum {
    UUSUT_KC_FIRST = UUSST_KC_POST_LAST,
    UUSUT_KC_7 = UUSUT_KC_FIRST,
    UUSUT_KC_0,
    UUSUT_KC_COMM,
    UUSUT_KC_BSDL,

    UUSUT_KC_POST_LAST,
    UUSUT_KC_LAST = UUSUT_KC_POST_LAST - 1,
};

#define UUSUT_KC_COUNT (UUSUT_KC_POST_LAST - UUSUT_KC_FIRST)

// USSST: Unshifted keycode translates to a shifted keycode, shifted keycode translates to a different shifted keycode.
enum {
    USSST_KC_FIRST = UUSUT_KC_POST_LAST,
    USSST_KC_LABK = USSST_KC_FIRST,
    USSST_KC_PLUS,
    USSST_KC_IQUE,

    USSST_KC_POST_LAST,
    USSST_KC_LAST = USSST_KC_POST_LAST - 1,
};

#define USSST_KC_COUNT (USSST_KC_POST_LAST - USSST_KC_FIRST)

// USSUT: Unshifted keycode translates to a shifted keycode, shifted keycode translates to a different unshifted keycode.
enum {
    USSUT_KC_FIRST = USSST_KC_POST_LAST,
    USSUT_KC_LCBR = USSUT_KC_FIRST,
    USSUT_KC_RCBR,
    USSUT_KC_PIPE,

    USSUT_KC_POST_LAST,
    USSUT_KC_LAST = USSUT_KC_POST_LAST - 1,
};

#define USSUT_KC_COUNT  (USSUT_KC_POST_LAST - USSUT_KC_FIRST)

#define UST_KC_FIRST    UUSST_KC_FIRST
#define UST_KC_LAST     USSUT_KC_LAST
#define UST_KC_COUNT    (1 + UST_KC_LAST - UST_KC_FIRST)


extern uint8_t current_raise_layer;
void local_init_raise_layer(void);

// useful macros

#define REP_1(x)    x
#define REP_2(x)    REP_1(x), REP_1(x)
#define REP_3(x)    REP_2(x), REP_1(x)
#define REP_4(x)    REP_3(x), REP_1(x)
#define REP_5(x)    REP_4(x), REP_1(x)
#define REP_6(x)    REP_5(x), REP_1(x)
#define REP_7(x)    REP_6(x), REP_1(x)
#define REP_8(x)    REP_7(x), REP_1(x)
#define REP_9(x)    REP_8(x), REP_1(x)

#define REP_CAT(a, ...)   a ## __VA_ARGS__
#define REP(n, x)   REP_CAT(REP_ , n) (x)

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#define EOA_FOR_RANGE(type, name, first, last)    for (type name = first; name <= last; ++name)

#define EOA_BIT_POS(type, size, val, arr_i, bit_i) \
    const type arr_i = val / size; \
    const type bit_i = val % size;

#define EOA_GET_UST_KC_INDEX(keycode) ((keycode) - UST_KC_FIRST)

// clang-format on
