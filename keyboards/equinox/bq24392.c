#include <stddef.h>
#include "bq24392.h"

static void on_chg_al_changed(gpio_t* gpio, gpio_status_e status, void* user_data)
{
    bq24392_t* bq24392 = (bq24392_t*)user_data;

    if (bq24392 != NULL && bq24392->event_handler != NULL)
    {
        bq24392_status_e status = bq24392_status(bq24392);
        bq24392->event_handler(status);
    }
}

void bq24392_init(bq24392_t* bq24392,
                  gpio_t* chg_det,
                  gpio_t* chg_al,
                  gpio_t* sw_open,
                  bq24392_event_handler event_handler)
{
    /* ensure: all params are non-null */

    bq24392->chg_al = chg_al;
    bq24392->chg_det = chg_det;
    bq24392->sw_open = sw_open;
    bq24392->event_handler = event_handler;

    gpio_add_trigger(chg_al, GPIO_TRIGGER_TOGGLE, on_chg_al_changed, bq24392);

}

bq24392_status_e bq24392_status(bq24392_t* bq24392)
{
    static  bq24392_status_e status_map[] =
    {
        BQ24392_STATUS_SDP,
        BQ24392_STATUS_100_MA,
        BQ24392_STATUS_CDP,
        BQ24392_STATUS_DCP,
        BQ24392_STATUS_NO_CHARGE,
        BQ24392_STATUS_NO_CHARGE,
    };

    uint8_t index = 0;

    bq24392_status_e status;

    gpio_status_e chg_al = gpio_read(bq24392->chg_al);
    gpio_status_e chg_det = gpio_read(bq24392->chg_det);
    gpio_status_e sw_open = gpio_read(bq24392->sw_open);    


    if (chg_al == GPIO_STATUS_HIGH)
        index |= 1 << 2;
    if (chg_det == GPIO_STATUS_HIGH)
        index |= 1 << 1;
    if (sw_open == GPIO_STATUS_HIGH)
        index |= 1 << 0;

    status = status_map[index];

    return status;
}

bool 
bq24392_usb_has_data(bq24392_t* bq24392)
{
    gpio_status_e sw_open = gpio_read(bq24392->sw_open);
    bool has_data = (sw_open == GPIO_STATUS_LOW);
    return has_data;
}

