/* =======================================================================
 * bq27441.h
 *
 * Purpose:
 *  Declarations interface for TI BQ-27441 Battery Fuel Guage
 *
 * License:
 *  Copyright (c) 2018, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */

#ifndef BQ27441_H
#define BQ27441_H


#include <stdint.h>
#include <stdbool.h>

#include "i2c_master.h"

#define BQ72441_I2C_ADDRESS 0x55 // Default I2C address of the BQ27441-G1A

/* General Constants
 * ------------------------------------------------------ */
#define BQ27441_UNSEAL_KEY              0x8000 // Unseal code
#define BQ27441_DEVICE_ID               0x0421 // Default device ID

/* Standard Commands
 * ------------------------------------------------------ */
#define BQ27441_COMMAND_CONTROL         0x00 // Control()
#define BQ27441_COMMAND_TEMP            0x02 // Temperature()
#define BQ27441_COMMAND_VOLTAGE         0x04 // Voltage()
#define BQ27441_COMMAND_FLAGS           0x06 // Flags()
#define BQ27441_COMMAND_NOM_CAPACITY    0x08 // NominalAvailableCapacity()
#define BQ27441_COMMAND_AVAIL_CAPACITY  0x0A // FullAvailableCapacity()
#define BQ27441_COMMAND_REM_CAPACITY    0x0C // RemainingCapacity()
#define BQ27441_COMMAND_FULL_CAPACITY   0x0E // FullChargeCapacity()
#define BQ27441_COMMAND_AVG_CURRENT     0x10 // AverageCurrent()
#define BQ27441_COMMAND_STDBY_CURRENT   0x12 // StandbyCurrent()
#define BQ27441_COMMAND_MAX_CURRENT     0x14 // MaxLoadCurrent()
#define BQ27441_COMMAND_AVG_POWER       0x18 // AveragePower()
#define BQ27441_COMMAND_SOC             0x1C // StateOfCharge()
#define BQ27441_COMMAND_INT_TEMP        0x1E // InternalTemperature()
#define BQ27441_COMMAND_SOH             0x20 // StateOfHealth()
#define BQ27441_COMMAND_REM_CAP_UNFL    0x28 // RemainingCapacityUnfiltered()
#define BQ27441_COMMAND_REM_CAP_FIL     0x2A // RemainingCapacityFiltered()
#define BQ27441_COMMAND_FULL_CAP_UNFL   0x2C // FullChargeCapacityUnfiltered()
#define BQ27441_COMMAND_FULL_CAP_FIL    0x2E // FullChargeCapacityFiltered()
#define BQ27441_COMMAND_SOC_UNFL        0x30 // StateOfChargeUnfiltered()

/* Control Subcommands
 * ------------------------------------------------------ */
#define BQ27441_CONTROL_STATUS          0x00
#define BQ27441_CONTROL_DEVICE_TYPE     0x01
#define BQ27441_CONTROL_FW_VERSION      0x02
#define BQ27441_CONTROL_DM_CODE         0x04
#define BQ27441_CONTROL_PREV_MACWRITE   0x07
#define BQ27441_CONTROL_CHEM_ID         0x08
#define BQ27441_CONTROL_BAT_INSERT      0x0C
#define BQ27441_CONTROL_BAT_REMOVE      0x0D
#define BQ27441_CONTROL_SET_HIBERNATE   0x11
#define BQ27441_CONTROL_CLEAR_HIBERNATE 0x12
#define BQ27441_CONTROL_SET_CFGUPDATE   0x13
#define BQ27441_CONTROL_SHUTDOWN_ENABLE 0x1B
#define BQ27441_CONTROL_SHUTDOWN        0x1C
#define BQ27441_CONTROL_SEALED          0x20
#define BQ27441_CONTROL_PULSE_SOC_INT   0x23
#define BQ27441_CONTROL_RESET           0x41
#define BQ27441_CONTROL_SOFT_RESET      0x42
#define BQ27441_CONTROL_EXIT_CFGUPDATE  0x43
#define BQ27441_CONTROL_EXIT_RESIM      0x44

/* Extended Data Commands
 * ------------------------------------------------------ */
