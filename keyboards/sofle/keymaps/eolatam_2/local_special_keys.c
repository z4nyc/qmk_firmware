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
#include "local_enums.h"

uint16_t get_fake_en_us_simulating_es_la(const uint8_t index, const uint8_t column) {
    static const uint16_t key_index_to_keycode_set[FAKE_KC_COUNT][3] = {
        // FAKE KEYCODE             UNMOD           SHIFT           ALTGR
        /* F_ES_0            */ {   KC_0,           KC_EQUAL,       KC_TRNS     },
        /* F_ES_1            */ {   KC_1,           KC_1 /**/,      KC_TRNS     },
        /* F_ES_2            */ {   KC_2,           KC_QUOTE /**/,  KC_2 /**/   },
        /* F_ES_3            */ {   KC_3,           KC_3 /**/,      KC_TRNS     },

        /* F_ES_4            */ {   KC_4,           KC_4 /**/,      KC_TRNS     },
        /* F_ES_5            */ {   KC_5,           KC_5 /**/,      KC_TRNS     },
        /* F_ES_6            */ {   KC_6,           KC_7 /**/,      KC_TRNS     },
        /* F_ES_7            */ {   KC_7,           KC_SLASH,       KC_TRNS     },

        /* F_ES_8            */ {   KC_8,           KC_9 /**/,      KC_TRNS     },
        /* F_ES_9            */ {   KC_9,           KC_0 /**/,      KC_TRNS     },
        /* F_ES_COMM         */ {   KC_COMM,        KC_SCLN,        KC_TRNS     },
        /* F_ES_DOT          */ {   KC_DOT,         KC_SCLN /**/,   KC_TRNS     },

        /* F_ES_Q            */ {   KC_Q,           KC_Q /**/,      KC_2 /**/   },
        /* F_CM_BSPC         */ {   KC_BSPC,        KC_DEL,         KC_TRNS     },
        /* F_CM_LEFT         */ {   KC_LEFT,        KC_TRNS,        KC_HOME     },
        /* F_CM_RGHT         */ {   KC_RGHT,        KC_TRNS,        KC_END      },

        /* F_KS_LT           */ {   KC_COMM /**/,   KC_DOT /**/,    KC_TRNS     },
        /* F_KS_QUOT         */ {   KC_QUOT,        KC_SLASH /**/,  KC_BSLS     },
        /* F_KS_ACUT         */ {   KC_QUOT,        KC_TRNS,        KC_TRNS     }, /* on unmod: circumflex accent dead key; on shift ¨ dead key */
        /* F_KS_PLUS         */ {   KC_EQUAL /**/,  KC_8 /**/,      KC_GRV /**/  },

        /* F_KS_LCUB         */ {   KC_LBRC /**/,   KC_LBRC,        KC_6 /**/   }, /* on altgr: ^ dead key */
        /* F_KS_RCUB         */ {   KC_RBRC /**/,   KC_RBRC,        KC_GRV      }, /* on altgr: ` dead key */

    };

    return key_index_to_keycode_set[index][column];
}

bool get_fake_en_us_shift_status_to_simulate_es_la(const uint8_t index, const uint8_t column) {
    static const uint8_t  key_index_to_keycode_shift_status[1 + (FAKE_KC_COUNT - 1) / 8][3] = {
        // FAKE_KEY                 UNMOD           SHIFT           ALTGR
        /* _7    ... _0    */   {   0b00000000,     0b01111110,     0b00000100, },
        /* _RGHT ... _8    */   {   0b00000000,     0b00011011,     0b00010000, },
        /* _RCUB ... _QUOT */   {     0b111001,       0b001011,       0b011000, },
    };

    const uint8_t row_index = index / 8;
    const uint8_t bit_index = index % 8;
    return ( key_index_to_keycode_shift_status[row_index][column] & (1 << bit_index)) ? true : false;
}

uint8_t get_index_for_fake_keycode(const uint16_t keycode) {
    return keycode - F_ES_KC_FIRST;
}

