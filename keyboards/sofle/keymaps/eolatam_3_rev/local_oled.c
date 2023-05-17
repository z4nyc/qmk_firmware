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

#ifdef OLED_ENABLE

#include QMK_KEYBOARD_H

#include "local_init.h"
#include "local_general.h"
#include "local_oled.h"

static uint8_t left_pos = 2;

#define ARROW_SIZE 32
#define ARROW_TYPE_STEP 3
#define ARROW_TIP_START (1 + ARROW_TYPE_STEP*4)
static char arrow[ARROW_SIZE];

void create_arrow(void) {
    // creates an arrow in 32 bits.
    arrow[ 0] = 0b00000000;
    arrow[ 1] = 0b00011000;
    arrow[ 2] = 0b00011000;
    arrow[ 3] = 0b00011000;
    arrow[ 4] = 0b00111100;
    arrow[ 5] = 0b00111100;
    arrow[ 6] = 0b00111100;
    arrow[ 7] = 0b01111110;

    arrow[ 8] = 0b01111110;
    arrow[ 9] = 0b01111110;
    arrow[10] = 0b11111111;
    arrow[11] = 0b11111111;
    arrow[12] = 0b11111111;
    arrow[13] = 0b11111111;
    for (uint8_t i = 12; i < 30; ++i) {
        arrow[i] = 0b00111100;
    }
    arrow[30] = 0b00111100;
    arrow[31] = 0b00111100;

    if (!my_local_status._is_master) {
        for (uint8_t i = (1 + ARROW_TIP_START); i < (ARROW_SIZE - 2); ++i) {
            arrow[i] &= 0b11100111;
        }
    }

    if (!my_local_status._is_left) {
        for (uint8_t i = 0; i < (ARROW_SIZE / 2); ++i) {
            uint8_t tmp = arrow[i];
            arrow[i] = arrow[ARROW_SIZE - 1 - i];
            arrow[ARROW_SIZE - 1 - i] = tmp;
        }
    }
}

void print_arrow(void) {
    oled_write_raw(arrow, sizeof(arrow));
}

static char lock_space[] = {
    0b11111111,
    0b11111111,
    0b00000011,
    0b00000011,
    0b00000011,
    0b00000011,
    0b00000011,
    0b00000011,

    0b00000011,
    0b00000011,
    0b00000011,
    0b00000011,
    0b00000011,
    0b00000011,
    0b11111111,
    0b11111111,

    0b11111111,
    0b11111111,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
        
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11111111,
    0b11111111,

};

led_t led_usb_state;

static void clear_lock_space(void) {
    for (uint8_t i = 2; i < 14; ++i) {
        lock_space[i] &= 0x03;
    }
    for (uint8_t i = 18; i < 30; ++i) {
        lock_space[i] &= 0xc0;
    }
}

static void write_N(void) {
    lock_space[ 4] |= 0b11110000;
    lock_space[ 5] |= 0b11110000;
    lock_space[ 6] |= 0b11100000;
    lock_space[ 7] |= 0b11000000;

    lock_space[ 8] |= 0b10000000;
    lock_space[ 9] |= 0b00000000;
    lock_space[10] |= 0b11110000;
    lock_space[11] |= 0b11110000;

    lock_space[20] |= 0b00001111;
    lock_space[21] |= 0b00001111;
    lock_space[22] |= 0b00000000;
    lock_space[23] |= 0b00000001;

    lock_space[24] |= 0b00000011;
    lock_space[25] |= 0b00000111;
    lock_space[26] |= 0b00001111;
    lock_space[27] |= 0b00001111;
}

