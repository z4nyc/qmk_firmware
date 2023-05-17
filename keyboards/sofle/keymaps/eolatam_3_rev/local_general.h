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
    _FIRST_LAYER,
    _ES_LA = _FIRST_LAYER,
    _RAISE_ES_LA,
    _EN_US,
    _RAISE_EN_US,
    _LOWER,
    _NUMPAD,
    _ADJUST,
};

// Keycodes associated with layers, mapped 1:1 to values in sofle layers.
enum layer_keycodes {
    LAYER_KC_FIRST = SAFE_RANGE,
    LAYER_KC_ES_LA = LAYER_KC_FIRST,
    LAYER_KC_RAISE_ES_LA,
    LAYER_KC_EN_US,
    LAYER_KC_RAISE_EN_US,
    LAYER_KC_LOWER,
    LAYER_KC_NUMPAD,
    LAYER_KC_ADJUST,

    // Special layer keycode, if used will return to the layer before the numpad.
    LAYER_KC_NUMPAD_CALLER,

    LAYER_KC_POST_LAST,
};

enum {
    AB_KC_FIRST = LAYER_KC_POST_LAST,
    AB_ACUT = AB_KC_FIRST,

    BA_LCBR,
    BA_RCBR,
    BA_PIPE,

    BB_LABK,
    BB_IQUE,
    BB_PLUS,

    AB_KC_POST_LAST
};

#define AB_KC_COUNT (AB_KC_POST_LAST - AB_KC_FIRST)

extern uint8_t current_raise_layer;
void local_init_raise_layer(void);

// clang-format on