// Will implement a different behavior for the given keycode when a modifier
// is pressed, according to the data above.
// This function is strongly based on the procedure to emit KC_DEL when
// shift + KC_BSPS is pressed in the documentation.
// https://docs.qmk.fm/#/feature_advanced_keycodes?id=shift-backspace-for-delete
bool simulate_es_la_on_en_us_key_stroke(const uint8_t mod_state,
                                    	const uint16_t keycode,
                                    	const bool is_pressed) {
    // Initialize a boolean variable that keeps track
    // of the shifted status of each candidate key.
    static uint8_t registered_shifts[1 + (FAKE_KC_COUNT - 1) / 8] = {0, 0, 0};;

    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;

    if (is_altgr && is_shift) {
        // This is not allowed for these keys.
        return false;
    }

    const uint8_t index = get_index_for_fake_keycode(keycode);
    const uint8_t column = is_altgr ? 2 : (is_shift ? 1 : 0);
    const uint16_t actual_keycode = get_fake_en_us_simulating_es_la(index, column);

    if (actual_keycode == KC_TRNS) {
        // Nothing to do.
        return false;
    }

    const uint8_t row_index = index / 8;
    const uint8_t bit_index = index % 8;

    if (!is_pressed) {
        // On release of keycode, in case the altered keycode is still being sent.
        unregister_code(actual_keycode);
        if (registered_shifts[row_index] & bit_index) {
            registered_shifts[row_index] &= ~bit_index;
            return false;
        }

        return true;
    }

    const bool needs_shift = get_fake_en_us_shift_status_to_simulate_es_la(index, column);
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
            registered_shifts[row_index] |= bit_index;
        }
        register_code(actual_keycode);
        set_mods(mod_state);
        return false;
    }

    if (needs_shift) {
        set_mods(MOD_BIT(KC_LSFT));
	    registered_shifts[row_index] |= bit_index;

        register_code(actual_keycode);

        set_mods(mod_state);
        return false;
    }

    register_code(actual_keycode);

    // Just press the key.
    return true;
}

#if 0
uint16_t get_fake_to_es_la_keycode(const uint8_t index) {
    static const uint16_t fake_to_es_keymap[3] = {
        /* F_KS_LT              */ KC_NUBS,
        /* F_KS_QUOT            */ ES_QUOT,
        /* F_KS_ACUT            */ KC_LBRC,
    };

    return fake_to_es_keymap[index];
}

uint16_t get_fake_es_la_keymap(const uint8_t index, const uint8_t column) {
    static const uint16_t fake_keys_results[F_KS_KC_POST_LAST - F_KS_KC_FIRST][3] = {
        /* F_KS_LT           */ {   KC_NUBS /**/,   KC_NUBS /*s*/,  KC_TRNS      },
        /* F_KS_QUOT         */ {   KC_MINS /**/,   KC_MINS /*s*/,  KC_BSLS/*a*/ },
        /* F_KS_ACUT         */ {   KC_LBRC /**/,   KC_LBRC /*s*/,  KC_4/*a*/    },
        /* F_KS_PLUS         */ {   KC_RBRC /**/,   KC_RBRC /*s*/,  KC_RBRC/*a*/ },
        /* F_KS_LCUB         */ {   ES_ACUT /*a*/,  KC_LBRC /*a*/,  KC_LBRC/*s*/ },
        /* F_KS_RCUB         */ {   ES_CCED /*a*/,  ES_PLUS /*a*/,  ES_GRV      },
    };

    return fake_keys_results[index][column];
}

uint8_t get_fake_key_en_la_mod_status(const uint8_t index, const uint8_t column) {
    static const uint8_t fake_keys_shift_status[F_KS_KC_POST_LAST - F_KS_KC_FIRST] = {
        // FAKE_KEY ... 00: unmodified; 01: shift; 10; altgr; 11: both
        /* F_KS_LT           */ 0b000000,
        /* F_KS_QUOT         */ 0b000110,
        /* F_KS_ACUT         */ 0b000110,
        /* F_KS_PLUS         */ 0b000110,
        /* F_KS_LCUB         */ 0b101001,
        /* F_KS_RCUB         */ 0b101000,
    };

    return  (fake_keys_shift_status[index] >> (column << 1)) & 0b11;
    // return ((flags & 0b01) * MOD_BIT(KC_LSFT)) | ((flags >> 1) & 0b01) * MOD_BIT(KC_LALT);
}

