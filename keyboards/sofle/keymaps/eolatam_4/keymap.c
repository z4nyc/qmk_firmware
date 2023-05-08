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
#include "local_general.h"
#include "local_init.h"
#include "local_layers.h"
#include "local_oled.h"

#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, current_raise_layer, _ADJUST);
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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

    local_init_raise_layer();

#ifdef CONSOLE_ENABLE
    debug_enable=true;
    debug_matrix=true;
    debug_keyboard=true;
    debug_mouse=true;
#endif
}

#if 0
#define MASK_LAYERS_ALL (~0)
#define MASK_LAYERS_US_ ((1 << _EN_US) | (1 << _RAISE_EN_US))
#define MASK_LAYERS_LA_ ((1 << _ES_LA) | (1 << _RAISE_ES_LA))
#define EO_MODS_NONE_      0
#define EO_MODS_LRSFT      MOD_MASK_SHIFT
#define EO_MODS_LRALT      MOD_MASK_ALT
#define EO_MODS__LALT      MOD_BIT(KC_LALT)
#define EO_MODS__RALT      MOD_BIT(KC_RALT)
#define EO_MODS_AS_LA      (MOD_BIT(KC_LALT) | MOD_MASK_SHIFT)
#define EO_MODS__LGUI      MOD_BIT(KC_LGUI)
#define EO_MODS__RGUI      MOD_BIT(KC_RGUI)
#define EO_MODS_AS_LG      (MOD_BIT(KC_LGUI) | MOD_MASK_SHIFT)
#define eo_make_ko(mods, key, rep, layers) ko_make_with_layers_and_negmods(mods, key, rep, layers, ~mods);

const key_override_t ___ba_pipe_to_s__kc_bsls = eo_make_ko(EO_MODS_NONE_, BA_PIPE, S(KC_BSLS), MASK_LAYERS_US_);
const key_override_t s__ba_pipe_to____kc_grv_ = eo_make_ko(EO_MODS_LRSFT, BA_PIPE,   KC_GRV  , MASK_LAYERS_US_);
const key_override_t a__ba_pipe_to____kc_grv_ = eo_make_ko(EO_MODS__RALT, BA_PIPE,   KC_GRV  , MASK_LAYERS_US_);

const key_override_t s__kc_2____to_s__kc_quot = eo_make_ko(EO_MODS_LRSFT, KC_2   , S(KC_QUOT), MASK_LAYERS_US_);
const key_override_t a__kc_2____to_s__kc_2___ = eo_make_ko(EO_MODS__RALT, KC_2   , S(KC_2)   , MASK_LAYERS_US_);
const key_override_t a__es_2____to____es_at__ = eo_make_ko(EO_MODS__RALT, ES_2   ,   KC_AT   , MASK_LAYERS_LA_);

const key_override_t a__kc_4____to_a__kc_f4__ = eo_make_ko(EO_MODS_LRALT, KC_4   , A(KC_F4)  , MASK_LAYERS_ALL);
const key_override_t s__kc_6____to_s__kc_7___ = eo_make_ko(EO_MODS_LRSFT, KC_6   , S(KC_7)   , MASK_LAYERS_US_);
const key_override_t s__kc_7____to____kc_slsh = eo_make_ko(EO_MODS_LRSFT, KC_7   ,   KC_SLSH , MASK_LAYERS_US_);
const key_override_t s__kc_8____to_s__kc_9___ = eo_make_ko(EO_MODS_LRSFT, KC_8   , S(KC_9)   , MASK_LAYERS_US_);
const key_override_t s__kc_9____to_s__kc_0___ = eo_make_ko(EO_MODS_LRSFT, KC_9   , S(KC_0)   , MASK_LAYERS_US_);
const key_override_t s__kc_0____to____kc_eql_ = eo_make_ko(EO_MODS_LRSFT, KC_0   ,   KC_EQL  , MASK_LAYERS_US_);

const key_override_t s__kc_quot_to_s__kc_slsh = eo_make_ko(EO_MODS_LRSFT, KC_QUOT, S(KC_SLSH), MASK_LAYERS_US_);
const key_override_t a__kc_quot_to_s__kc_bsls = eo_make_ko(EO_MODS__RALT, KC_QUOT,   KC_BSLS , MASK_LAYERS_US_);

