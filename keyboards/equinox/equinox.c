#include "equinox.h"

#include "board.h"
#include "i2c_master.h"
#include "gpio.h"
#include "gpio_nrf5.h"
#include "lp50xx.h"
#include "bq2407x.h"
#include "bq24392.h"
#include "bq27441.h"

static const nrfx_twim_t bq27441_twim = NRFX_TWIM_INSTANCE(0);
static const nrfx_twim_t lp5018_twim = NRFX_TWIM_INSTANCE(1);

static i2c_master_t i2c_0;
static i2c_master_t i2c_1;
static lp50xx_t lp5018;
static bq24392_t bq24392;
static bq2407x_t bq24075;
static bq27441_t bq27441;

static gpio_t chg_al;
static gpio_t chg_det;
static gpio_t sw_open;

static gpio_t chg_en1;
static gpio_t chg_en2;

static gpio_t lp5018_enable;

static uint16_t battery_voltage; /* in mV */
static board_usb_state_e usb_state = BOARD_USB_DISCONNECTED;

typedef enum led_mode_e
{
    LED_MODE_OFF,
    LED_MODE_ON,
    LED_MODE_BLINK,
    LED_MODE_TIMEOUT,
} led_mode_e;

typedef struct led_cb_s
{
    led_mode_e mode;
    uint32_t color;
    uint32_t interval;
    uint32_t count;
    bool is_on;
    bool dirty;
} led_cb_s;

#define NUM_LEDS 4

static led_cb_s leds[] = {{0},{0},{0},{0}};

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {

	// put your per-action keyboard code here

	// runs for every action, just before processing by the firmware



	return process_record_user(keycode, record);

}


void on_bq24392_status_change(bq24392_status_e status)
{
    bq2407x_current_mode_e current_mode;

    NRF_LOG_INFO("BQ24392 status change: %d", status);

    switch(status)
    {
        case BQ24392_STATUS_NO_CHARGE:
            current_mode = BQ2407X_CURR_MODE_STANDBY;
            break;
        case BQ24392_STATUS_100_MA:
            current_mode = BQ2407X_CURR_MODE_USB100;
            break;
        case BQ24392_STATUS_SDP:
            current_mode = BQ2407X_CURR_MODE_USB100;
            if (usb_state == BOARD_USB_STARTED)
               current_mode = BQ2407X_CURR_MODE_USB500;
            break;
        case BQ24392_STATUS_DCP:
        case BQ24392_STATUS_CDP:
            current_mode = BQ2407X_CURR_MODE_ILIM;
            break;
        default:
            current_mode = BQ2407X_CURR_MODE_STANDBY;
            break;
    }

    bq2407x_set_current_mode(&bq24075, current_mode);
}


void eqnx_led_on(uint8_t led, uint32_t color)
{
    leds[led].mode = LED_MODE_ON;
    leds[led].color = color;
    leds[led].dirty = true;
}


void eqnx_led_off(uint8_t led)
{
    leds[led].mode = LED_MODE_OFF;
    leds[led].dirty = true;
}

void eqnx_led_blink(uint8_t led, uint32_t color, uint32_t interval)
{
    leds[led].mode = LED_MODE_BLINK;
    leds[led].color = color;
    leds[led].interval = interval;
    leds[led].dirty = true;
}

void eqnx_led_timeout(uint8_t led, uint32_t color, uint32_t timeout)
{
    leds[led].mode = LED_MODE_TIMEOUT;
    leds[led].color = color;
    leds[led].interval = timeout;
    leds[led].dirty = true;
}


void eqnx_init(void)
{

    gpio_nrf5_construct(&chg_al, BQ24392_CHG_AL);
    gpio_nrf5_construct(&chg_det, BQ24392_CHG_DET);
    gpio_nrf5_construct(&sw_open, BQ24392_SW_OPEN);
    gpio_config_as_input(&chg_al,GPIO_PULL_NONE);
    gpio_config_as_input(&chg_det,GPIO_PULL_NONE);
    gpio_config_as_input(&sw_open,GPIO_PULL_NONE);


    bq24392_init(&bq24392, &chg_det, &chg_al, &sw_open, on_bq24392_status_change);



    gpio_nrf5_construct(&chg_en1, BQ2407X_EN1);
    gpio_nrf5_construct(&chg_en2, BQ2407X_EN2);
    gpio_config_as_open_source(&chg_en1, GPIO_STATUS_HIGH, GPIO_PULL_NONE);
    gpio_config_as_open_source(&chg_en2, GPIO_STATUS_HIGH, GPIO_PULL_NONE);

    bq2407x_init(&bq24075, &chg_en1, &chg_en2, NULL, NULL);


    bq24392_status_e chg_status = bq24392_status(&bq24392);

    on_bq24392_status_change(chg_status);


    i2c_master_init(&i2c_1,
                    &lp5018_twim,
                    LP50XX_SCL,
                    LP50XX_SDA );


    i2c_master_init(&i2c_0,
                    &bq27441_twim,
                    BQ27441_SCL,
                    BQ27441_SDA );

    bq27441_init(&bq27441, &i2c_0);

    gpio_nrf5_construct(&lp5018_enable, LP50XX_EN);
    gpio_config_as_push_pull(&lp5018_enable, GPIO_STATUS_LOW);

    lp50xx_init(&lp5018, &i2c_1, &lp5018_enable, LP5018);

    NRF_LOG_INFO ("equinox initialized");
}