bool traslate_fake_keycode_to_es_la(const uint16_t keycode,
                                    const bool is_pressed) {
    const uint16_t actual_keycode = get_fake_to_es_la_keycode(keycode - F_KS_KC_FIRST);
    if (is_pressed) {
        register_code(actual_keycode);
    } else {
        unregister_code(actual_keycode);
    }

    return false;
}

bool process_potential_modification_2(const uint8_t mod_state,
                                    const uint16_t keycode,
                                    const bool is_pressed) {
    if ((default_layer_state & (1 << _ES_LA)) == 0) {
        return process_potential_modification(mod_state, keycode, is_pressed);
    }

    return traslate_fake_keycode_to_es_la(keycode, is_pressed);
}

bool traslate_fake_keycode_to_es_es(const uint8_t mod_state,
                                    const uint16_t keycode,
                                    const bool is_pressed) {
    // Initialize a boolean variable that keeps track
    // of the shifted status of each candidate key.
    static uint8_t registered_mods[2];

    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;

    if (is_altgr && is_shift) {
        // This is not allowed for these keys.
        return false;
    }

    const uint8_t index = keycode - F_KS_LCUB;
    const uint8_t column = is_altgr ? 2 : (is_shift ? 1 : 0);
    const uint16_t actual_keycode = get_fake_es_la_keymap(index, column);
    const uint8_t flags = get_fake_key_en_la_mod_status(index, column);

    if (actual_keycode == KC_TRNS) {
        // Nothing to do.
        return false;
    }

    if (!is_pressed) {
        // On release of keycode, in case the altered keycode is still being sent.
        const uint8_t mask = 0b11 << column;
        unregister_code(actual_keycode);
        if (registered_mods[index] & mask) {
            registered_mods[index] &= ~mask;
            return false;
        }

        return true;
    }

    const uint16_t req_mods = ((flags & 0b01) * MOD_BIT(KC_LSFT)) | ((flags >> 1) & 0b01) * MOD_BIT(KC_LALT);
    const uint16_t all_masks = MOD_MASK_SHIFT | MOD_BIT(KC_RALT);

    del_mods(all_masks);
    set_mods(req_mods);
    register_code(actual_keycode);
    set_mods(mod_state);

    register_code(actual_keycode);
    registered_mods[index] |= (flags << column);

    // Just press the key.
    return true;
}

bool process_potential_modification_3(const uint8_t mod_state,
                                    const uint16_t keycode,
                                    const bool is_pressed) {
    if ((default_layer_state & (1 << _ES_LA)) == 0) {
        return process_potential_modification(mod_state, keycode, is_pressed);
    }

    return traslate_fake_keycode_to_es_es(mod_state, keycode, is_pressed);
}
#endif

bool process_fake_symbols(const uint8_t mod_state, const uint16_t keycode, const bool is_pressed) {
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    if (!is_pressed) {
        if (keycode == F_SS_BAR && !is_altgr && !is_shift) {
            unregister_code(KC_BSLS);
        }
        return true;
    }

    if (is_altgr && is_shift) {
        return true;
    }

    del_mods(MOD_MASK_SHIFT);
    if (is_altgr) {
        if (keycode != F_SS_BAR) {
            return false;
        }

        // F_SS_BAR, on altgr
        // ¬ ...
        SEND_STRING(SS_RALT(SS_TAP(X_KP_0)SS_TAP(X_KP_1)SS_TAP(X_KP_7)SS_TAP(X_KP_2)));
        goto done;
    }

    switch (keycode) {
        case F_SS_NTILDE:
            if (is_shift) {
                // capital n-tilde
                SEND_STRING(SS_RALT(SS_TAP(X_KP_0)SS_TAP(X_KP_2)SS_TAP(X_KP_0)SS_TAP(X_KP_9)));
            } else {
                // n-tilde
                SEND_STRING(SS_RALT(SS_TAP(X_KP_0)SS_TAP(X_KP_2)SS_TAP(X_KP_4)SS_TAP(X_KP_1)));
            }
            break;

        case F_SS_INV_QUESTION:
            //oled_set_cursor(0, 4);
            if (is_shift) {
                // starting exclamation
                //oled_write("exclm", false);
                SEND_STRING(SS_RALT(SS_TAP(X_KP_0)SS_TAP(X_KP_1)SS_TAP(X_KP_6)SS_TAP(X_KP_1)));
            } else {
                // starting interrogation
                //oled_write("intrr", false);
                SEND_STRING(SS_RALT(SS_TAP(X_KP_0)SS_TAP(X_KP_1)SS_TAP(X_KP_9)SS_TAP(X_KP_1)));
            }
            break;

        case F_SS_BAR:
            if (is_shift) {
                // °
                SEND_STRING(SS_RALT(SS_TAP(X_KP_0)SS_TAP(X_KP_1)SS_TAP(X_KP_7)SS_TAP(X_KP_6)));
            } else {
                // |
                set_mods(MOD_BIT(KC_LSFT));
                register_code(KC_BSLS);
                del_mods(MOD_BIT(KC_LSFT));
                return true;
            }
            break;

        default:
            break;
    }

done:
    set_mods(mod_state);
    return false;
}


