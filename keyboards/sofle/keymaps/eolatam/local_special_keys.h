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

 // This file provides data and functions needed to allow keys to have a
 // behavior different than the expected when shift is pressed.
 // The main purpose of this is to allow numeric keys to emit the latin
 // keymap symbols when shifted.

#pragma once

enum potential_modifiers {
    PMOD_SHIFT,
    PMOD_ALTGR,

    PMOD_COUNT
};

bool process_potential_modification(const uint8_t mod_state,
                                    const uint16_t keycode,
                                    const bool is_pressed);

bool process_fake_symbols(const uint8_t mod_state,
                          const uint16_t keycode,
                          const bool is_pressed);

bool simulate_es_la_on_en_us_key_stroke(const uint8_t mod_state,
                                    	const uint16_t keycode,
                                    	const bool is_pressed);

