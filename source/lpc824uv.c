/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include "SGX_G12864.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include <stdio.h>
#include <string.h>

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_MASTER_BASE (I2C0_BASE)
#define I2C_MASTER_CLOCK_FREQUENCY (12000000)
#define EXAMPLE_I2C_MASTER ((I2C_Type *)EXAMPLE_I2C_MASTER_BASE)

#define I2C_MASTER_SLAVE_ADDR_7BIT (0x10U)
#define I2C_BAUDRATE (100000) /* 100K */

/* Every 5 samples */
#define SAMPLES (5)

static uint8_t txData[2];
static uint8_t rxData[2];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    i2c_master_transfer_t masterXfer = {0};
    status_t reVal                   = kStatus_Fail;
    uint8_t i                        = 0;
    uint32_t sumdata                 = 0;

    /* Enable clock of uart0. */
    CLOCK_EnableClock(kCLOCK_Uart0);
    /* Ser DIV of uart0. */
    CLOCK_SetClkDivider(kCLOCK_DivUsartClk, 1U);
    /* Enable clock of i2c0. */
    CLOCK_EnableClock(kCLOCK_I2c0);
    /* Enable GPIO clock */
    CLOCK_EnableClock(kCLOCK_Gpio0);
    
    LED_GREEN_INIT(LOGIC_LED_ON);

    BOARD_InitPins();
    BOARD_BootClockIRC12M();
    BOARD_InitDebugConsole();

    PRINTF("\r\nI2C example -- Master transfer.\r\n");

    set_lcmGPIO();
    LcmInit();
    PUTImage(LOGO);
    delay_Long(30000);

    LcmClear();
    Welcome_on();
    delay_Long(30000);

    LcmClear();
    UV_data();

    i2c_master_config_t masterConfig;

    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kI2C_2PinOpenDrain;
     * masterConfig.baudRate_Bps = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    /* Initialize the I2C master peripheral */
    I2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer +
      stop*/
    uint8_t deviceAddress     = 0;
    txData[0]                 = 0x40;
    masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)deviceAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = txData;
    masterXfer.dataSize       = 1;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    /* Send master non-blocking data to slave */
    reVal = I2C_MasterTransferBlocking(EXAMPLE_I2C_MASTER, &masterXfer);

    if (reVal != kStatus_Success)
    {
        return -1;
    }

    PRINTF("Receive sent data from slave :");

    while (1)
    {
        deviceAddress             = 0x7U;
        masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR_7BIT;
        masterXfer.direction      = kI2C_Read;
        masterXfer.subaddress     = (uint32_t)deviceAddress;
        masterXfer.subaddressSize = 1;
        masterXfer.data           = rxData;
        masterXfer.dataSize       = 2;
        masterXfer.flags          = kI2C_TransferDefaultFlag;

        for (i = 0; i < SAMPLES; i++)
        {
            reVal = I2C_MasterTransferBlocking(EXAMPLE_I2C_MASTER, &masterXfer);

            if (reVal != kStatus_Success)
            {
                return -1;
            }
            sumdata += (rxData[1] << 8) + rxData[0];
        }

        PUTchar8x16(4, 11, "       ", 0);
        PUTNum(4, 11, sumdata / SAMPLES, 0);

        PRINTF("UVA data: %d\r\n", sumdata / SAMPLES);

        sumdata = 0;

        deviceAddress = 0x09U;
        for (i = 0; i < SAMPLES; i++)
        {
            reVal = I2C_MasterTransferBlocking(EXAMPLE_I2C_MASTER, &masterXfer);

            if (reVal != kStatus_Success)
            {
                return -1;
            }
            sumdata += (rxData[1] << 8) + rxData[0];
        }

        PUTchar8x16(6, 11, "       ", 0);
        PUTNum(6, 11, sumdata / SAMPLES, 0);
        PRINTF("UVB data: %d\r\n", sumdata / SAMPLES);

        delay_Long(20000);
    }
}
