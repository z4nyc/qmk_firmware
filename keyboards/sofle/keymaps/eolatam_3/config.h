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

#if defined(ENCODER_RESOLUTION)
#undef ENCODER_RESOLUTION
#define ENCODER_RESOLUTION 4
#endif

//#ifdef TAPPING_TERM
//#undef TAPPING_TERM
//#define TAPPING_TERM 250
//#endif

// handness controlled by eeprom. to facilitate uploading firmware on both halves.
// flashing commands:
//   - qmk flash -kb sofle/rev1 -km eossesa -bl avrdude-split-left
//   - qmk flash -kb sofle/rev1 -km eossesa -bl avrdude-split-right
//#define EE_HANDS

#define OLED_TIMEOUT 120000
#define OLED_BRIGHTNESS 120

#define SPLIT_LED_STATE_ENABLE
#define SPLIT_LAYER_STATE_ENABLE

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
#define NO_ACTION_ONESHOT