#define BQ27441_COMMAND_EX_OPCONFIG     0x3A // OpConfig()
#define BQ27441_COMMAND_EX_CAPACITY     0x3C // DesignCapacity()
#define BQ27441_COMMAND_EX_DATACLASS    0x3E // DataClass()
#define BQ27441_COMMAND_EX_DATABLOCK    0x3F // DataBlock()
#define BQ27441_COMMAND_EX_BLOCKDATA    0x40 // BlockData()
#define BQ27441_COMMAND_EX_CHECKSUM     0x60 // BlockDataCheckSum()
#define BQ27441_COMMAND_EX_CONTROL      0x61 // BlockDataControl()

/* CONTROL_STATUS  Bit Definitions
 * ------------------------------------------------------ */
#define BQ27441_STATUS_SHUTDOWNEN   (1<<15)
#define BQ27441_STATUS_WDRESET      (1<<14)
#define BQ27441_STATUS_SS           (1<<13)
#define BQ27441_STATUS_CALMODE      (1<<12)
#define BQ27441_STATUS_CCA          (1<<11)
#define BQ27441_STATUS_BCA          (1<<10)
#define BQ27441_STATUS_QMAX_UP      (1<<9)
#define BQ27441_STATUS_RES_UP       (1<<8)
#define BQ27441_STATUS_INITCOMP     (1<<7)
#define BQ27441_STATUS_HIBERNATE    (1<<6)
#define BQ27441_STATUS_SLEEP        (1<<4)
#define BQ27441_STATUS_LDMD         (1<<3)
#define BQ27441_STATUS_RUP_DIS      (1<<2)
#define BQ27441_STATUS_VOK          (1<<1)

/* COMMAND_FLAGS - Bit Definitions
 * ------------------------------------------------------ */
#define BQ27441_FLAG_OT         (1<<15)
#define BQ27441_FLAG_UT         (1<<14)
#define BQ27441_FLAG_FC         (1<<9)
#define BQ27441_FLAG_CHG        (1<<8)
#define BQ27441_FLAG_OCVTAKEN   (1<<7)
#define BQ27441_FLAG_ITPOR      (1<<5)
#define BQ27441_FLAG_CFGUPMODE  (1<<4)
#define BQ27441_FLAG_BAT_DET    (1<<3)
#define BQ27441_FLAG_SOC1       (1<<2)
#define BQ27441_FLAG_SOCF       (1<<1)
#define BQ27441_FLAG_DSG        (1<<0)


/* Configuration Class, Subclass IDs
 * ------------------------------------------------------ */
#define BQ27441_ID_SAFETY           2   // Safety
#define BQ27441_ID_CHG_TERMINATION  36  // Charge Termination
#define BQ27441_ID_CONFIG_DATA      48  // Data
#define BQ27441_ID_DISCHARGE        49  // Discharge
#define BQ27441_ID_REGISTERS        64  // Registers
#define BQ27441_ID_POWER            68  // Power
// Gas Gauging Classes
#define BQ27441_ID_IT_CFG           80  // IT Cfg
#define BQ27441_ID_CURRENT_THRESH   81  // Current Thresholds
#define BQ27441_ID_STATE            82  // State
// Ra Tables Classes
#define BQ27441_ID_R_A_RAM          89  // R_a RAM
// Calibration Classes
#define BQ27441_ID_CALIB_DATA       104 // Data
#define BQ27441_ID_CC_CAL           105 // CC Cal
#define BQ27441_ID_CURRENT          107 // Current
// Security Classes
#define BQ27441_ID_CODES            112 // Codes


/* Configuration Clas,s SubClass offsets
 * ------------------------------------------------------ */