const key_override_t ___bb_ique_to_s__kc_slsh = eo_make_ko(EO_MODS_NONE_, BB_IQUE, S(KC_C   ), MASK_LAYERS_US_);
const key_override_t s__bb_ique_to_s__kc_1___ = eo_make_ko(EO_MODS_LRSFT, BB_IQUE, S(KC_1)   , MASK_LAYERS_US_);

const key_override_t a__kc_q____to_s__kc_2___ = eo_make_ko(EO_MODS__RALT, KC_Q   , S(KC_2)   , MASK_LAYERS_US_);

const key_override_t ___ab_acut_to____kc_quot = eo_make_ko(EO_MODS_NONE_, AB_ACUT,   KC_QUOT , MASK_LAYERS_US_);
const key_override_t s__ab_acut_to_s__kc_quot = eo_make_ko(EO_MODS_LRSFT, AB_ACUT, S(KC_QUOT), MASK_LAYERS_US_);

const key_override_t ___bb_acut_to____kc_quot = eo_make_ko(EO_MODS_NONE_, BB_PLUS, S(KC_EQL) , MASK_LAYERS_US_);
const key_override_t s__bb_acut_to_s__kc_quot = eo_make_ko(EO_MODS_LRSFT, BB_PLUS, S(KC_8)   , MASK_LAYERS_US_);
const key_override_t a__bb_acut_to_s__kc_quot = eo_make_ko(EO_MODS__RALT, BB_PLUS, S(KC_GRV) , MASK_LAYERS_US_);

//const key_override_t a__kc_z____to_s__kc_comm = eo_make_ko(EO_MODS__LGUI, KC_Z   , S(KC_COMM), MASK_LAYERS_US_);
const key_override_t a__kc_z____to_s__kc_comm = {
    .trigger = KC_Z,
    .trigger_mods = EO_MODS__LGUI,
    .layers = MASK_LAYERS_US_,
    .negative_mod_mask = ~EO_MODS__LGUI,
    .suppressed_mods = EO_MODS__LGUI,
    .replacement = S(KC_COMM),
    .options = ko_options_default,
    .context = NULL,
    .enabled = NULL
};
//const key_override_t sa_kc_z____to_s__kc_dot_ = eo_make_ko(EO_MODS_AS_LG, KC_Z   , S(KC_DOT) , MASK_LAYERS_US_);
const key_override_t sa_kc_z____to_s__kc_dot_ = {
    .trigger = KC_Z,
    .trigger_mods = EO_MODS_AS_LG,
    .layers = MASK_LAYERS_US_,
    .negative_mod_mask = ~EO_MODS_AS_LG,
    .suppressed_mods = EO_MODS__LGUI,
    .replacement = S(KC_COMM),
    .options = ko_options_default,
    .context = NULL,
    .enabled = NULL
};

//const key_override_t a__es_z____to____kc_labk = eo_make_ko(EO_MODS__LGUI, ES_Z   ,   ES_LABK , MASK_LAYERS_LA_);
const key_override_t a__es_z____to____kc_labk = {
    .trigger = ES_Z,
    .trigger_mods = EO_MODS__LGUI,
    .layers = MASK_LAYERS_LA_,
    .negative_mod_mask = ~EO_MODS__LGUI,
    .suppressed_mods = EO_MODS__LGUI,
    .replacement = ES_LABK,
    .options = ko_options_default,
    .context = NULL,
    .enabled = NULL
};
//const key_override_t sa_es_z____to_s__kc_labk = eo_make_ko(EO_MODS_AS_LG, ES_Z   , S(ES_LABK), MASK_LAYERS_LA_);
const key_override_t sa_es_z____to_s__kc_labk = {
    .trigger = ES_Z,
    .trigger_mods = EO_MODS_AS_LG,
    .layers = MASK_LAYERS_LA_,
    .negative_mod_mask = ~EO_MODS_AS_LG,
    .suppressed_mods = EO_MODS__LGUI,
    .replacement = S(ES_LABK),
    .options = ko_options_default,
    .context = NULL,
    .enabled = NULL
};

const key_override_t ___bb_labk_to_s__kc_comm = eo_make_ko(EO_MODS_NONE_, BB_LABK, S(KC_COMM), MASK_LAYERS_US_);
const key_override_t s__bb_labk_to_s__kc_dot_ = eo_make_ko(EO_MODS_LRSFT, BB_LABK, S(KC_DOT) , MASK_LAYERS_US_);

