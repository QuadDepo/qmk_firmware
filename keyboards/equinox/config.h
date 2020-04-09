/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert
Copyright 2017 Biacco42
Copyright 2018 Sekigon

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H


#define ARGB_RED      0xFFFF0000
#define ARGB_GREEN    0xFF002F00
#define ARGB_BLUE     0xFF0000FF
#define ARGB_YELLOW   0xFFFFFF00
#define ARGB_MAGENTA  0xFFFF00FF
#define ARGB_CYAN     0xFF00FFFF
#define ARGB_WHITE    0xFFFFFFFF
#define ARGB_ORANGE   0XFFFF2F00



#define BLE_NUS_MIN_INTERVAL 15
#define BLE_NUS_MAX_INTERVAL 30
#define BLE_HID_MAX_INTERVAL 30
#define BLE_HID_SLAVE_LATENCY 10

/* USB Device descriptor parameter */
#define VENDOR_ID       0xBC42
#define PRODUCT_ID      0x0042
#define DEVICE_VER      0x0100
#define MANUFACTURER    Equinox
#define PRODUCT         Equinox
#define DESCRIPTION     Spilt Wireless Keyboard

/* key matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 6

#define DIODE_DIRECTION COL2ROW


#define LED_LEFT 0
#define LED_RIGHT 3

#define BLE_HOST_STATUS_LED   LED_LEFT
#define BLE_SLAVE_STATUS_LED  LED_RIGHT
#define BATTERY_STATUS_LED    LED_RIGHT
#define NUM_LOCK_LED          LED_RIGHT
#define CAPS_LOCK_LED         LED_RIGHT
#define SCROLL_LOCK_LED       LED_RIGHT
#define LAYER_LED             LED_LEFT

#define NUM_LOCK_COLOR ARGB_RED
#define CAPS_LOCK_COLOR ARGB_GREEN
#define SCROLL_LOCK_COLOR ARGB_BLUE
#define LOW_BATTERY_COLOR ARGB_ORANGE

#define LOW_BATTERY_LEVEL 3500
#define LOW_BATTERY_HYSTERESIS 200

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    1

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#define ENABLE_STARTUP_ADV_NOLIST

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

#endif