//////////////////////////////////////////////////////////
// Functions to handled shifted-to-shifted translation, i.e. a shifted
// keycode is translated to a different shifted keycode.
static uint8_t uusst_kc_shifted[UUSST_KC_POST_LAST - UUSST_KC_FIRST][2] = {
    /* UUSST_KC_2       */  { KC_2      , KC_QUOT,  }, // double quotes, as in ES-LA keymap.
    /* UUSST_KC_6       */  { KC_6      , KC_7,     }, // ampersand, as in ES-LA keymap.
    /* UUSST_KC_8       */  { KC_8      , KC_9      }, // left parenthesis, as in ES-LA keymap.
    /* UUSST_KC_9       */  { KC_9      , KC_0      }, // right parenthesis, as in ES-LA keymap.
    /* UUSST_KC_QUOT    */  { KC_QUOT   , KC_SLSH   }, // question mark, as in ES-LA keymap.
    /* UUSST_KC_DOT     */  { KC_DOT    , KC_SCLN   }, // semicolon, as in ES-LA keymap.
};

#define UUSST_KC_FLAG_VECTOR_SIZE (1 + (UUSST_KC_COUNT - 1) / 8)
static uint8_t uusst_kc_is_pressed[UUSST_KC_FLAG_VECTOR_SIZE] = {
    0,
};

uint8_t get_uusst_kc_index(const uint8_t keycode) {
    return keycode - UUSST_KC_FIRST;
}

bool uusst_kc_is_pressed_any(void) {
    for (uint8_t i = 0; i < UUSST_KC_FLAG_VECTOR_SIZE; ++i) {
        if (uusst_kc_is_pressed[i]) {
            return true;
        }
    }
    return false;
}

bool uusst_kc_is_pressed_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    return uusst_kc_is_pressed[arr_index] & (1 << bit_index) ? true : false;
}

void uusst_kc_is_pressed_set_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    uusst_kc_is_pressed[arr_index] |= (1 << bit_index);
}

void uusst_kc_is_pressed_unset_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    uusst_kc_is_pressed[arr_index] &= ~(1 << bit_index);
}

bool uusst_kc_operate_on_shift(const uint16_t keycode,
                            const bool is_pressed) {
    if (!uusst_kc_is_pressed_any()) {
        // no UUSST_KC key is pressed, nothing to do here.
        return true;
    }


    if (is_pressed) {
        for (uint8_t index = 0; index < UUSST_KC_COUNT; ++index) {
            register_code(keycode);

            if (uusst_kc_is_pressed_on(index)) {
                const uint16_t shift_kc = uusst_kc_shifted[index][1];
                register_code(shift_kc);
            }
        }

        return true;
    }

    for (uint8_t index = 0; index < UUSST_KC_COUNT; ++index) {
        unregister_code(keycode);

        if (uusst_kc_is_pressed_on(index)) {
            const uint16_t plain_kc = uusst_kc_shifted[index][0];
            const uint16_t shift_kc = uusst_kc_shifted[index][1];
            unregister_code(shift_kc);
            register_code(plain_kc);
        }
    }

    return true;
}

