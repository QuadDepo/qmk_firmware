#include "bq2407x.h"

#include <stdint.h>
#include <stddef.h>

static void 
set_mode_usb100(bq2407x_t* bq)
{
    if (bq->en1 != NULL) 
        gpio_clear(bq->en1);
    if (bq->en2 != NULL) 
        gpio_clear(bq->en2);
}

static void 
set_mode_usb500(bq2407x_t* bq)
{
    if (bq->en1 != NULL) 
        gpio_set(bq->en1);
    if (bq->en2 != NULL) 
        gpio_clear(bq->en2);
}

static void 
set_mode_ilim(bq2407x_t* bq)
{
    if (bq->en1 != NULL) 
        gpio_clear(bq->en1);
    if (bq->en2 != NULL) 
        gpio_set(bq->en2);
}

static void 
set_mode_standby(bq2407x_t* bq)
{
    if (bq->en1 != NULL)
        gpio_set(bq->en1);
    if (bq->en2 != NULL)
        gpio_set(bq->en2);
}



void bq2407x_init(bq2407x_t* bq,
                  gpio_t* en1,
                  gpio_t* en2,
                  gpio_t* chg,
                  gpio_t* pgood )
{
    bq->en1 = en1;
    bq->en2 = en2;
    bq->chg = chg;
    bq->pgood = pgood;
}

void bq2407x_set_current_mode(bq2407x_t* bq, bq2407x_current_mode_e mode)
{
    switch(mode)
    {
        case BQ2407X_CURR_MODE_USB100:
            set_mode_usb100(bq);
            break;

        case BQ2407X_CURR_MODE_USB500:
            set_mode_usb500(bq);
            break;

        case BQ2407X_CURR_MODE_ILIM:
            set_mode_ilim(bq);
            break;

        case BQ2407X_CURR_MODE_STANDBY:
            set_mode_standby(bq);
            break;

        default:
            break;
    }
}

bool bq2407x_is_charging(bq2407x_t* bq)
{
    bool is_charging = false;

    if(bq->chg != NULL)
    {
        gpio_status_e status = gpio_read(bq->chg);
        is_charging = (status == GPIO_STATUS_LOW);
    }

    return is_charging;
}

bool bq24075_power_is_good(bq2407x_t* bq)
{
    bool is_good = false;
    if (bq->pgood != NULL)
    {
        gpio_status_e status = gpio_read(bq->pgood);
        is_good = (status == GPIO_STATUS_LOW);
    }

    return is_good;
}

