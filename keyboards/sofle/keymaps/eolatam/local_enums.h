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

// Fake es-la key keycodes.
// If OS keymap is EN-US, these fake keycodes are mapped to keycodes that mimic the ES-LA keymap.
// These keycodes make sense only in the _EN_US keymap.
enum fake_es_keycodes {
    F_ES_KC_FIRST = LAYER_KC_POST_LAST,
    F_ES_0 = F_ES_KC_FIRST,
    F_ES_1,
    F_ES_2,
    F_ES_3,
    F_ES_4,
    F_ES_5,
    F_ES_6,
    F_ES_7,
    F_ES_8,
    F_ES_9,
    F_ES_COMM,
    F_ES_DOT,
    F_ES_Q,

    F_ES_KC_POST_LAST,
    F_ES_KC_LAST = F_ES_KC_POST_LAST - 1
};

// Fake keycodes, independent of the layer.
enum fake_common_keycodes {
    F_CM_KC_FIRST = F_ES_KC_POST_LAST,
    F_CM_BSPS = F_CM_KC_FIRST,
    F_CM_LEFT,
    F_CM_RGHT,

    F_CM_KC_POST_LAST,
    F_CM_KC_LAST = F_CM_KC_POST_LAST - 1
};

// Fake keycodes, keymap specific.
// If in layer _EN_US, will issue a symbol consistent with an ES-LA keymap.
enum fake_keymap_specific_keycodes {
    F_KS_KC_FIRST = F_CM_KC_POST_LAST,
    F_KS_LT = F_KS_KC_FIRST,
    F_KS_QUOT,
    F_KS_ACUT,
    F_KS_PLUS,
    F_KS_LCUB,
    F_KS_RCUB,

    F_KS_KC_POST_LAST,
    F_KS_KC_LAST = F_KS_KC_POST_LAST - 1
};

#define FAKE_KC_COUNT (F_KS_KC_POST_LAST - F_ES_KC_FIRST)

// Fake keycodes, issuing a symbols from alt sequences with send_string.
enum fake_senstring_keycodes {
    F_SS_KC_FIRST = F_KS_KC_POST_LAST,
    F_SS_NTILDE = F_SS_KC_FIRST,
    F_SS_INV_QUESTION,
    F_SS_BAR,

    F_SS_KC_POST_LAST,
    F_SS_KC_LAST = F_SS_KC_POST_LAST - 1,
};

// clang-format on
