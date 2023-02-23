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

//#if defined(ENCODER_RESOLUTION)
//#undef ENCODER_RESOLUTION
//#define ENCODER_RESOLUTION 4
//#endif

#ifdef TAPPING_TERM
#undef TAPPING_TERM
#define TAPPING_TERM 250
#endif

#define OLED_TIMEOUT 120000
#define OLED_BRIGHTNESS 120


// space cadet configuration
#if defined(LSPO_KEYS) || defined(RSPC_KEYS) || defined(LCPO_KEYS) || defined(RCPC_KEYS) || defined(LAPO_KEYS) || defined(RAPC_KEYS)
  #error "neither of these keys must be defined yet"
#else
  // shift keys: ()
  #define LSPO_KEYS KC_LEFT_SHIFT,  KC_LEFT_SHIFT,  KC_9
  #define RSPC_KEYS KC_RIGHT_SHIFT, KC_RIGHT_SHIFT, KC_0

  // control keys: []
  #define LCPO_KEYS KC_LEFT_CTRL,  KC_TRANSPARENT, KC_LBRC
  #define RCPC_KEYS KC_RIGHT_CTRL, KC_TRANSPARENT, KC_RBRC

  // alt keys: {}
  #define LAPO_KEYS KC_LEFT_ALT,  KC_LEFT_SHIFT,  KC_LBRC
  #define RAPC_KEYS KC_RIGHT_ALT, KC_RIGHT_SHIFT, KC_RBRC
#endif
