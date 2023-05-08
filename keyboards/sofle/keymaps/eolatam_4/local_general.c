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

#include QMK_KEYBOARD_H

#include "local_general.h"

uint8_t current_raise_layer = _RAISE_ES_LA;

void local_init_raise_layer(void) {
    uint8_t current_layer = get_highest_layer(default_layer_state);
    if (current_layer != _ES_LA && current_layer != _EN_US) {
        set_single_persistent_default_layer(_ES_LA);
        current_layer = _ES_LA;
    }
    current_raise_layer = current_layer + 1;
}