void eqnx_start(void)
{
    i2c_master_enable(&i2c_0, I2C_SPEED_100);

    i2c_master_enable(&i2c_1, I2C_SPEED_400);

    lp50xx_enable(&lp5018);

    nrf_delay_us(600);

    lp50xx_standby_exit(&lp5018);


    lp50xx_config_flags_e config = LP50XX_CFG_AUTO_INCR_EN |
                                   LP50XX_CFG_LOG_SCALE_EN |
                                   LP50XX_CFG_PWM_DITHERING_EN |
                                   LP50XX_CFG_POWER_SAVE_EN;

    lp50xx_config_set(&lp5018, config);

    NRF_LOG_INFO ("equinox started");
}


static void process_leds(void)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        led_cb_s* cb = &leds[i];

        switch (cb->mode)
        {
            case LED_MODE_ON:
                if (cb->dirty)
                {
                    lp50xx_led_rgb_set32(&lp5018, i, cb->color);
                    cb->is_on = true;
                    cb->dirty = false;
                }
                break;
            case LED_MODE_OFF:
                if (cb->dirty)
                {
                    lp50xx_led_rgb_set32(&lp5018, i, 0);
                    cb->is_on = false;
                    cb->dirty = false;
                }
                break;
            case LED_MODE_TIMEOUT:
                if (cb->dirty)
                {
                    lp50xx_led_rgb_set32(&lp5018, i, cb->color);
                    cb->is_on = true;
                    cb->count = timer_read32();
                    cb->dirty = false;
                }
                else if (timer_elapsed32(cb->count) > cb->interval)
                {
                    lp50xx_led_rgb_set32(&lp5018, i, 0);
                    cb->is_on = false;
                    cb->mode = LED_MODE_OFF;
                }
                break;
            case LED_MODE_BLINK:
                if (cb->dirty)
                {
                    lp50xx_led_rgb_set32(&lp5018, i, cb->color);
                    cb->count = timer_read32();
                    cb->is_on = true;
                    cb->dirty = false;
                }
                else if (timer_elapsed32(cb->count) > cb->interval)
                {
                    if (cb->is_on)
                        lp50xx_led_rgb_set32(&lp5018, i, 0);
                    else
                        lp50xx_led_rgb_set32(&lp5018, i, cb->color);

                    cb->is_on = !cb->is_on;
                    cb->count = timer_read32();
                }
                break;
        }
    }
}

void eqnx_task(void)
{
    process_leds();
}

void eqnx_sleep(void)
{
    lp50xx_disable(&lp5018);
    i2c_master_disable(&i2c_1);
    i2c_master_disable(&i2c_0);
}

void eqnx_on_usb_state_change(board_usb_state_e state)
{
    usb_state = state;

    NRF_LOG_INFO("Equinox usb state changed: %d", state);

    switch(state)
    {
        case BOARD_USB_CONNECTED:
        case BOARD_USB_DISCONNECTED:
        case BOARD_USB_SUSPEND:
            bq2407x_set_current_mode(&bq24075, BQ2407X_CURR_MODE_STANDBY);
            break;
        case BOARD_USB_STARTED:
        case BOARD_USB_RESUME:
        {
            bq24392_status_e chg_status = bq24392_status(&bq24392);
            NRF_LOG_INFO("Equinox charge status: %d", chg_status);
            if (chg_status == BQ24392_STATUS_SDP)
                bq2407x_set_current_mode(&bq24075, BQ2407X_CURR_MODE_USB500);
            else if (chg_status == BQ24392_STATUS_CDP)
                bq2407x_set_current_mode(&bq24075, BQ2407X_CURR_MODE_ILIM);
        }
            break;

        default:
            break;
    }
}

uint16_t eqnx_battery_voltage(void)
{
    return battery_voltage;
}


uint8_t eqnx_battery_level(void)
{
    uint8_t level;
    static bool low_battery = false;

    bq27441_voltage(&bq27441, &battery_voltage);
    level = (battery_voltage * 100)/4100;

    if (!low_battery)
    {
        if (battery_voltage <= LOW_BATTERY_LEVEL)
        {
          low_battery = true;
          eqnx_led_on(BATTERY_STATUS_LED, LOW_BATTERY_COLOR);
        }
    }
    else
    {
       if (battery_voltage > LOW_BATTERY_LEVEL + LOW_BATTERY_HYSTERESIS)
       {
          low_battery = false;
          eqnx_led_off(BATTERY_STATUS_LED);
       }
    }

    return level;
}
