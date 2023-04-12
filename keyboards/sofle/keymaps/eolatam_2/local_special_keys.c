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
#include "local_general.h"

#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Vector handling function.
static void internal_show_bits(const uint8_t* const vec,
                               const uint8_t s,
                               const char* const msg) {
#ifdef CONSOLE_ENABLE
    dprintf("%s: ", msg);
    for (uint8_t i = s; i > 0; --i) {
        dprintf(BYTE_TO_BINARY_PATTERN " ", BYTE_TO_BINARY(vec[i - 1]));
    }
    dprintf("\n");
#endif
}

static bool internal_get_bit(const uint8_t* const vec,
                             const uint8_t index) {
    EOA_BIT_POS(uint8_t, 8, index, arr_index, bit_index);
    return (vec[arr_index] & (1 << bit_index)) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// The following keycodes have been added so when computer is in EN-US keymap,
// they behave as ES-LA keycodes.
// TODO: Add alt-gr functionality.
//
// UUSST:   unshifted translates to unshifted keycode.
//            shifted translates to   shifted keycode.
//                  |   current |  expected |
// - UUSST_KC_2     :       2"  |       2"@ |
// - UUSST_KC_6     :       6&  |       6&  |
// - UUSST_KC_8     :       8(  |       8(  |
// - UUSST_KC_9     :       9)  |       9)  |
// - UUSST_KC_QUOT  :       '?  |       '?\ |
// - UUSST_KC_DOT   :       .:  |       .:  |
// - UUSST_KC_NTIL  :       nN  |       ñÑ  |
// - UUSST_KC_Q     :       qQ  |       qQ@ |
// - UUSST_KC_ACUT  :       '"  |       '"  |
//
// UUSUT:   unshifted translates to unshifted keycode.
//          shifted translate to unshifted keycode.
//                  |   current |  expected |
// - UUSUT_KC_7     :       7/  |       7/  |
// - UUSUT_KC_0     :       0=  |       0=  |
// - UUSUT_KC_COMM  :       ,;  |       ,;  |
// - UUSUT_KC_BSDL  : bkspc,del | bkspc,del |
//
// USSST:   unshifted translates to   shifted keycode.
//            shifted translates to   shifted keycode.
//                  |   current |  expected |
// - USSST_KC_LABK  :       <>  |       <>  |
// - USSST_KC_PLUS  :       +*  |       +*~ |
// - USSST_KC_IQUE  :       ?!  |       ¿¡  |
//
// USSST:   unshifted translates to   shifted keycode.
//            shifted translates to   shifted keycode.
//                  |   current |  expected |
// - USSUT_KC_LCBR  :       {[  |       {[^ |
// - USSUT_KC_RCBR  :       }]  |       }]` |
// - USSUT_KC_PIPE  :       |`  |       |°¬ |

static const uint8_t ust_kc_shifted[UST_KC_COUNT][3] = {
    /* begin UUSST block*/
    /* UUSST_KC_2       */  {   KC_2,       KC_QUOT,    KC_2,       }, // 2"@ ... as in ES-LA keymap.
    /* UUSST_KC_6       */  {   KC_6,       KC_7,       KC_TRNS,    }, // 6&  ... as in ES-LA keymap.
    /* UUSST_KC_8       */  {   KC_8,       KC_9,       KC_TRNS,    }, // 8(  ... as in ES-LA keymap.
    /* UUSST_KC_9       */  {   KC_9,       KC_0,       KC_TRNS,    }, // 9)  ... as in ES-LA keymap.
    /* UUSST_KC_QUOT    */  {   KC_QUOT,    KC_SLSH,    KC_BSLS,    }, // '?\ ... as in ES-LA keymap.
    /* UUSST_KC_DOT     */  {   KC_DOT,     KC_SCLN,    KC_TRNS,    }, // .:  ... as in ES-LA keymap.
    /* UUSST_KC_NTIL    */  {   KC_N,       KC_N,       KC_TRNS,    }, // ñÑ  ... as in ES-LA keymap.
    /* UUSST_KC_Q       */  {   KC_Q,       KC_Q,       KC_2,       }, // qQ@ ... as in ES-LA keymap.
    /* UUSST_KC_ACUT    */  {   KC_QUOT,    KC_QUOT,    KC_TRNS,    }, // ´¨  ... as in ES-LA keymap.
    /* end UUSST block  */


    /* begin UUSUT block*/
    /* UUSUT_KC_7       */  {   KC_7,       KC_SLSH,    KC_TRNS,    }, // 7/  ... as in ES-LA keymap.
    /* UUSUT_KC_0       */  {   KC_0,       KC_EQL,     KC_TRNS,    }, // 0=  ... as in ES-LA keymap.
    /* UUSUT_KC_COMM    */  {   KC_COMM,    KC_SCLN,    KC_TRNS,    }, // ,;  ... as in ES-LA keymap.
    /* UUSUT_KC_BSDL    */  {   KC_BSPC,    KC_DEL,     KC_TRNS,    }, // backspace, del
    /* end UUSUT block  */


    /* begin UUSST block*/
    /* USSST_KC_LABK    */  {   KC_COMM,    KC_DOT,     KC_TRNS,    }, // <>  ... as in ES-LA keymap.
    /* USSST_KC_PLUS    */  {   KC_EQL,     KC_8,       KC_GRV,     }, // +*~ ... as in ES-LA keymap.
    /* USSST_KC_IQUE    */  {   KC_SLSH,    KC_1,       KC_TRNS,    }, // ¿¡  ... as in ES-LA keymap.
    /* end UUSST block  */


    /* begin USSUT block*/
    /* USSUT_KC_LCBR    */  {   KC_LBRC,    KC_LBRC,    KC_6,       }, // {[^ ... as in ES-LA keymap.
    /* USSUT_KC_RCBR    */  {   KC_RBRC,    KC_RBRC,    KC_GRV,     }, // }]` ... as in ES-LA keymap.
    /* USSUT_KC_PIPE    */  {   KC_BSLS,    KC_GRV,     KC_TRNS,    }, // |°¬ ... as in ES-LA keymap.
    /* end USSUT block  */
};

static const uint8_t ust_kc_altgrd_indices[6] = {
    EOA_GET_UST_KC_INDEX(UUSST_KC_2),
    EOA_GET_UST_KC_INDEX(UUSST_KC_QUOT),
    EOA_GET_UST_KC_INDEX(UUSST_KC_Q),
    EOA_GET_UST_KC_INDEX(USSST_KC_PLUS),
    EOA_GET_UST_KC_INDEX(USSUT_KC_LCBR),
    EOA_GET_UST_KC_INDEX(USSUT_KC_RCBR),
};

static const uint8_t ust_kc_altgrd[(1 + ((UST_KC_COUNT - 1) / 8))] = {
    0b10000001,
    0b01000000,
    0b00000001,
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Flags needed to mark each UST keycode as pressed, if so.
// Additionally we need three extra flags for LSFT, RSFT, RALT keycodes.

// Two flags for KC_LSFT, KC_RSFT, KC_RALT
#define UST_MOD_FLAG_COUNT        3
// Total flags: UST keycode one's + shift one's.
#define UST_TOTAL_FLAG_COUNT        (UST_KC_COUNT + UST_MOD_FLAG_COUNT)
// Number of uint8_t needed for UST_TOTAL_FLAG_COUNT.
#define UST_UINT8_FLAG_VECTOR_SIZE  (1 + ((UST_TOTAL_FLAG_COUNT - 1) / 8))

// TODO: Enhance macro REP so it allows passing a macro-defined value with the
//       same complexity as UST_UINT8_FLAG_VECTOR_SIZE.
#define UST_UINT8_FLAG_VECTOR_SIZE_EQUIV    3
static uint8_t ust_kc_is_pressed[UST_UINT8_FLAG_VECTOR_SIZE] = {
    REP(UST_UINT8_FLAG_VECTOR_SIZE_EQUIV, 0)
};

void ust_show_key_flags(const char* const msg) {
    internal_show_bits(ust_kc_is_pressed, UST_UINT8_FLAG_VECTOR_SIZE, msg);
}

uint8_t get_ust_kc_index(const uint16_t keycode) {
    return EOA_GET_UST_KC_INDEX(keycode);
}

bool ust_kc_is_pressed_on(const uint8_t index) {
    return internal_get_bit(ust_kc_is_pressed, index);
}

void ust_kc_is_pressed_set_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    ust_kc_is_pressed[arr_index] |= (1 << bit_index);
}

void ust_kc_is_pressed_unset_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    ust_kc_is_pressed[arr_index] &= ~(1 << bit_index);
}