const key_override_t ___ba_lcbr_to_s__kc_lbrc = eo_make_ko(EO_MODS_NONE_, BA_LCBR, S(KC_LBRC), MASK_LAYERS_US_);
const key_override_t s__ba_lcbr_to_s__kc_lbrc = eo_make_ko(EO_MODS_LRSFT, BA_LCBR,   KC_LBRC , MASK_LAYERS_US_);
const key_override_t a__ba_lcbr_to_s__kc_6___ = eo_make_ko(EO_MODS__RALT, BA_LCBR, S(KC_6)   , MASK_LAYERS_US_);

const key_override_t ___ba_rcbr_to_s__kc_rbrc = eo_make_ko(EO_MODS_NONE_, BA_RCBR, S(KC_RBRC), MASK_LAYERS_US_);
const key_override_t s__ba_rcbr_to_s__kc_rbrc = eo_make_ko(EO_MODS_LRSFT, BA_RCBR,   KC_RBRC , MASK_LAYERS_US_);
const key_override_t a__ba_rcbr_to____kc_grv_ = eo_make_ko(EO_MODS__RALT, BA_RCBR,   KC_GRV  , MASK_LAYERS_US_);

const key_override_t s__kc_comm_to____kc_scln = eo_make_ko(EO_MODS_LRSFT, KC_COMM,   KC_SCLN , MASK_LAYERS_US_);
const key_override_t s__kc_dot__to_s__kc_scln = eo_make_ko(EO_MODS_LRSFT, KC_DOT , S(KC_SCLN), MASK_LAYERS_US_);


const key_override_t **key_overrides = (const key_override_t*[]){
    /*  0 */    &___ba_pipe_to_s__kc_bsls,
    /*  1 */    &s__ba_pipe_to____kc_grv_,
    /*  2 */    &a__ba_pipe_to____kc_grv_,

    /*  3 */    &s__kc_2____to_s__kc_quot,
    /*  4 */    &a__kc_2____to_s__kc_2___,
    /*  5 */    &a__es_2____to____es_at__,

    /*  6 */    &a__kc_4____to_a__kc_f4__,
    /*  7 */    &s__kc_6____to_s__kc_7___,
    /*  8 */    &s__kc_7____to____kc_slsh,
    /*  9 */    &s__kc_8____to_s__kc_9___,
    /* 10 */    &s__kc_9____to_s__kc_0___,
    /* 11 */    &s__kc_0____to____kc_eql_,

    /* 12 */    &s__kc_quot_to_s__kc_slsh,
    /* 13 */    &a__kc_quot_to_s__kc_bsls,

    /* 14 */    &___bb_ique_to_s__kc_slsh,
    /* 15 */    &s__bb_ique_to_s__kc_1___,

    /* 16 */    &a__kc_q____to_s__kc_2___,

    /* 17 */    &___ab_acut_to____kc_quot,
    /* 18 */    &s__ab_acut_to_s__kc_quot,

    /* 19 */    &___bb_acut_to____kc_quot,
    /* 20 */    &s__bb_acut_to_s__kc_quot,
    /* 21 */    &a__bb_acut_to_s__kc_quot,

    /* 22 */    //&a__kc_z____to_s__kc_comm,
    /* 23 */    //&sa_kc_z____to_s__kc_dot_,

    /* 24 */    //&a__es_z____to____kc_labk,
    /* 25 */    //&sa_es_z____to_s__kc_labk,

    /* 26 */    &___bb_labk_to_s__kc_comm,
    /* 27 */    &s__bb_labk_to_s__kc_dot_,

    /* 28 */    &___ba_lcbr_to_s__kc_lbrc,
    /* 29 */    &s__ba_lcbr_to_s__kc_lbrc,
    /* 30 */    &a__ba_lcbr_to_s__kc_6___,

    /* 31 */    &___ba_rcbr_to_s__kc_rbrc,
    /* 32 */    &s__ba_rcbr_to_s__kc_rbrc,
    /* 33 */    &a__ba_rcbr_to____kc_grv_,

    /* 34 */    &s__kc_comm_to____kc_scln,
    /* 35 */    &s__kc_dot__to_s__kc_scln,

    NULL
};

#endif

