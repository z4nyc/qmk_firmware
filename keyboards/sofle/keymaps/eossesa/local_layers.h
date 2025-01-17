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

#include "keymap_spanish.h"
#include "sendstring_spanish.h"

// Layer IDs.
enum sofle_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _ES_LA, // std layer
    _EN_US = _ES_LA,
    _NUMPAD,
    _LOWER,
    _RAISE,
    _ADJUST,
};

// Keycodes associated with layers, mapped 1:1 to values in sofle layers.
enum layer_keycodes {
    LAYER_KC_FIRST = SAFE_RANGE,
    LAYER_KC_ES_LA = LAYER_KC_FIRST,
    LAYER_KC_EN_US = LAYER_KC_FIRST,
    LAYER_KC_NUMPAD,
    LAYER_KC_LOWER,
    LAYER_KC_RAISE,
    LAYER_KC_ADJUST,

    // Special layer keycode, if used will return to the layer before the numpad.
    LAYER_KC_NUMPAD_CALLER,

    LAYER_KC_POST_LAST,
    LAYER_KC_LAST = LAYER_KC_POST_LAST - 1,
};

// Fake es-la key keycodes.
// If OS keymap is EN-US, these fake keycodes are mapped to keycodes that mimic the ES-LA keymap.
// These keycodes make sense only in the _EN_US keymap.
enum fake_es_keycodes {
    F_ES_KC_FIRST = LAYER_KC_POST_LAST,
    F_ES_0 = F_ES_KC_FIRST,
    F_ES_1,
    F_ES_2,
    F_ES_3,
    F_ES_4,
    F_ES_5,
    F_ES_6,
    F_ES_7,
    F_ES_8,
    F_ES_9,
    F_ES_COMM,
    F_ES_DOT,
    F_ES_Q,

    F_ES_KC_POST_LAST,
    F_ES_KC_LAST = F_ES_KC_POST_LAST - 1
};

// Fake keycodes, independent of the layer.
enum fake_common_keycodes {
    F_CM_KC_FIRST = F_ES_KC_POST_LAST,
    F_CM_BSPS = F_CM_KC_FIRST,
    F_CM_LEFT,
    F_CM_RGHT,

    F_CM_KC_POST_LAST,
    F_CM_KC_LAST = F_CM_KC_POST_LAST - 1
};

// Fake keycodes, keymap specific.
// If in layer _EN_US, will issue a symbol consistent with an ES-LA keymap.
enum fake_keymap_specific_keycodes {
    F_KS_KC_FIRST = F_CM_KC_POST_LAST,
    F_KS_LT = F_KS_KC_FIRST,
    F_KS_QUOT,
    F_KS_ACUT,
    F_KS_PLUS,
    F_KS_LCUB,
    F_KS_RCUB,

    F_KS_KC_POST_LAST,
    F_KS_KC_LAST = F_KS_KC_POST_LAST - 1
};

#define FAKE_KC_COUNT (F_KS_KC_POST_LAST - F_ES_KC_FIRST)

// Fake keycodes, issuing a symbols from alt sequences with send_string.
enum fake_senstring_keycodes {
    F_SS_KC_FIRST = F_KS_KC_POST_LAST,
    F_SS_NTILDE = F_SS_KC_FIRST,
    F_SS_INV_QUESTION,
    F_SS_BAR,

    F_SS_KC_POST_LAST,
    F_SS_KC_LAST = F_SS_KC_POST_LAST - 1,
};

const char* const uc_layer_names[] = {/*"ES-LA",*/ "EN-US", "NUMLK", "LOWER", "RAISE", "ADJST"};
const char* const lc_layer_names[] = {/*"es-la",*/ "en-us", "numlk", "lower", "raise", "adjst"};
const char uc_undef[] = "UNDEF";
const char lc_undef[] = "undef";


