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

//////////////////////////////////////////////////////////
// Shift-key status.

static uint8_t shift_pressed_status = 0;

void mark_shift_pressed(const uint16_t keycode,
                        const bool is_pressed) {
    if (is_pressed) {
        if (keycode == KC_LSFT) {
            shift_pressed_status |= 0x01;
        } else if (keycode == KC_RSFT) {
            shift_pressed_status |= 0x10;
        }
    } else {
        if (keycode == KC_LSFT) {
            shift_pressed_status &= ~0x01;
        } else if (keycode == KC_RSFT) {
            shift_pressed_status &= ~0x10;
        }
    }
}

bool left_shift_is_pressed(void) {
    return (shift_pressed_status & 0b01) ? true : false;
}

bool right_shift_is_pressed(void) {
    return (shift_pressed_status & 0b10) ? true : false;
}

// This function is strongly based on the procedure to emit KC_DEL when
// shift + KC_BSPS is pressed in the documentation.
// https://docs.qmk.fm/#/feature_advanced_keycodes?id=shift-backspace-for-delete
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
    /* UUSST_KC_NTIL    */  { KC_N      , KC_N      }, // n-tilde, as in ES-LA keymap.
    /* UUSST_KC_Q       */  { KC_DOT    , KC_SCLN   }, // q, as in ES-LA keymap.
    /* UUSST_KC_ACUT    */  { KC_QUOT   , KC_QUOT   }, // acute, dieresis, as in ES-LA keymap.
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
    /* UUSUT_KC_BSDL    */  { KC_BSPC   , KC_DEL    }, // backspace, del when shifted.
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

//////////////////////////////////////////////////////////
// Functions to handle shifted-to-unshifted translation, i.e. a shifted
// keycode is translated to a different unshifted keycode.
static uint8_t ussst_kc_shifted[USSST_KC_POST_LAST - USSST_KC_FIRST][2] = {
    /* USSST_KC_LABK    */  { KC_COMM   , KC_DOT,   }, // <> ... as in ES-LA keymap.
    /* USSST_KC_PLUS    */  { KC_EQL    , KC_8,     }, // +* ... as in ES-LA keymap.
    /* USSST_KC_IQUE    */  { KC_SLSH   , KC_1,     }, // ¿¡ ... as in ES-LA keymap.
};

#define USSST_KC_FLAG_VECTOR_SIZE (1 + (USSST_KC_COUNT - 1) / 8)
static uint8_t ussst_kc_is_pressed[USSST_KC_FLAG_VECTOR_SIZE] = {
    0,
};

uint8_t get_ussst_kc_index(const uint8_t keycode) {
    return keycode - USSST_KC_FIRST;
}

bool ussst_kc_is_pressed_any(void) {
    for (uint8_t i = 0; i < USSST_KC_FLAG_VECTOR_SIZE; ++i) {
        if (ussst_kc_is_pressed[i]) {
            return true;
        }
    }
    return false;
}

bool ussst_kc_is_pressed_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    return ussst_kc_is_pressed[arr_index] & (1 << bit_index) ? true : false;
}

void ussst_kc_is_pressed_set_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    ussst_kc_is_pressed[arr_index] |= (1 << bit_index);
}

void ussst_kc_is_pressed_unset_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    ussst_kc_is_pressed[arr_index] &= ~(1 << bit_index);
}

bool ussst_kc_operate_on_shift(const uint16_t keycode,
                            const bool is_pressed) {
    if (!ussst_kc_is_pressed_any()) {
        // no USSST_KC key is pressed, nothing to do here.
        return true;
    }

    if (is_pressed) {
        for (uint8_t index = 0; index < USSST_KC_COUNT; ++index) {
            register_code(keycode);

            if (ussst_kc_is_pressed_on(index)) {
                const uint16_t shift_kc = ussst_kc_shifted[index][1];
                register_code(shift_kc);
            }
        }
        return false;
    }

    for (uint8_t index = 0; index < USSST_KC_COUNT; ++index) {
        if (ussst_kc_is_pressed_on(index)) {
            const uint16_t plain_kc = ussst_kc_shifted[index][0];
            const uint16_t shift_kc = ussst_kc_shifted[index][1];
            unregister_code(shift_kc);
            register_code(plain_kc);
        }
    }

    return false;
}

