#pragma once
#ifndef BOARD_H_
#define BOARD_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum board_usb_state_e
{
    BOARD_USB_CONNECTED,
    BOARD_USB_DISCONNECTED,
    BOARD_USB_STARTED,
    BOARD_USB_SUSPEND,
    BOARD_USB_RESUME,
} board_usb_state_e;

typedef enum board_ble_event_e
{
    BOARD_BLE_HOST_DISCONNECTED,
    BOARD_BLE_HOST_ADVERTISING,
    BOARD_BLE_HOST_CONNECTED,
    BOARD_BLE_SLAVE_DISCONNECTED,
    BOARD_BLE_SLAVE_SCANNING,
    BOARD_BLE_SLAVE_CONNECTED,
} board_ble_event_e;

void board_on_hid_leds(uint8_t leds);
void board_on_usb_state_change(board_usb_state_e state);
void board_on_ble_event(board_ble_event_e event);
void board_init(void);
void board_start(void);
void board_task(void);
void board_sleep(void);
uint8_t board_battery_level(void);

#endif
