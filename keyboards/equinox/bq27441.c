#include <stddef.h>

#include "bq27441.h"
#include "i2c_master.h"

bq27441_result_e
command_read(bq27441_t* fg, uint8_t cmd, uint16_t* data)
{
    i2c_result_e i2c_result;
    bq27441_result_e result = BQ27441_SUCCESS;

    static uint8_t wr_buffer[2];
    uint8_t wr_length = 1;

    wr_buffer[0] = cmd;

    i2c_result = i2c_master_transmit_receive(fg->i2c,
                                             BQ72441_I2C_ADDRESS,
                                             wr_buffer,
                                             wr_length,
                                             (uint8_t*)data,
                                             2);

    if (i2c_result == I2C_RESULT_BUSY)
    {
        result = BQ27441_ERROR_BUSY;
    }
    else
    {
        result = BQ27441_ERROR_INTERNAL;
    }


    return result;
}

void
bq27441_init(bq27441_t* fg,
             i2c_master_t* i2c)
{
    fg->i2c = i2c;
}

bq27441_result_e
bq27441_voltage(bq27441_t* fg, uint16_t* voltage)
{
    bq27441_result_e result;

    result = command_read(fg, BQ27441_COMMAND_VOLTAGE, voltage);

    return result;
}

bq27441_result_e
bq27441_nominal_available_capacity(bq27441_t* fg, uint16_t* capacity)
{
    bq27441_result_e result;

    result = command_read(fg, BQ27441_COMMAND_NOM_CAPACITY, capacity);

    return result;
}
