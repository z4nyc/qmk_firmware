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

#include "local_general.h"

#ifdef ENCODER_ENABLE

void move_encoder0(const bool clockwise) {
    const uint16_t lnum = get_highest_layer(layer_state);
    if (lnum == _RAISE_ES_LA || lnum == _RAISE_EN_US) {
        // mouse horizontal wheel.
        if (clockwise) {
            tap_code(KC_WH_L);
        } else {
            tap_code(KC_WH_R);
        }
        return;
    }

    // brightness
    if (clockwise) {
        tap_code(KC_BRIU);
    } else {
        tap_code(KC_BRID);
    }
}

void move_encoder1(const bool clockwise) {
    const uint16_t mods = get_mods();
    const bool control_is_pressed = (mods & MOD_MASK_CTRL) ? true : false;
    const bool shift_is_pressed = (mods & MOD_MASK_SHIFT) ? true : false;
    const bool alt_is_pressed = (mods & MOD_MASK_ALT) ? true : false;
    const bool gui_is_pressed = (mods & MOD_MASK_GUI) ? true : false;
    if (gui_is_pressed || (alt_is_pressed == control_is_pressed)) {
        // volume
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
        return;
    }

    // alt+tab or ctrl+tab
    if (clockwise || shift_is_pressed) {
        tap_code(KC_TAB);
        return;
    }

    register_code(KC_LSFT);
    tap_code(KC_TAB);
    unregister_code(KC_LSFT);
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        move_encoder0(clockwise);
    } else {
        move_encoder1(clockwise);
    }

    return true;
}

#endif
