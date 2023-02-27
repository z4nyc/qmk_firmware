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

#include "local_layers.h"
#include "local_special_keys.h"
#include "local_encoder.h"
#include "local_oled.h"

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mod_state = get_mods();
    const bool is_pressed = record->event.pressed;
    static uint8_t numpad_caller = _NUMPAD;
    const uint8_t current_layer_state = get_highest_layer(default_layer_state);

    switch (keycode) {
        // case LAYER_KC_ES_LA:
        case LAYER_KC_EN_US:
            if (is_pressed) {
                set_single_persistent_default_layer(keycode - LAYER_KC_FIRST);
            }
            return false;

        case LAYER_KC_NUMPAD:
            if (is_pressed) {
                numpad_caller = current_layer_state;
                set_single_persistent_default_layer(_NUMPAD);
                register_code(KC_NUM);
            } else {
                unregister_code(KC_NUM);
            }
            return false;

        case LAYER_KC_NUMPAD_CALLER:
            if (is_pressed) {
                set_single_persistent_default_layer(numpad_caller);
                register_code(KC_NUM);
            } else {
                unregister_code(KC_NUM);
            }
            return false;

#if 0
        case F_ES_KC_FIRST ... F_ES_KC_LAST:
        case F_CM_KC_FIRST ... F_CM_KC_LAST:
            return process_potential_modification(mod_state, keycode, is_pressed);

        case F_KS_KC_FIRST ... F_KS_ACUT:
            return process_potential_modification_2(mod_state, keycode, is_pressed);

        case F_KS_PLUS ... F_KS_RCUB:
            if (current_layer_state == _ES_LA) {
                return process_potential_modification_3(mod_state, keycode, is_pressed);
            } else {
                return process_potential_modification(mod_state, keycode, is_pressed);
            }
#endif
        case F_ES_KC_FIRST ... F_ES_KC_LAST:
        case F_CM_KC_FIRST ... F_CM_KC_LAST:
        case F_KS_KC_FIRST ... F_KS_KC_LAST:
            return process_potential_modification(mod_state, keycode, is_pressed);

        case F_SS_KC_FIRST ... F_SS_KC_LAST:
            return process_fake_symbols(mod_state, keycode, is_pressed);

        case LAYER_KC_ADJUST:
            if (is_pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;

        case KC_SPC:
            if (is_pressed) {
                isJumping  = true;
                showedJump = false;
            } else {
                isJumping = false;
            }
            break;

    }
    return true;
}
