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

#include "local_encoder.h"
#include "local_enums.h"
#include "local_init.h"
#include "local_layers.h"
#include "local_oled.h"
#include "local_special_keys.h"

uint8_t current_raise_layer = _RAISE_ES_LA;
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, current_raise_layer, _ADJUST);
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mod_state = get_mods();
    const bool is_pressed = record->event.pressed;
    const uint8_t current_layer_state = get_highest_layer(default_layer_state);
    const led_t led_usb_state = host_keyboard_led_state();

    static uint8_t numpad_caller = _NUMPAD;

    switch (keycode) {
        case LAYER_KC_ES_LA:
        case LAYER_KC_EN_US:
            if (is_pressed) {
                const uint8_t delta = keycode - LAYER_KC_ES_LA;
                set_single_persistent_default_layer(_ES_LA + delta);
                current_raise_layer = _RAISE_ES_LA + delta;
            }
            return false;

        case LAYER_KC_NUMPAD:
            if (is_pressed) {
                numpad_caller = current_layer_state;
                set_single_persistent_default_layer(_NUMPAD);
		if (!led_usb_state.num_lock) {
		    register_code(KC_NUM);
		}
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

        case F_ES_KC_FIRST ... F_ES_KC_LAST:
        case F_CM_KC_FIRST ... F_CM_KC_LAST:
        case F_KS_KC_FIRST ... F_KS_KC_LAST:
            return simulate_es_la_on_en_us_key_stroke(mod_state, keycode, is_pressed);

        case F_SS_KC_FIRST ... F_SS_KC_LAST:
            return process_fake_symbols(mod_state, keycode, is_pressed);

    }
    return true;
}

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    return local_oled_task_user();
}

#endif

void keyboard_post_init_user(void) {
    my_local_status._is_master = is_keyboard_master();
    my_local_status._is_left = is_keyboard_left();

    local_init_oled();
}

