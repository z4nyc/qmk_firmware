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

static uint8_t ust_kc_shifted[UST_KC_COUNT][2] = {
    /* begin UUSST block*/
    /* UUSST_KC_2       */  { KC_2      , KC_QUOT,  }, // 2"@ ... as in ES-LA keymap.
    /* UUSST_KC_6       */  { KC_6      , KC_7,     }, // 6&  ... as in ES-LA keymap.
    /* UUSST_KC_8       */  { KC_8      , KC_9,     }, // 8(  ... as in ES-LA keymap.
    /* UUSST_KC_9       */  { KC_9      , KC_0,     }, // 9)  ... as in ES-LA keymap.
    /* UUSST_KC_QUOT    */  { KC_QUOT   , KC_SLSH,  }, // '?\ ... as in ES-LA keymap.
    /* UUSST_KC_DOT     */  { KC_DOT    , KC_SCLN,  }, // .:  ... as in ES-LA keymap.
    /* UUSST_KC_NTIL    */  { KC_N      , KC_N,     }, // ñÑ  ... as in ES-LA keymap.
    /* UUSST_KC_Q       */  { KC_DOT    , KC_SCLN,  }, // qQ@ ... as in ES-LA keymap.
    /* UUSST_KC_ACUT    */  { KC_QUOT   , KC_QUOT,  }, // ´¨  ... as in ES-LA keymap.
    /* end UUSST block  */


    /* begin UUSUT block*/
    /* UUSUT_KC_7       */  { KC_7      , KC_SLSH,  }, // 7/  ... as in ES-LA keymap.
    /* UUSUT_KC_0       */  { KC_0      , KC_EQL,   }, // 0=  ... as in ES-LA keymap.
    /* UUSUT_KC_COMM    */  { KC_COMM   , KC_SCLN,  }, // ,;  ... as in ES-LA keymap.
    /* UUSUT_KC_BSDL    */  { KC_BSPC   , KC_DEL,   }, // backspace, del
    /* end UUSUT block  */


    /* begin UUSST block*/
    /* USSST_KC_LABK    */  { KC_COMM   , KC_DOT,   }, // <>  ... as in ES-LA keymap.
    /* USSST_KC_PLUS    */  { KC_EQL    , KC_8,     }, // +*  ... as in ES-LA keymap.
    /* USSST_KC_IQUE    */  { KC_SLSH   , KC_1,     }, // ¿¡  ... as in ES-LA keymap.
    /* end UUSST block  */


    /* begin USSUT block*/
    /* USSUT_KC_LCBR    */  { KC_LBRC   , KC_LBRC,  }, // {[  ... as in ES-LA keymap.
    /* USSUT_KC_RCBR    */  { KC_RBRC   , KC_RBRC,  }, // }]  ... as in ES-LA keymap.
    /* USSUT_KC_PIPE    */  { KC_BSLS   , KC_GRV,   }, // |°¬ ... as in ES-LA keymap.
    /* end USSUT block  */
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Flags needed to mark each UST keycode as pressed, if so.
// Additionally we need two additional flags for the two SHIFT keycodes.

// Two flags for KC_LSFT, KC_RSFT
#define UST_SHIFT_FLAG_COUNT        2
// Total flags: UST keycode one's + shift one's.
#define UST_TOTAL_FLAG_COUNT        (UST_KC_COUNT + UST_SHIFT_FLAG_COUNT)
// Number of uint8_t needed for UST_TOTAL_FLAG_COUNT.
#define UST_UINT8_FLAG_VECTOR_SIZE  (1 + ((UST_TOTAL_FLAG_COUNT - 1) / 8))

// TODO: Enhance macro REP so it allows passing a macro-defined value with the
//       same complexity as UST_UINT8_FLAG_VECTOR_SIZE.
#define UST_UINT8_FLAG_VECTOR_SIZE_EQUIV    3
static uint8_t ust_kc_is_pressed[UST_UINT8_FLAG_VECTOR_SIZE] = {
    REP(UST_UINT8_FLAG_VECTOR_SIZE_EQUIV, 0)
};

void ust_show_flags(const char* const msg) {
#ifdef CONSOLE_ENABLE
    dprintf("%s: ", msg);
    for (uint8_t i = UST_UINT8_FLAG_VECTOR_SIZE; i > 0; --i) {
        dprintf(BYTE_TO_BINARY_PATTERN " ", BYTE_TO_BINARY(ust_kc_is_pressed[i - 1]));
    }
    dprintf("\n");
#endif
}

uint8_t get_ust_kc_index(const uint16_t keycode) {
    return keycode - UST_KC_FIRST;
}

bool ust_kc_is_pressed_on(const uint8_t index) {
    const uint8_t arr_index = index / 8;
    const uint8_t bit_index = index % 8;
    return (ust_kc_is_pressed[arr_index] & (1 << bit_index)) ? true : false;
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
// Shift-key status.

// KC_LSFT flag index
#define UST_LSFT_INDEX              UST_KC_COUNT
// KC_RSFT flag index
#define UST_RSFT_INDEX              (1 + UST_KC_COUNT)

void mark_shift_pressed(const uint16_t keycode,
                        const bool is_pressed) {
    const uint8_t index = (keycode == KC_LSFT) ? UST_LSFT_INDEX :
                                                 UST_RSFT_INDEX;
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

bool uusst_kc_operate_on_keycode(const uint8_t mod_state,
                              const uint16_t keycode,
                              const bool is_pressed) {
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    if (is_altgr && is_shift) {
        // alt+shift does not work in this case.
        return true;
    }

    const uint8_t index = get_ust_kc_index(keycode);
    const uint16_t plain_kc = ust_kc_shifted[index][0];
    const uint16_t shift_kc = ust_kc_shifted[index][1];

    if (is_pressed) {
        ust_kc_is_pressed_set_on(index);
        if (is_shift) {
            register_code(shift_kc);
            return true;
        }

        register_code(plain_kc);
        return true;
    }

    ust_kc_is_pressed_unset_on(index);
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
#define UUSUT_KC_FIRST_INDEX   (get_ust_kc_index(UUSUT_KC_FIRST))
#define UUSUT_KC_LAST_INDEX    (get_ust_kc_index(UUSUT_KC_LAST ))
bool uusut_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed) {
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

// This function is strongly based on the procedure to emit KC_DEL when
// shift + KC_BSPC is pressed in the documentation.
// https://docs.qmk.fm/#/feature_advanced_keycodes?id=shift-backspace-for-delete
bool uusut_kc_operate_on_keycode(const uint8_t mod_state,
                                 const uint16_t keycode,
                                 const bool is_pressed) {
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    if (is_altgr && is_shift) {
        // alt+shift does not work in this case.
        return true;
    }

    const uint8_t index = get_ust_kc_index(keycode);
    const uint16_t plain_kc = ust_kc_shifted[index][0];
    const uint16_t shift_kc = ust_kc_shifted[index][1];

    if (is_pressed) {
        ust_kc_is_pressed_set_on(index);
        if (is_shift) {
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

//////////////////////////////////////////////////////////
// Functions to handle shifted-to-unshifted translation, i.e. a shifted
// keycode is translated to a different unshifted keycode.
#define USSST_KC_FIRST_INDEX   (get_ust_kc_index(USSST_KC_FIRST))
#define USSST_KC_LAST_INDEX    (get_ust_kc_index(USSST_KC_LAST ))
bool ussst_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed) {
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

bool ussst_kc_operate_on_keycode(const uint8_t mod_state,
                              const uint16_t keycode,
                              const bool is_pressed) {
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    if (is_altgr && is_shift) {
        // alt+shift does not work in this case.
        return true;
    }

    const uint8_t index = get_ust_kc_index(keycode);
    const uint16_t plain_kc = ust_kc_shifted[index][0];
    const uint16_t shift_kc = ust_kc_shifted[index][1];

    if (is_pressed) {
        ust_kc_is_pressed_set_on(index);
        if (is_shift) {
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

//////////////////////////////////////////////////////////
// Functions to handle shifted-to-unshifted translation, i.e. a shifted
// keycode is translated to a different unshifted keycode.
#define USSUT_KC_FIRST_INDEX   (get_ust_kc_index(USSUT_KC_FIRST))
#define USSUT_KC_LAST_INDEX    (get_ust_kc_index(USSUT_KC_LAST ))
bool ussut_kc_operate_on_shift(const uint8_t mod_state,
                               const uint16_t keycode,
                               const bool is_pressed) {
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

bool ussut_kc_operate_on_keycode(const uint8_t mod_state,
                              const uint16_t keycode,
                              const bool is_pressed) {
    const bool is_shift = (mod_state & MOD_MASK_SHIFT) ? true : false;
    const bool is_altgr = (mod_state & MOD_BIT(KC_RALT)) ? true : false;

    if (is_altgr && is_shift) {
        // alt+shift does not work in this case.
        return true;
    }

    const uint8_t index = get_ust_kc_index(keycode);
    const uint16_t plain_kc = ust_kc_shifted[index][0];
    const uint16_t shift_kc = ust_kc_shifted[index][1];

    if (is_pressed) {
        ust_kc_is_pressed_set_on(index);
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

    ust_kc_is_pressed_unset_on(index);
    unregister_code(shift_kc);
    unregister_code(plain_kc);
    if (!l_shift_is_pressed()) { unregister_code(KC_LSFT); }
    if (!r_shift_is_pressed()) { unregister_code(KC_RSFT); }
    return true;
}

