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

#include "local_special_keys.h"
#include "local_layers.h"

uint16_t get_actual_keycode(const uint8_t index, const uint8_t column) {
    static const uint16_t fake_keys_results[FAKE_KC_COUNT][3] = {
        // FAKE_KEY                 UNMOD       SHIFT           ALTGR
        /* FAKE_0            */ {   KC_0,       KC_EQUAL,       KC_TRNS     },
        /* FAKE_1            */ {   KC_1,       KC_1 /**/,      KC_TRNS     },
        /* FAKE_2            */ {   KC_2,       KC_QUOTE /**/,  KC_2 /**/   },
        /* FAKE_3            */ {   KC_3,       KC_3 /**/,      KC_TRNS     },

        /* FAKE_4            */ {   KC_4,       KC_4 /**/,      KC_TRNS     },
        /* FAKE_5            */ {   KC_5,       KC_5 /**/,      KC_TRNS     },
        /* FAKE_6            */ {   KC_6,       KC_7 /**/,      KC_TRNS     },
        /* FAKE_7            */ {   KC_7,       KC_SLASH,       KC_TRNS     },

        /* FAKE_8            */ {   KC_8,       KC_9 /**/,      KC_TRNS     },
        /* FAKE_9            */ {   KC_9,       KC_0 /**/,      KC_TRNS     },
        /* FAKE_COMM         */ {   KC_COMM,    KC_SCLN,        KC_TRNS     },
        /* FAKE_DOT          */ {   KC_DOT,     KC_SCLN /**/,   KC_TRNS     },

        /* FAKE_Q            */ {   KC_Q,       KC_Q /**/,      KC_2 /**/   },
        /* FAKE_BSPC         */ {   KC_BSPC,    KC_DEL,         KC_TRNS     },
        /* FAKE_QUOT         */ {   KC_QUOT,    KC_SLASH /**/,  KC_BSLS     },
    };

    return fake_keys_results[index][column];
}

bool get_fake_key_shift_status(const uint8_t index, const uint8_t column) {
    static const uint16_t fake_keys_shift_status[3] = {
        /* UNMOD */ 0b000000000000000,
        /* SHIFT */ 0b101101101111110,
        /* ALTGR */ 0b001000000000100,
    };

    return (fake_keys_shift_status[column] & (1 << index)) ? true : false;
}

uint8_t get_index_for_fake_keycode(const uint16_t keycode) {
    return keycode - FAKE_KC_FIRST;
}

// Will implement a different behavior for the given keycode when a modifier
// is pressed, according to the data above.
// This function is strongly based on the procedure to emit KC_DEL when
// shift + KC_BSPS is pressed in the documentation.
// https://docs.qmk.fm/#/feature_advanced_keycodes?id=shift-backspace-for-delete
bool process_potential_modification_2(const uint8_t mod_state,
                                    const uint16_t keycode,
                                    const bool is_pressed) {
    // Initialize a boolean variable that keeps track
    // of the shifted status of each candidate key.
    static uint16_t registered_shifts;

    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;

    if (is_altgr && is_shift) {
        // This is not allowed for these keys.
        return false;
    }

    const uint8_t index = get_index_for_fake_keycode(keycode);
    const uint8_t column = is_altgr ? 2 : (is_shift ? 1 : 0);
    const uint16_t actual_keycode = get_actual_keycode(index, column);
    // const uint16_t actual_keycode = fake_keys_results[index][column];

    if (actual_keycode == KC_TRNS) {
        // Nothing to do.
        return false;
    }

    const uint16_t bit_index = (1 << index);

    if (!is_pressed) {
        oled_set_cursor(0, 14);
        oled_write("ooooo", true);

        // On release of keycode, in case the altered keycode is still being sent.
        unregister_code(actual_keycode);
        if (registered_shifts & bit_index) {
            registered_shifts &= ~bit_index;
            return false;
        }

        return true;
    }

    const bool needs_shift = get_fake_key_shift_status(index, column);
    const uint16_t all_masks = MOD_MASK_SHIFT | MOD_BIT(KC_RALT);

    if (is_shift) {
        if (needs_shift) {
            register_code(actual_keycode);
            return true;
        }

        del_mods(all_masks);
        register_code(actual_keycode);
        set_mods(mod_state);
        return false;
    }

    if (is_altgr) {
        del_mods(all_masks);
        if (needs_shift) {
            set_mods(MOD_BIT(KC_LSFT));
            registered_shifts |= bit_index;
        }
        register_code(actual_keycode);
        set_mods(mod_state);
        return false;
    }

    if (needs_shift) {
        set_mods(MOD_BIT(KC_LSFT));
        registered_shifts |= bit_index;

        register_code(actual_keycode);

        set_mods(mod_state);
        return false;
    }

    register_code(actual_keycode);

    // Just press the key.
    return true;
}

bool process_fake_symbols(const uint8_t mod_state, const uint16_t keycode, const bool is_pressed) {
    if (!is_pressed) {
        return true;
    }

    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    const uint16_t all_masks = (MOD_MASK_SHIFT | MOD_BIT(KC_RALT));
    del_mods(all_masks);

    switch (keycode) {
        case SEQ_NTILDE:
            if (!is_altgr) {
                if (is_shift) {
                    SEND_STRING(SS_RALT(SS_TAP(X_KP_1)SS_TAP(X_KP_6)SS_TAP(X_KP_5)));
                } else {
                    SEND_STRING(SS_RALT(SS_TAP(X_KP_1)SS_TAP(X_KP_6)SS_TAP(X_KP_4)));
                }
            }
            break;

        case SEQ_INV_QUESTION:
            if (!is_altgr) {
                if (is_shift) {
                    SEND_STRING(SS_RALT(SS_TAP(X_KP_1)SS_TAP(X_KP_7)SS_TAP(X_KP_3)));
                } else {
                    SEND_STRING(SS_RALT(SS_TAP(X_KP_1)SS_TAP(X_KP_6)SS_TAP(X_KP_8)));
                }
            }
            break;

        default:
            break;
    }
    set_mods(mod_state);
    return false;
}