bool ust_kc_is_pressed_any(const uint8_t first, const uint8_t last) {
    // TODO: Optimize so it doesn't go bit by bit.
    EOA_FOR_RANGE(uint8_t, i, first, last) {
        if (ust_kc_is_pressed_on(i)) {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////
// mod-key status.

// KC_LSFT flag index
#define UST_LSFT_INDEX              UST_KC_COUNT
// KC_RSFT flag index
#define UST_RSFT_INDEX              (1 + UST_LSFT_INDEX)
// KC_RALT flag index
#define UST_RALT_INDEX              (1 + UST_RSFT_INDEX)

uint8_t get_mod_keycode_index(const uint16_t keycode) {
    switch (keycode) {
        case KC_LSFT:
            return UST_LSFT_INDEX;
        case KC_RSFT:
            return UST_RSFT_INDEX;
        case KC_RALT:
            return UST_RALT_INDEX;
    }

    return UST_RALT_INDEX;
}

void mark_modifier_pressed(const uint16_t keycode,
                           const bool is_pressed) {
    const uint8_t index = get_mod_keycode_index(keycode);
    if (is_pressed) {
        ust_kc_is_pressed_set_on(index);
    } else {
        ust_kc_is_pressed_unset_on(index);
    }
}

bool l_shift_is_pressed(void) {
    return ust_kc_is_pressed_on(UST_LSFT_INDEX);
}

bool r_shift_is_pressed(void) {
    return ust_kc_is_pressed_on(UST_RSFT_INDEX);
}

bool r_alt_is_pressed(void) {
    return ust_kc_is_pressed_on(UST_RALT_INDEX);
}

bool any_shift_is_pressed(void) {
    const uint8_t arr_index = UST_LSFT_INDEX / 8;
    const uint8_t bit_index = UST_LSFT_INDEX % 8;
    if (bit_index == 7) {
        return l_shift_is_pressed() || r_shift_is_pressed();
    }

    return ust_kc_is_pressed[arr_index] & (0b11 << bit_index) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
// Functions to handled shifted-to-shifted translation, i.e. a shifted keycode
// is translated to a different shifted keycode.
#define UUSST_KC_FIRST_INDEX   (get_ust_kc_index(UUSST_KC_FIRST))
#define UUSST_KC_LAST_INDEX    (get_ust_kc_index(UUSST_KC_LAST ))
bool uusst_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed) {
    if (r_alt_is_pressed()) {
        return true;
    }

    if (!ust_kc_is_pressed_any(UUSST_KC_FIRST_INDEX, UUSST_KC_LAST_INDEX)) {
        // no UUSST_KC key is pressed, nothing to do here.
        return true;
    }

    if (is_pressed) {
        EOA_FOR_RANGE(uint8_t, index, UUSST_KC_FIRST_INDEX, UUSST_KC_LAST_INDEX) {
            register_code(keycode);

            if (ust_kc_is_pressed_on(index)) {
                const uint16_t shift_kc = ust_kc_shifted[index][1];
                register_code(shift_kc);
            }
        }

        return true;
    }

    EOA_FOR_RANGE(uint8_t, index, UUSST_KC_FIRST_INDEX, UUSST_KC_LAST_INDEX) {
        unregister_code(keycode);

        if (ust_kc_is_pressed_on(index)) {
            const uint16_t plain_kc = ust_kc_shifted[index][0];
            const uint16_t shift_kc = ust_kc_shifted[index][1];
            unregister_code(shift_kc);
            register_code(plain_kc);
        }
    }

    return true;
}

bool uusst_kc_operate_on_altgr_keycode(const uint8_t mod_state,
                                       const uint16_t keycode,
                                       const bool is_pressed) {
    const uint8_t index = get_ust_kc_index(keycode);
    const uint16_t altgr_kc = ust_kc_shifted[index][2];
    if (altgr_kc == KC_TRNS) {
        // nothing to do.
        return true;
    }

    const bool is_shifted = internal_get_bit(ust_kc_altgrd, index);
    if (is_pressed) {
        del_mods(MOD_BIT(KC_RALT));
        if (is_shifted) {
            set_mods(MOD_MASK_SHIFT);
            register_code(altgr_kc);
        } else {
            // symbol is plain.
            register_code(altgr_kc);
        }
        set_mods(mod_state);
        return false;
    }

    unregister_code(altgr_kc);
    return false;
}

bool uusst_kc_operate_on_shift_keycode(const uint8_t mod_state,
                                       const uint16_t keycode,
                                       const bool is_pressed) {
    const uint8_t index = get_ust_kc_index(keycode);
    const uint16_t plain_kc = ust_kc_shifted[index][0];
    const uint16_t shift_kc = ust_kc_shifted[index][1];

    if (is_pressed) {
        ust_kc_is_pressed_set_on(index);
        if (any_shift_is_pressed()) {
            register_code(shift_kc);
            return true;
        }

        register_code(plain_kc);
        return true;
    }

    ust_kc_is_pressed_unset_on(index);
    if (any_shift_is_pressed()) {
        unregister_code(shift_kc);
        return true;
    }

    unregister_code(plain_kc);
    return true;
}

bool uusst_kc_operate_on_keycode(const uint8_t mod_state,
                                 const uint16_t keycode,
                                 const bool is_pressed) {
    if (r_alt_is_pressed()) {
        if (any_shift_is_pressed()) {
            // alt+shift does not work in this case.
            return true;
        }

        return uusst_kc_operate_on_altgr_keycode(mod_state, keycode, is_pressed);
    }

    return uusst_kc_operate_on_shift_keycode(mod_state, keycode, is_pressed);
}

//////////////////////////////////////////////////////////
// Functions to handled shifted-to-unshifted translation, i.e. a shifted
// keycode is translated to a different unshifted keycode.
#define UUSUT_KC_FIRST_INDEX   (get_ust_kc_index(UUSUT_KC_FIRST))
#define UUSUT_KC_LAST_INDEX    (get_ust_kc_index(UUSUT_KC_LAST ))
bool uusut_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed) {
    if (r_alt_is_pressed()) {
        return true;
    }

    if (!ust_kc_is_pressed_any(UUSUT_KC_FIRST_INDEX, UUSUT_KC_LAST_INDEX)) {
        // no UUSUT_KC key is pressed, nothing to do here.
        return true;
    }

    if (is_pressed) {
        del_mods(MOD_MASK_SHIFT);
        uint8_t count = 0;
        EOA_FOR_RANGE(uint8_t, index, UUSUT_KC_FIRST_INDEX, UUSUT_KC_LAST_INDEX) {
            if (ust_kc_is_pressed_on(index)) {
                const uint16_t shift_kc = ust_kc_shifted[index][1];
                register_code(shift_kc);
                ++count;
            }
        }
        set_mods(mod_state);
        return false;
    }

    EOA_FOR_RANGE(uint8_t, index, UUSUT_KC_FIRST_INDEX, UUSUT_KC_LAST_INDEX) {
        if (ust_kc_is_pressed_on(index)) {
            const uint16_t plain_kc = ust_kc_shifted[index][0];
            const uint16_t shift_kc = ust_kc_shifted[index][1];
            unregister_code(shift_kc);
            register_code(plain_kc);
        }
    }

    return true;
}

bool uusut_kc_operate_on_altgr_keycode(const uint8_t mod_state,
                                       const uint16_t keycode,
                                       const bool is_pressed) {
    return uusst_kc_operate_on_altgr_keycode(mod_state, keycode, is_pressed);
}

// This function is strongly based on the procedure to emit KC_DEL when
// shift + KC_BSPC is pressed in the documentation.
// https://docs.qmk.fm/#/feature_advanced_keycodes?id=shift-backspace-for-delete
bool uusut_kc_operate_on_shift_keycode(const uint8_t mod_state,
                                       const uint16_t keycode,
                                       const bool is_pressed) {
    const uint8_t index = get_ust_kc_index(keycode);
    const uint16_t plain_kc = ust_kc_shifted[index][0];
    const uint16_t shift_kc = ust_kc_shifted[index][1];

    if (is_pressed) {
        ust_kc_is_pressed_set_on(index);
        if (any_shift_is_pressed()) {
            del_mods(MOD_MASK_SHIFT);
            register_code(shift_kc);
            set_mods(mod_state);
            return false;
        }

        register_code(plain_kc);
        return true;
    }

    ust_kc_is_pressed_unset_on(index);
    unregister_code(shift_kc);
    unregister_code(plain_kc);
    return true;
}

bool uusut_kc_operate_on_keycode(const uint8_t mod_state,
                                 const uint16_t keycode,
                                 const bool is_pressed) {
    if (r_alt_is_pressed()) {
        if (any_shift_is_pressed()) {
            // alt+shift does not work in this case.
            return true;
        }

        return uusut_kc_operate_on_altgr_keycode(mod_state, keycode, is_pressed);
    }

    return uusut_kc_operate_on_shift_keycode(mod_state, keycode, is_pressed);
}

//////////////////////////////////////////////////////////
// Functions to handle shifted-to-unshifted translation, i.e. a shifted
// keycode is translated to a different unshifted keycode.
#define USSST_KC_FIRST_INDEX   (get_ust_kc_index(USSST_KC_FIRST))
#define USSST_KC_LAST_INDEX    (get_ust_kc_index(USSST_KC_LAST ))
bool ussst_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed) {
    if (r_alt_is_pressed()) {
        return true;
    }

    if (!ust_kc_is_pressed_any(USSST_KC_FIRST_INDEX, USSST_KC_LAST_INDEX)) {
        // no USSST_KC key is pressed, nothing to do here.
        return true;
    }

    if (is_pressed) {
        EOA_FOR_RANGE(uint8_t, index, USSST_KC_FIRST_INDEX, USSST_KC_LAST_INDEX) {
            register_code(keycode);

            if (ust_kc_is_pressed_on(index)) {
                const uint16_t shift_kc = ust_kc_shifted[index][1];
                register_code(shift_kc);
            }
        }
        return false;
    }

    EOA_FOR_RANGE(uint8_t, index, USSST_KC_FIRST_INDEX, USSST_KC_LAST_INDEX) {
        if (ust_kc_is_pressed_on(index)) {
            const uint16_t plain_kc = ust_kc_shifted[index][0];
            const uint16_t shift_kc = ust_kc_shifted[index][1];
            unregister_code(shift_kc);
            register_code(plain_kc);
        }
    }

    return false;
}

bool ussst_kc_operate_on_altgr_keycode(const uint8_t mod_state,
                                       const uint16_t keycode,
                                       const bool is_pressed) {
    return uusst_kc_operate_on_altgr_keycode(mod_state, keycode, is_pressed);
}

bool ussst_kc_operate_on_shift_keycode(const uint8_t mod_state,
                                       const uint16_t keycode,
                                       const bool is_pressed) {
    const uint8_t index = get_ust_kc_index(keycode);
    const uint16_t plain_kc = ust_kc_shifted[index][0];
    const uint16_t shift_kc = ust_kc_shifted[index][1];

    if (is_pressed) {
        ust_kc_is_pressed_set_on(index);
        if (any_shift_is_pressed()) {
           register_code(shift_kc);
           return false;
        }

        set_mods(MOD_MASK_SHIFT);
        register_code(plain_kc);
        set_mods(mod_state);
        return true;
    }

    ust_kc_is_pressed_unset_on(index);
    unregister_code(shift_kc);
    unregister_code(plain_kc);
    if (!l_shift_is_pressed()) { unregister_code(KC_LSFT); }
    if (!r_shift_is_pressed()) { unregister_code(KC_RSFT); }
    return true;
}

bool ussst_kc_operate_on_keycode(const uint8_t mod_state,
                                 const uint16_t keycode,
                                 const bool is_pressed) {
    if (r_alt_is_pressed()) {
        if (any_shift_is_pressed()) {
            // alt+shift does not work in this case.
            return true;
        }

        return ussst_kc_operate_on_altgr_keycode(mod_state, keycode, is_pressed);
    }

    return ussst_kc_operate_on_shift_keycode(mod_state, keycode, is_pressed);
}

//////////////////////////////////////////////////////////
// Functions to handle shifted-to-unshifted translation, i.e. a shifted
// keycode is translated to a different unshifted keycode.
#define USSUT_KC_FIRST_INDEX   (get_ust_kc_index(USSUT_KC_FIRST))
#define USSUT_KC_LAST_INDEX    (get_ust_kc_index(USSUT_KC_LAST ))
bool ussut_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed) {
    if (r_alt_is_pressed()) {
        return true;
    }

    if (!ust_kc_is_pressed_any(USSUT_KC_FIRST_INDEX, USSUT_KC_LAST_INDEX)) {
        // no USSUT_KC key is pressed, nothing to do here.
        return true;
    }

    if (is_pressed) {
        del_mods(MOD_MASK_SHIFT);
        EOA_FOR_RANGE(uint8_t, index, USSUT_KC_FIRST_INDEX, USSUT_KC_LAST_INDEX) {

            if (ust_kc_is_pressed_on(index)) {
                const uint16_t shift_kc = ust_kc_shifted[index][1];
                register_code(shift_kc);
            }
        }
        return false;
    }

    set_mods(MOD_MASK_SHIFT);
    EOA_FOR_RANGE(uint8_t, index, USSUT_KC_FIRST_INDEX, USSUT_KC_LAST_INDEX) {
        if (ust_kc_is_pressed_on(index)) {
            const uint16_t plain_kc = ust_kc_shifted[index][0];
            const uint16_t shift_kc = ust_kc_shifted[index][1];
            unregister_code(shift_kc);
            register_code(plain_kc);
        }
    }
    del_mods(MOD_MASK_SHIFT);

    return false;
}

