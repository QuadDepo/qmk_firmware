#include "board.h"
#include "config.h"
#include "equinox.h"

bool set_caps = false;
bool clr_caps = false;
bool set_ble_status = false;
uint32_t ble_status_color;

enum hid_leds
{
    HID_LED_NUM_LOCK    = (1 << 0),
    HID_LED_CAPS_LOCK   = (1 << 1),
    HID_LED_SCROLL_LOCK = (1 << 2),
};

void board_on_hid_leds(uint8_t leds)
{
#if defined(NUM_LOCK_LED) && defined (NUM_LOCK_COLOR)
    if (leds & HID_LED_NUM_LOCK)
        eqnx_led_on(NUM_LOCK_LED, NUM_LOCK_COLOR);
    else
        eqnx_led_off(NUM_LOCK_LED);
#endif
#if defined(CAPS_LOCK_LED) && defined (CAPS_LOCK_COLOR)
    if (leds & HID_LED_CAPS_LOCK)
        eqnx_led_on(CAPS_LOCK_LED, CAPS_LOCK_COLOR);
    else
        eqnx_led_off(CAPS_LOCK_LED);

#endif
#if defined(SCROLL_LOCK_LED) && defined (SCROLL_LOCK_COLOR)
    if (leds & HID_LED_SCROLL_LOCK)
        eqnx_led_on(SCROLL_LOCK_LED, SCROLL_LOCK_COLOR);
    else
        eqnx_led_off(SCROLL_LOCK_LED);
#endif

}



void board_init(void)
{
    eqnx_init();
}
void board_start(void)
{
    eqnx_start();
    board_on_ble_event(BOARD_BLE_HOST_DISCONNECTED);
}
void board_task(void)
{
    eqnx_task();

}

void board_sleep(void)
{
   eqnx_sleep();
}


void
board_on_usb_state_change(board_usb_state_e state)
{
    eqnx_on_usb_state_change(state);
}


void board_on_ble_event(board_ble_event_e event)
{
    switch(event)
    {
        case BOARD_BLE_HOST_DISCONNECTED:
            eqnx_led_on(BLE_HOST_STATUS_LED, ARGB_RED);
            break;
        case BOARD_BLE_HOST_ADVERTISING:
            eqnx_led_blink(BLE_HOST_STATUS_LED, ARGB_GREEN, 250);
            break;
        case BOARD_BLE_HOST_CONNECTED:
            eqnx_led_timeout(BLE_HOST_STATUS_LED, ARGB_GREEN, 5000);
            break;
        case BOARD_BLE_SLAVE_DISCONNECTED:
            eqnx_led_on(BLE_SLAVE_STATUS_LED, ARGB_RED);
            break;
        case BOARD_BLE_SLAVE_SCANNING:
            eqnx_led_blink(BLE_SLAVE_STATUS_LED, ARGB_GREEN, 250);
            break;
        case BOARD_BLE_SLAVE_CONNECTED:
            eqnx_led_timeout(BLE_SLAVE_STATUS_LED, ARGB_GREEN, 5000);
            break;
        default:
            break;

    }

}

uint8_t board_battery_level(void)
{
    return eqnx_battery_level();
}