const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
#if 0
/*
 * QWERTY ES_LA: to run with es-la keymap
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Esc  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |BS/DEL|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | ACUT |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Caps |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |  ñÑ  |  {   |
 * |------+------+------+------+------+------| MPLAY |    | MUTE  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |  ,;  |  .;  |  -_  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LCTR | LGUI | LAlt |LOWER | /Space  /       \ Enter\  |RAISE | RAlt | RGUI | RCTR |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_ES_LA] = LAYOUT(
  QK_GESC,   ES_1,   ES_2,    ES_3,    ES_4,    ES_5,                        ES_6,    ES_7,    ES_8,    ES_9,    ES_0,  F_CM_BSPS,
   KC_TAB,   ES_Q,   ES_W,    ES_E,    ES_R,    ES_T,                        ES_Y,    ES_U,    ES_I,    ES_O,    ES_P,  KC_LBRC,
  KC_CAPS,   ES_A,   ES_S,    ES_D,    ES_F,    ES_G,                        ES_H,    ES_J,    ES_K,    ES_L, ES_NTIL,  ES_ACUT,
  KC_LSFT,   ES_Z,   ES_X,    ES_C,    ES_V,    ES_B, KC_MPLY,      KC_MUTE, ES_N,    ES_M, ES_COMM,  ES_DOT, ES_MINS,  KC_RSFT,
                  KC_LCTL, KC_LGUI, KC_LALT, MO(_LOWER), KC_SPC,      KC_ENT,  MO(_RAISE), KC_RALT, KC_RGUI, KC_RCTL
),
#endif
/*
 * QWERTY EN_US: symbols mapped to their position on a ES_LA keymap.
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Esc  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |BS/DEL|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | ´¨   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Caps |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |  ñÑ  | {[^  |
 * |------+------+------+------+------+------| MPLAY |    | MUTE  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |  ,;  |  .;  |  -_  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LCTR | LGUI | LAlt |LOWER | /Space  /       \ Enter\  |RAISE | RAlt | RGUI | RCTR |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_EN_US] = LAYOUT(
  KC_ESC, F_ES_1, F_ES_2,  F_ES_3,  F_ES_4,  F_ES_5,                      F_ES_6,  F_ES_7,    F_ES_8,    F_ES_9,      F_ES_0,  F_CM_BSPS,
   KC_TAB, F_ES_Q,   KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,    KC_U,      KC_I,      KC_O,        KC_P,  F_KS_ACUT,
  KC_CAPS,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                        KC_H,    KC_J,      KC_K,      KC_L, F_SS_NTILDE,  F_KS_LCUB,
  KC_LSFT,   KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MPLY,      KC_MUTE, KC_N,    KC_M, F_ES_COMM,  F_ES_DOT,    KC_MINUS,  KC_RSFT,
                  KC_LCTL, KC_LGUI, KC_LALT, MO(_LOWER), KC_SPC,      KC_ENT,  MO(_RAISE), KC_RALT, KC_RGUI, KC_RCTL
),

/*
 * NUMPAD ...
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Esc  |      |      |      |      |      |                    | NLCK |   /  |   *  |   -  |      |BS/DEL|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |      |      |      |      |      |                    |   7  |   8  |   9  |   +  |      | DEL  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Caps |      |      |      |      |      |-------.    ,-------|   4  |   5  |   6  |   +  |      |      |
 * |------+------+------+------+------+------| MPLAY |    | MUTE  |------+------+------+------+------+------|
 * |LShift|  <>  |      |      |      |      |-------|    |-------|   1  |   2  |   3  | ENT  |      |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LCTR | LGUI | LAlt |LOWER | /Space  /       \ Enter\  |RAISE |  0   |  .   | RCTR |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_NUMPAD] = LAYOUT(
  KC_ESC, XXXXXXX, LAYER_KC_ES_LA, XXXXXXX, XXXXXXX, XXXXXXX,         LAYER_KC_NUMPAD_CALLER, KC_PSLS, KC_PAST, KC_PMNS, XXXXXXX, F_CM_BSPS,
  KC_TAB, XXXXXXX, LAYER_KC_EN_US, XXXXXXX, XXXXXXX, XXXXXXX,                          KC_P7,   KC_P8,   KC_P9, KC_PPLS, XXXXXXX, KC_DEL,
  KC_CAPS,XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                          KC_P4,   KC_P5,   KC_P6, KC_PPLS, XXXXXXX, XXXXXXX,
  KC_LSFT,F_KS_LT,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_MPLY,    KC_MUTE,   KC_P1,   KC_P2,   KC_P3, KC_PENT, XXXXXXX, KC_RSFT,
                          KC_LCTL, KC_LGUI, KC_LALT, MO(_LOWER), KC_SPC,     KC_ENT,   KC_P0,   KC_P0, KC_PDOT,  KC_RCTL
),

/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   !  |   @  |   #  |   $  |   %  |-------.    ,-------|   ^  |   &  |   *  |   (  |   )  |   |  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * | Shift|  =   |  -   |  +   |   {  |   }  |-------|    |-------|   [  |   ]  |   ;  |   :  |   \  | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_LOWER] = LAYOUT(
  _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_F12,
  _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
  _______,  KC_EQL, KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______,       _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
                       _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),

/* RAISE
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |  |°¬ |      |      |      |      |      |                    | NLOCK|      |      |      |  '?\ |  ¿¡  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |      |      |      |      |      |                    | Ins  | PgUp |  Up  | PgDn |  '¨  |  +*~ |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Caps |      |      |      |      |      |-------.    ,-------| Del  | Left | Down | Rigth|  {[^ |  }]` |
 * |------+------+------+------+------+------| MPLAY |    |  MUTE |------+------+------+------+------+------|
 * |Shift |  <>  |      |      |      |      |-------|    |-------|      |      |      |      |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_RAISE] = LAYOUT(
  F_SS_BAR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                LAYER_KC_NUMPAD,   XXXXXXX, XXXXXXX, XXXXXXX, F_KS_QUOT, F_SS_INV_QUESTION,
     _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                         KC_INS,   KC_PGUP,   KC_UP, KC_PGDN, F_KS_ACUT, F_KS_PLUS,
     _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                         KC_DEL,   KC_LEFT, KC_DOWN, KC_RGHT, F_KS_LCUB, F_KS_RCUB,
     _______, F_KS_LT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  _______,      _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,
                        _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      | ES-LA|      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | QK_BOOT|    | EN-US|      |MACWIN|      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |MACWIN|      |      |      |-------.    ,-------|      | VOLDO| MUTE | VOLUP|      |      |
 * |------+------+------+------+------+------| MPLAY |    |  MUTE |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      | PREV | PLAY | NEXT |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
  [_ADJUST] = LAYOUT(
  XXXXXXX, XXXXXXX, LAYER_KC_ES_LA, XXXXXXX, XXXXXXX, XXXXXXX,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  QK_BOOT, XXXXXXX, LAYER_KC_EN_US, XXXXXXX, CG_TOGG, XXXXXXX,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, CG_TOGG       , XXXXXXX, XXXXXXX, XXXXXXX,                        XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX       , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                           _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
  )
};
// clang-format on