bool ussut_kc_operate_on_altgr_keycode(const uint8_t mod_state,
                                       const uint16_t keycode,
                                       const bool is_pressed) {
    return uusst_kc_operate_on_altgr_keycode(mod_state, keycode, is_pressed);
}

bool ussut_kc_operate_on_shift_keycode(const uint8_t mod_state,
                                       const uint16_t keycode,
                                       const bool is_pressed) {
    const uint8_t index = get_ust_kc_index(keycode);
    const uint16_t plain_kc = ust_kc_shifted[index][0];
    const uint16_t shift_kc = ust_kc_shifted[index][1];

    if (is_pressed) {
        ust_kc_is_pressed_set_on(index);
        if (any_shift_is_pressed()) {
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

    ust_kc_is_pressed_unset_on(index);
    unregister_code(shift_kc);
    unregister_code(plain_kc);
    if (!l_shift_is_pressed()) { unregister_code(KC_LSFT); }
    if (!r_shift_is_pressed()) { unregister_code(KC_RSFT); }
    return true;
}

bool ussut_kc_operate_on_keycode(const uint8_t mod_state,
                                 const uint16_t keycode,
                                 const bool is_pressed) {
    if (r_alt_is_pressed()) {
        if (any_shift_is_pressed()) {
            // alt+shift does not work in this case.
            return true;
        }

        return ussut_kc_operate_on_altgr_keycode(mod_state, keycode, is_pressed);
    }

    return ussut_kc_operate_on_shift_keycode(mod_state, keycode, is_pressed);
}

//////////////////////////////////////////////////////////
//
bool ust_kc_operate_on_altgr(const uint8_t mod_state,
                             const uint16_t keycode,
                             const bool is_pressed) {
    if (any_shift_is_pressed()) {
        return true;
    }

    bool has_any = false;
    EOA_FOR_RANGE(uint8_t, i, 0, sizeof(ust_kc_altgrd_indices)) {
        const uint8_t index = ust_kc_altgrd_indices[i];
        if (ust_kc_is_pressed_on(index)) {
            has_any = true;
            break;
        }
    }
    if (!has_any) {
        return true;
    }

    del_mods(MOD_BIT(KC_RALT));

    EOA_FOR_RANGE(uint8_t, i, 0, (sizeof(ust_kc_altgrd_indices) - 1)) {
        const uint8_t index = ust_kc_altgrd_indices[i];
        const uint16_t plain_kc = ust_kc_shifted[index][0];
        const uint16_t altgr_kc = ust_kc_shifted[index][2];
        if (!ust_kc_is_pressed_on(index)) {
            continue;
        }

        if (is_pressed) {
            const bool is_shifted = internal_get_bit(ust_kc_altgrd, index);
            if (is_shifted) {
                set_mods(MOD_MASK_SHIFT);
                register_code(altgr_kc);
                del_mods(MOD_MASK_SHIFT);
                continue;
            }

            register_code(altgr_kc);
            continue;
        }

        unregister_code(altgr_kc);
        unregister_code(plain_kc);
    }

    set_mods(mod_state);

    return false;
}

