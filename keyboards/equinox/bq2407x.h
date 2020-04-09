#pragma once
#ifndef BQ2407X_H
#define BQ2407X_H

#include "gpio.h"

typedef struct bq2407x_s
{
    gpio_t* en1;
    gpio_t* en2;
    gpio_t* chg;
    gpio_t* pgood;
} bq2407x_t;

typedef enum bq2407x_current_mode_e
{
    BQ2407X_CURR_MODE_USB100,
    BQ2407X_CURR_MODE_USB500,
    BQ2407X_CURR_MODE_ILIM,
    BQ2407X_CURR_MODE_STANDBY,
} bq2407x_current_mode_e;

void bq2407x_init(bq2407x_t* bq,
                  gpio_t* en1,
                  gpio_t* en2,
                  gpio_t* chg,
                  gpio_t* pgood);
void bq2407x_set_current_mode(bq2407x_t* bq,
                              bq2407x_current_mode_e mode);
bool bq2407x_is_charging(bq2407x_t* bq);
bool bq24075_power_is_good(bq2407x_t* bq);

#endif /* BQ2407X_H */
