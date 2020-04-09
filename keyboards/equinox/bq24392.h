#ifndef BQ24392_H_
#define BQ24392_H_

#include "gpio.h"

typedef enum bq24392_status_e
{
    BQ24392_STATUS_SDP,
    BQ24392_STATUS_100_MA,
    BQ24392_STATUS_CDP,
    BQ24392_STATUS_DCP,
    BQ24392_STATUS_NO_CHARGE = 5,
} bq24392_status_e;

typedef void (*bq24392_event_handler)(bq24392_status_e status);

typedef struct bq24392_s
{
    gpio_t* chg_det;
    gpio_t* chg_al;
    gpio_t* sw_open;
    bq24392_event_handler event_handler;
} bq24392_t;

/* GPIOs must be configured by caller prior to 
 * calling functions other than _init() */
void bq24392_init(bq24392_t* bq24392,
                  gpio_t* chg_det,
                  gpio_t* chg_al,
                  gpio_t* sw_open,
                  bq24392_event_handler event_handler);

bq24392_status_e bq24392_status(bq24392_t* bq2439);
bool bq24392_usb_has_data(bq24392_t* bq24392);

#endif /* BQ24392_H_ */