bool ussst_kc_operate_on_keycode(const uint8_t mod_state,
                              const uint16_t keycode,
                              const bool is_pressed) {
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    if (is_altgr && is_shift) {
        // alt+shift does not work in this case.
        return true;
    }

    const uint8_t index = get_ussst_kc_index(keycode);
    const uint16_t plain_kc = ussst_kc_shifted[index][0];
    const uint16_t shift_kc = ussst_kc_shifted[index][1];

    if (is_pressed) {
        ussst_kc_is_pressed_set_on(index);
        if (is_shift) {
           register_code(shift_kc);
           return false;
        }

        set_mods(MOD_MASK_SHIFT);
        register_code(plain_kc);
        set_mods(mod_state);
        return true;
    }

    ussst_kc_is_pressed_unset_on(index);
    unregister_code(shift_kc);
    unregister_code(plain_kc);
    if (!left_shift_is_pressed()) { unregister_code(KC_LSFT); }
    if (!right_shift_is_pressed()) { unregister_code(KC_RSFT); }
    return true;
}

//////////////////////////////////////////////////////////
// Functions to handle shifted-to-unshifted translation, i.e. a shifted
// keycode is translated to a different unshifted keycode.
static uint8_t ussut_kc_shifted[USSUT_KC_POST_LAST - USSUT_KC_FIRST][2] = {
    /* USSUT_KC_LCBR    */  { KC_LBRC   , KC_LBRC,  }, // {[ ... as in ES-LA keymap.
    /* USSUT_KC_RCBR    */  { KC_RBRC   , KC_RBRC,  }, // }] ... as in ES-LA keymap.
    /* USSUT_KC_PIPE    */  { KC_BSLS   , KC_GRV,   }, // |°¬ ... as in ES-LA keymap.
};

#define USSUT_KC_FLAG_VECTOR_SIZE (1 + (USSUT_KC_COUNT - 1) / 8)
static uint8_t ussut_kc_is_pressed[USSUT_KC_FLAG_VECTOR_SIZE] = {
    0,
};

uint8_t get_ussut_kc_index(const uint8_t keycode) {
    return keycode - USSUT_KC_FIRST;
}

bool ussut_kc_is_pressed_any(void) {
    for (uint8_t i = 0; i < USSUT_KC_FLAG_VECTOR_SIZE; ++i) {
        if (ussut_kc_is_pressed[i]) {
            return true;
        }
    }
    return false;
}

bool ussut_kc_is_pressed_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    return ussut_kc_is_pressed[arr_index] & (1 << bit_index) ? true : false;
}

void ussut_kc_is_pressed_set_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    ussut_kc_is_pressed[arr_index] |= (1 << bit_index);
}

void ussut_kc_is_pressed_unset_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    ussut_kc_is_pressed[arr_index] &= ~(1 << bit_index);
}

bool ussut_kc_operate_on_shift(const uint16_t keycode,
                               const bool is_pressed) {
    if (!ussut_kc_is_pressed_any()) {
        // no USSUT_KC key is pressed, nothing to do here.
        return true;
    }


    if (is_pressed) {
        del_mods(MOD_MASK_SHIFT);
        for (uint8_t index = 0; index < USSUT_KC_COUNT; ++index) {

            if (ussut_kc_is_pressed_on(index)) {
                const uint16_t shift_kc = ussut_kc_shifted[index][1];
                register_code(shift_kc);
            }
        }
        return false;
    }

    set_mods(MOD_MASK_SHIFT);
    for (uint8_t index = 0; index < USSUT_KC_COUNT; ++index) {
        if (ussut_kc_is_pressed_on(index)) {
            const uint16_t plain_kc = ussut_kc_shifted[index][0];
            const uint16_t shift_kc = ussut_kc_shifted[index][1];
            unregister_code(shift_kc);
            register_code(plain_kc);
        }
    }
    del_mods(MOD_MASK_SHIFT);

    return false;
}

bool ussut_kc_operate_on_keycode(const uint8_t mod_state,
                              const uint16_t keycode,
                              const bool is_pressed) {
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    if (is_altgr && is_shift) {
        // alt+shift does not work in this case.
        return true;
    }

    const uint8_t index = get_ussut_kc_index(keycode);
    const uint16_t plain_kc = ussut_kc_shifted[index][0];
    const uint16_t shift_kc = ussut_kc_shifted[index][1];

    if (is_pressed) {
        ussut_kc_is_pressed_set_on(index);
        if (is_shift) {
            del_mods(MOD_MASK_SHIFT);
            register_code(shift_kc);
            set_mods(mod_state);
            return false;
        }

        set_mods(MOD_MASK_SHIFT);
        register_code(plain_kc);
        del_mods(mod_state);
        return false;
    }

    ussut_kc_is_pressed_unset_on(index);
    unregister_code(shift_kc);
    unregister_code(plain_kc);
    if (!left_shift_is_pressed()) { unregister_code(KC_LSFT); }
    if (!right_shift_is_pressed()) { unregister_code(KC_RSFT); }
    return true;
}

