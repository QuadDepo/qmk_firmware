#ifndef EQUINOX_H_
#define EQUINOX_H_

#include "quantum.h"
#include "nrf_gpio.h"

#include "board.h"

#define LAYOUT( \
	L00, L01, L02, L03, L04, L05, R00, R01, R02, R03, R04, R05, \
	L10, L11, L12, L13, L14, L15, R10, R11, R12, R13, R14, R15, \
	L20, L21, L22, L23, L24, L25, R20, R21, R22, R23, R24, R25, \
	L30, L31, L32, L33, L34, L35, R30, R31, R32, R33, R34, R35 \
	) \
	{ \
		{ L00, L01, L02, L03, L04, L05 }, \
		{ L10, L11, L12, L13, L14, L15 }, \
		{ L20, L21, L22, L23, L24, L25 }, \
		{ L30, L31, L32, L33, L34, L35 }, \
		{ R05, R04, R03, R02, R01, R00 }, \
		{ R15, R14, R13, R12, R11, R10 }, \
		{ R25, R24, R23, R22, R21, R20 }, \
		{ R35, R34, R33, R32, R31, R30 } \
	}



#define BQ2407X_EN1     NRF_GPIO_PIN_MAP(1,12)
#define BQ2407X_EN2     NRF_GPIO_PIN_MAP(1,13)
#define BQ2407X_CHG     NULL
#define BQ2407X_PGOOD   NULL

#define LP50XX_EN       NRF_GPIO_PIN_MAP(0,31)
#define LP50XX_SCL      NRF_GPIO_PIN_MAP(0,30)
#define LP50XX_SDA      NRF_GPIO_PIN_MAP(0,29)

#define BQ24392_CHG_AL  NRF_GPIO_PIN_MAP(0,13)
#define BQ24392_SW_OPEN NRF_GPIO_PIN_MAP(0,14)
#define BQ24392_CHG_DET NRF_GPIO_PIN_MAP(0,15)

#define BQ27441_SCL     NRF_GPIO_PIN_MAP(0, 3)
#define BQ27441_SDA     NRF_GPIO_PIN_MAP(0, 5)

void eqnx_led_on(uint8_t led, uint32_t color);
void eqnx_led_off(uint8_t led);
void eqnx_led_blink(uint8_t led, uint32_t color, uint32_t interval);
void eqnx_led_timeout(uint8_t led, uint32_t color, uint32_t timeout);
//todo: func which temporarily changes color and then goes back to original

void eqnx_init(void);
void eqnx_start(void);
void eqnx_task(void);
uint16_t eqnx_battery_voltage(void);
uint8_t eqnx_battery_level(void);
void eqnx_sleep(void);
void eqnx_on_usb_state_change(board_usb_state_e state);

#endif /* EQUINOX_H_ */
