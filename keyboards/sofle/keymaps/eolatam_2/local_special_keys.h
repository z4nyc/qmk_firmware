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

void mark_shift_pressed(const uint16_t keycode,
                        const bool is_pressed);

void ust_show_flags(const char* const msg);

bool uusst_kc_operate_on_keycode(const uint8_t mod_state,
                                 const uint16_t keycode,
                                 const bool is_pressed);

bool uusst_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed);

bool uusut_kc_operate_on_keycode(const uint8_t mod_state,
                                 const uint16_t keycode,
                                 const bool is_pressed);

bool uusut_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed);

bool ussst_kc_operate_on_keycode(const uint8_t mod_state,
                                 const uint16_t keycode,
                                 const bool is_pressed);

bool ussst_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed);

bool ussut_kc_operate_on_keycode(const uint8_t mod_state,
                                 const uint16_t keycode,
                                 const bool is_pressed);

bool ussut_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed);

