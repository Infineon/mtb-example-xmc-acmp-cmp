/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the XMC MCU: ACMP (Analog Comparator)
*              Example for ModusToolbox.
*              Comparator block(ANACMP-1) is configured to receive divided
*              Reference voltage on positive input (IN-P) pin. Users may then
*              modify the voltage to negative input pin (IN-N) pin (Port 2.6)
*              and watch the user LED toggle according to ANACMP1:CMP_OUT change.
*
* Related Document: See README.md
*
******************************************************************************
*
* Copyright (c) 2015-2022, Infineon Technologies AG
* All rights reserved.
*
* Boost Software License - Version 1.0 - August 17th, 2003
*
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (the "Software") to use, reproduce, display, distribute,
* execute, and transmit the Software, and to prepare derivative works of the
* Software, and to permit third-parties to whom the Software is furnished to
* do so, all subject to the following:
*
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/
#include <stdio.h>
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"

/*******************************************************************************
* Defines
*******************************************************************************/
/* Define macro to enable/disable printing of debug messages */
#define ENABLE_XMC_DEBUG_PRINT              (0)

/* Define macro to set the loop count before printing debug messages */
#if ENABLE_XMC_DEBUG_PRINT
#define DEBUG_LOOP_COUNT_MAX                (1U)
#endif

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It sets up the comparator to receive Reference
* voltage on IN-P pin.Bit field CMP_OUT of ANACMP1 register changes its state
* based on the input applied on IN-N pad. The input to Comparator pin IN-N (Pin 2.6)
* can be modified to see ANACMP1:CMP_OUT register status to change. An LED is
* toggled depending upon the register status.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/

int main(void)
{
    cy_rslt_t result;

    #if ENABLE_XMC_DEBUG_PRINT
    /* Assign false to disable printing of debug messages*/
    static volatile bool debug_printf = true;
    /* Initialize the current loop count to zero */
    static uint32_t debug_loop_count_high = 0;
    static uint32_t debug_loop_count_low = 0;
    #endif

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_HW);

    #if ENABLE_XMC_DEBUG_PRINT
    printf("Initialization done\r\n");
    #endif

    /* Users may now apply inputs on P2.6 and evaluate USER LED output */
    while(1U)
    {
        /* Toggle USER LED depending on comparator output */
        if((COMPARATOR->ANACMP1) & COMPARATOR_ANACMP1_CMP_OUT_Msk)
        {
            #if ENABLE_XMC_DEBUG_PRINT
            debug_loop_count_high++;
            if (debug_printf && debug_loop_count_high == DEBUG_LOOP_COUNT_MAX)
            {
                debug_printf = false;
                /* Print message after the loop has run DEBUG_LOOP_COUNT_MAX times */
                printf("Turns LED ON because comparator output is high\r\n");
            }
            #endif
            /* Output low turns LED ON when comparator output is high */
            XMC_GPIO_SetOutputLow(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);
        }
        else
        {
            #if ENABLE_XMC_DEBUG_PRINT
            debug_loop_count_low++;
            if (debug_printf && debug_loop_count_low == DEBUG_LOOP_COUNT_MAX)
            {
                debug_printf = false;
                /* Print message after the loop has run DEBUG_LOOP_COUNT_MAX times */
                printf("Turns LED OFF because comparator output is low\r\n");
            }
            #endif
            /* Output low turns LED OFF when comparator output is low */
            XMC_GPIO_SetOutputHigh(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);
        }
        /* Infinite loop */
    }
}

/* [] END OF FILE */