bool uusst_kc_operate_on_keycode(const uint8_t mod_state,
                              const uint16_t keycode,
                              const bool is_pressed) {
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    if (is_altgr && is_shift) {
        // alt+shift does not work in this case.
        return true;
    }

    const uint8_t index = get_uusst_kc_index(keycode);
    const uint16_t plain_kc = uusst_kc_shifted[index][0];
    const uint16_t shift_kc = uusst_kc_shifted[index][1];

    if (is_pressed) {
        uusst_kc_is_pressed_set_on(index);
        if (is_shift) {
            register_code(shift_kc);
            return true;
        }

        register_code(plain_kc);
        return true;
    }

    uusst_kc_is_pressed_unset_on(index);
    if (is_shift) {
        unregister_code(shift_kc);
        return true;
    }

    unregister_code(plain_kc);
    return true;
}

//////////////////////////////////////////////////////////
// Functions to handled shifted-to-unshifted translation, i.e. a shifted
// keycode is translated to a different unshifted keycode.
static uint8_t uusut_kc_shifted[UUSUT_KC_POST_LAST - UUSUT_KC_FIRST][2] = {
    /* UUSUT_KC_7       */  { KC_7      , KC_SLSH,  }, // slash, as in ES-LA keymap.
    /* UUSUT_KC_0       */  { KC_0      , KC_EQL,   }, // equal, as in ES-LA keymap.
    /* UUSUT_KC_COMM    */  { KC_COMM   , KC_SCLN   }, // semicolon, as in ES-LA keymap.
};

#define UUSUT_KC_FLAG_VECTOR_SIZE (1 + (UUSUT_KC_COUNT - 1) / 8)
static uint8_t uusut_kc_is_pressed[UUSUT_KC_FLAG_VECTOR_SIZE] = {
    0,
};

uint8_t get_uusut_kc_index(const uint8_t keycode) {
    return keycode - UUSUT_KC_FIRST;
}

bool uusut_kc_is_pressed_any(void) {
    for (uint8_t i = 0; i < UUSUT_KC_FLAG_VECTOR_SIZE; ++i) {
        if (uusut_kc_is_pressed[i]) {
            return true;
        }
    }
    return false;
}

bool uusut_kc_is_pressed_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    return uusut_kc_is_pressed[arr_index] & (1 << bit_index) ? true : false;
}

void uusut_kc_is_pressed_set_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    uusut_kc_is_pressed[arr_index] |= (1 << bit_index);
}

void uusut_kc_is_pressed_unset_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    uusut_kc_is_pressed[arr_index] &= ~(1 << bit_index);
}

bool uusut_kc_operate_on_shift(const uint16_t keycode,
                            const bool is_pressed) {
    if (!uusut_kc_is_pressed_any()) {
        // no UUSUT_KC key is pressed, nothing to do here.
        return true;
    }


    if (is_pressed) {
        for (uint8_t index = 0; index < UUSUT_KC_COUNT; ++index) {
            del_mods(MOD_MASK_SHIFT);

            if (uusut_kc_is_pressed_on(index)) {
                const uint16_t shift_kc = uusut_kc_shifted[index][1];
                register_code(shift_kc);
            }
        }
        return false;
    }

    for (uint8_t index = 0; index < UUSUT_KC_COUNT; ++index) {
        if (uusut_kc_is_pressed_on(index)) {
            const uint16_t plain_kc = uusut_kc_shifted[index][0];
            const uint16_t shift_kc = uusut_kc_shifted[index][1];
            unregister_code(shift_kc);
            register_code(plain_kc);
        }
    }

    return true;
}

bool uusut_kc_operate_on_keycode(const uint8_t mod_state,
                              const uint16_t keycode,
                              const bool is_pressed) {
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    if (is_altgr && is_shift) {
        // alt+shift does not work in this case.
        return true;
    }

    const uint8_t index = get_uusut_kc_index(keycode);
    const uint16_t plain_kc = uusut_kc_shifted[index][0];
    const uint16_t shift_kc = uusut_kc_shifted[index][1];

    if (is_pressed) {
        uusut_kc_is_pressed_set_on(index);
        if (is_shift) {
            del_mods(MOD_MASK_SHIFT);
            register_code(shift_kc);
            set_mods(mod_state);
            return false;
        }

        register_code(plain_kc);
        return true;
    }

    uusut_kc_is_pressed_unset_on(index);
    unregister_code(shift_kc);
    unregister_code(plain_kc);
    return true;
}