static void write_C(void) {
    lock_space[ 4] |= 0b11100000;
    lock_space[ 5] |= 0b11110000;
    lock_space[ 6] |= 0b01110000;
    lock_space[ 7] |= 0b00110000;

    lock_space[ 8] |= 0b00110000;
    lock_space[ 9] |= 0b00110000;
    lock_space[10] |= 0b00110000;
    lock_space[11] |= 0b00000000;

    lock_space[20] |= 0b00000111;
    lock_space[21] |= 0b00001111;
    lock_space[22] |= 0b00001110;
    lock_space[23] |= 0b00001100;

    lock_space[24] |= 0b00001100;
    lock_space[25] |= 0b00001100;
    lock_space[26] |= 0b00001100;
    lock_space[27] |= 0b00000000;
}

void write_lock_space(const uint8_t col, const uint8_t row) {
    oled_set_cursor(col, row);
    oled_write_raw(lock_space, 16);
    oled_set_cursor(col, row + 1);
    oled_write_raw(lock_space + 16, 16);
}

void print_num_lock(const uint8_t col, const uint8_t row) {
    clear_lock_space();
    if (led_usb_state.num_lock) {
        write_N();
    }
    write_lock_space(col, row);
}

void print_caps_lock(const uint8_t col, const uint8_t row) {
    clear_lock_space();
    if (led_usb_state.caps_lock) {
        write_C();
    }
    write_lock_space(col, row);
}

#define UPPER_LOCK_POS 4
#define LOWER_LOCK_POS 7
#define UPPER_MODIFIER_POS 10

static void print_mods(const uint16_t mods,
                       const uint16_t lbit,
                       const uint16_t rbit,
                       const char c,
                       const uint8_t pos)
{
    static char line[] = "     ";
    const char lchar = (mods & lbit) ? c : ' ';
    const char rchar = (mods & rbit) ? c : ' ';
    line[0] = lchar;
    line[4] = rchar;

    oled_set_cursor(0, pos);
    oled_write(line, false);
}

static void print_modifiers(const uint8_t pos) {
    const uint16_t mods = get_mods();

    print_mods(mods, MOD_BIT(KC_LSFT), MOD_BIT(KC_RSFT), 'S', pos + 0);
    print_mods(mods, MOD_BIT(KC_LCTL), MOD_BIT(KC_RCTL), 'C', pos + 1);
    print_mods(mods, MOD_BIT(KC_LALT), MOD_BIT(KC_RALT), 'A', pos + 2);
    print_mods(mods, MOD_BIT(KC_LGUI), MOD_BIT(KC_RGUI), 'M', pos + 3);
}

static void print_status_narrow(void) {
    oled_set_cursor(0, 0);
    print_arrow();

    oled_set_cursor(0, 1);
    switch (get_highest_layer(layer_state)) {
        case _RAISE_ES_LA:
            oled_write("raise", false);
            break;
        case _RAISE_EN_US:
            oled_write("raisE", false);
            break;
        case _LOWER:
            oled_write("lower", false);
            break;
        case _ADJUST:
            oled_write("adjst", false);
            break;

	    default:
            oled_write("-----", false);
    }

    oled_set_cursor(0, 2);
    switch (get_highest_layer(default_layer_state)) {
        case _ES_LA:
            oled_write("ES-LA", false);
            break;
        case _EN_US:
            oled_write("EN-US", false);
            break;
        case _NUMPAD:
            oled_write("NUMPD", false);
            break;
        default:
            oled_write("?????", true);
    }

    print_num_lock(left_pos, UPPER_LOCK_POS);
    print_caps_lock(left_pos, LOWER_LOCK_POS);
    print_modifiers(UPPER_MODIFIER_POS);
}

bool local_oled_task_user(void) {
    led_usb_state = host_keyboard_led_state();
    print_status_narrow();
    // if (is_keyboard_master()) {
    //     print_status_narrow();
    // } else {
    //     render_logo();
    // }
    return false;
}

void local_init_oled(void) {
    create_arrow();

    // initializes position for layer, caps, numlock strings.
    left_pos = my_local_status._is_left ? 0 : 2;

    // print squares for num/caps lock.
    write_lock_space(left_pos, UPPER_LOCK_POS);
    write_lock_space(left_pos, LOWER_LOCK_POS);
}


#endif

