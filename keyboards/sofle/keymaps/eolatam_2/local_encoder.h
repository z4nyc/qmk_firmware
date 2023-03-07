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

#include "local_enums.h"

#ifdef ENCODER_ENABLE

const uint16_t get_encoder0_keycode(const bool clockwise) {
    const uint16_t lnum = get_highest_layer(layer_state);
    if (lnum == _RAISE_ES_LA || lnum == _RAISE_EN_US) {
        // mouse horizontal wheel.
        return clockwise ? KC_WH_L : KC_WH_R;
    }

    return clockwise ? KC_BRIU : KC_BRID;
}

const uint16_t get_encoder1_keycode(const bool clockwise) {
    return clockwise ? KC_VOLU : KC_VOLD;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    const uint16_t k = (index == 0) ? get_encoder0_keycode(clockwise) :
                                      get_encoder1_keycode(clockwise);
    tap_code(k);

    return true;
}

#endif