#define BQ27441_OVER_TEMP           0
#define BQ27441_UNDER_TEMP          2
#define BQ27441_TEMP_HYS            4
#define BQ27441_MIN_TAPER_CAP       0
#define BQ27441_CUR_TAPER_WINDOW    2
#define BQ27441_TCA_SET_PCT         3
#define BQ27441_TCA_CLR_PCT         4
#define BQ27441_FC_SET_PCT          5
#define BQ27441_FC_CLEAR_PCT        6
#define BQ27441_DOD_COC_DELTA_T     7
#define BQ27441_INIT_STANDBY        2
#define BQ27441_INIT_MAX_LOAD       3
#define BQ27441_SOC1_SET_THRESHOLD  0
#define BQ27441_SOC1_CLR_THRESHOLD  1
#define BQ27441_SOCF_SET_THRESHOLD  2
#define BQ27441_SOCF_CLR_THRESHOLD  3
#define BQ27441_OP_CONFIG           0
#define BQ27441_OP_CONFIG_B         2
#define BQ27441_DM_CODE             3
#define BQ27441_HIBERNATE_1         7
#define BQ27441_HIBERNATE_V         9
#define BQ27441_FS_WAIT             11
/* TODO complete this */

/* OpConfig Register Definitions
 * ------------------------------------------------------ */
#define BQ27441_OPCONFIG_BIE      (1<<13)
#define BQ27441_OPCONFIG_BI_PU_EN (1<<12)
#define BQ27441_OPCONFIG_GPIOPOL  (1<<11)
#define BQ27441_OPCONFIG_SLEEP    (1<<5)
#define BQ27441_OPCONFIG_RMFCC    (1<<4)
#define BQ27441_OPCONFIG_BATLOWEN (1<<2)
#define BQ27441_OPCONFIG_TEMPS    (1<<0)

/* OpConfigB Register Definitions
 * ------------------------------------------------------ */
#define BQ27441_OPCONFIG_B_SMOOTHEN (1<<2)


/* Enumerations
 * ------------------------------------------------------ */

// Parameters for the capacity() function, to specify which capacity to read
typedef enum {
    CAPACITY_TYPE_REMAINING,              // Remaining Capacity (DEFAULT)
    CAPACITY_TYPE_FULL_CHARGE,            // Full Charge Capacity
    CAPACITY_TYPE_NOMINAL_AVAILABLE,      // Nominal Available Capacity
    CAPACITY_TYPE_FULL_AVAILABLE,         // Full Available Capacity
    CAPACITY_TYPE_REMAIN_F,   // Remaining Capacity Filtered
    CAPACITY_TYPE_REMAIN_UF,  // Remaining Capacity Unfiltered
    CAPACITY_TYPE_FULL_F,     // Full Capacity Filtered
    CAPACITY_TYPE_FULL_UF,    // Full Capacity Unfiltered
    CAPACITY_TYPE_DESIGN                   // Design Capacity
} capacity_type;

// Parameters for the soc() function
typedef enum {
    FILTERED,  // State of Charge Filtered (DEFAULT)
    UNFILTERED // State of Charge Unfiltered
} soc_measure;

// Parameters for the soh() function
typedef enum {
    PERCENT,  // State of Health Percentage (DEFAULT)
    SOH_STAT  // State of Health Status Bits
} soh_measure;

// Parameters for the temperature() function
typedef enum {
    BATTERY,      // Battery Temperature (DEFAULT)
    INTERNAL_TEMP // Internal IC Temperature
} temp_measure;

// Parameters for the setGPOUTFunction() funciton
typedef enum {
    SOC_INT, // Set GPOUT to SOC_INT functionality
    BAT_LOW  // Set GPOUT to BAT_LOW functionality
} gpout_function;

/* Function pointer declarations
 * ------------------------------------------------------ */

typedef enum bq27441_result_e
{
    BQ27441_SUCCESS,
    BQ27441_ERROR_BUSY,
    BQ27441_ERROR_INTERNAL,
} bq27441_result_e;


/* Structures
 * ------------------------------------------------------ */

typedef struct bq27441_s
{
    i2c_master_t* i2c;
} bq27441_t;


/* Function declarations
 * ------------------------------------------------------ */

void bq27441_init(bq27441_t* fg,
                  i2c_master_t* i2c);


bq27441_result_e bq27441_voltage(bq27441_t* fg, uint16_t* voltage);
bq27441_result_e bq27441_nominal_available_capacity(bq27441_t* fg, uint16_t* capacity );


#endif // BQ27441_H
