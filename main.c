/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the CE231732 PSoC 4 MCU: PWM
*              triggering a DMA Channel Example for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2020-2022, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/


/*******************************************************************************
 * Include header files
 ******************************************************************************/

#include "cy_pdl.h"
#include "cybsp.h"
#include "cy_retarget_io.h"


/*******************************************************************************
* Macros
********************************************************************************/
#define USER_PWM_CNT             ((TCPWM_CNT_Type*) &USER_PWM_HW->CNT[USER_PWM_NUM])
#define CHANN_NUM                (0UL)
#define ARRAY_SIZE               (256u)
#define BREATHING_CHANGE         (512u)




const uint32_t sine[] = {
	0x8000,0x8324,0x8647,0x896a,0x8c8b,0x8fab,0x92c7,0x95e1,
	0x98f8,0x9c0b,0x9f19,0xa223,0xa527,0xa826,0xab1f,0xae10,
	0xb0fb,0xb3de,0xb6b9,0xb98c,0xbc56,0xbf17,0xc1cd,0xc47a,
	0xc71c,0xc9b3,0xcc3f,0xcebf,0xd133,0xd39a,0xd5f5,0xd842,
	0xda82,0xdcb3,0xded7,0xe0eb,0xe2f1,0xe4e8,0xe6cf,0xe8a6,
	0xea6d,0xec23,0xedc9,0xef5e,0xf0e2,0xf254,0xf3b5,0xf504,
	0xf641,0xf76b,0xf884,0xf989,0xfa7c,0xfb5c,0xfc29,0xfce3,
	0xfd89,0xfe1d,0xfe9c,0xff09,0xff61,0xffa6,0xffd8,0xfff5,
	0xffff,0xfff5,0xffd8,0xffa6,0xff61,0xff09,0xfe9c,0xfe1d,
	0xfd89,0xfce3,0xfc29,0xfb5c,0xfa7c,0xf989,0xf884,0xf76b,
	0xf641,0xf504,0xf3b5,0xf254,0xf0e2,0xef5e,0xedc9,0xec23,
	0xea6d,0xe8a6,0xe6cf,0xe4e8,0xe2f1,0xe0eb,0xded7,0xdcb3,
	0xda82,0xd842,0xd5f5,0xd39a,0xd133,0xcebf,0xcc3f,0xc9b3,
	0xc71c,0xc47a,0xc1cd,0xbf17,0xbc56,0xb98c,0xb6b9,0xb3de,
	0xb0fb,0xae10,0xab1f,0xa826,0xa527,0xa223,0x9f19,0x9c0b,
	0x98f8,0x95e1,0x92c7,0x8fab,0x8c8b,0x896a,0x8647,0x8324,
	0x8000,0x7cdb,0x79b8,0x7695,0x7374,0x7054,0x6d38,0x6a1e,
	0x6707,0x63f4,0x60e6,0x5ddc,0x5ad8,0x57d9,0x54e0,0x51ef,
	0x4f04,0x4c21,0x4946,0x4673,0x43a9,0x40e8,0x3e32,0x3b85,
	0x38e3,0x364c,0x33c0,0x3140,0x2ecc,0x2c65,0x2a0a,0x27bd,
	0x257d,0x234c,0x2128,0x1f14,0x1d0e,0x1b17,0x1930,0x1759,
	0x1592,0x13dc,0x1236,0x10a1,0xf1d,0xdab,0xc4a,0xafb,
	0x9be,0x894,0x77b,0x676,0x583,0x4a3,0x3d6,0x31c,
	0x276,0x1e2,0x163,0xf6,0x9e,0x59,0x27,0xa,
	0x0,0xa,0x27,0x59,0x9e,0xf6,0x163,0x1e2,
	0x276,0x31c,0x3d6,0x4a3,0x583,0x676,0x77b,0x894,
	0x9be,0xafb,0xc4a,0xdab,0xf1d,0x10a1,0x1236,0x13dc,
	0x1592,0x1759,0x1930,0x1b17,0x1d0e,0x1f14,0x2128,0x234c,
	0x257d,0x27bd,0x2a0a,0x2c65,0x2ecc,0x3140,0x33c0,0x364c,
	0x38e3,0x3b85,0x3e32,0x40e8,0x43a9,0x4673,0x4946,0x4c21,
	0x4f04,0x51ef,0x54e0,0x57d9,0x5ad8,0x5ddc,0x60e6,0x63f4,
	0x6707,0x6a1e,0x6d38,0x7054,0x7374,0x7695,0x79b8,0x7cdb,
};



#define TIMER_PERIOD_MSEC   10U

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void arrayInit(void);

/*******************************************************************************
* Global Variables
********************************************************************************/
int16_t adcResult0;
/* This array holds all the compare values that will be loaded onto the PWM
*  to create the breathing pattern */
uint32_t compareVal[ARRAY_SIZE];



void timer_interrupt_handler(void)
{
    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(USER_TIMER_HW, USER_TIMER_NUM, CY_TCPWM_INT_ON_TC );

    /* Toggle the LED */
    //Cy_GPIO_Inv(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_NUM);

    /* Start the continuous conversion */
	Cy_SAR_StartConvert(SAR0, CY_SAR_START_CONVERT_SINGLE_SHOT);


	//Cy_TCPWM_PWM_SetCompare0(USER_PWM_HW, USER_PWM_NUM, sine[i]);

}
/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM0 Plus CPU. It does the following functions -
*    1. Generate the compare array
*    2. Initialize and enable the PWM block
*    3. Connect PWM overflow output trigger to DMA input trigger
*    4. Set DMA ping descriptor source address as the compare array
*    5. Set DMA ping descriptor destination as TCPWM compare register
*    6. Initialize and enable the DMA channel and DMA block
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    cy_stc_sysint_t intrCfg =
    {
        /*.intrSrc =*/ USER_TIMER_IRQ,    /* Interrupt source is Timer interrupt */
        /*.intrPriority =*/ 3UL            /* Interrupt priority is 3 */
    };

    result = Cy_SysInt_Init(&intrCfg, timer_interrupt_handler);
    if(result != CY_SYSINT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable Interrupt */
    NVIC_EnableIRQ(intrCfg.intrSrc);


    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
           CY_RETARGET_IO_BAUDRATE);

    /*Enable Opamp0*/
	Cy_CTB_Enable(CTBM0);
	/* Initialize the SAR ADC with the device configurator generated structure*/
	result = Cy_SAR_Init(SAR0, &ADC_0_config);
	if (result != CY_SAR_SUCCESS)
	{
		CY_ASSERT(0);
	}
	/* Enable the SAR ADC */
	Cy_SAR_Enable(SAR0);

    /* Initialize the compareVal array */
    arrayInit();

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize PWM using the config structure generated using device configurator*/
    if (CY_TCPWM_SUCCESS != Cy_TCPWM_PWM_Init(USER_PWM_HW, USER_PWM_NUM, &USER_PWM_config))
    {
        CY_ASSERT(0);
    }

    /* Enable the initialized PWM */
    Cy_TCPWM_PWM_Enable(USER_PWM_HW, USER_PWM_NUM);

    /* Start the PWM */
    Cy_TCPWM_TriggerReloadOrIndex(USER_PWM_HW, USER_PWM_MASK);


    /* Configure DMA Descriptor to change the PWM compare value per breathingLed array */
    USER_DMA_ping_config.srcAddress = &sine[0];//&compareVal[0];
    USER_DMA_ping_config.dstAddress = (void *) &(USER_PWM_CNT->CC);
    /* Initialize the ping descriptor for channel 0 */
    if (CY_DMAC_SUCCESS != Cy_DMAC_Descriptor_Init(DMAC, CHANN_NUM, CY_DMAC_DESCRIPTOR_PING, &USER_DMA_ping_config))
    {
        CY_ASSERT(0);
    }
    /* Setup and enable DMAC channel 0 */
    if (CY_DMAC_SUCCESS != Cy_DMAC_Channel_Init(DMAC, CHANN_NUM, &USER_DMA_channel_config))
    {
        CY_ASSERT(0);
    }
    Cy_DMAC_Channel_Enable(DMAC, 0UL);

    /* Enable DMA hardware block */
    Cy_DMAC_Enable(USER_DMA_HW);


    /* Initialize the TCPWM component in timer/counter mode. The return value of the
	 * function indicates whether the arguments are valid or not.
	 */
	result = Cy_TCPWM_Counter_Init(USER_TIMER_HW, USER_TIMER_NUM, &USER_TIMER_config);
	if(result != CY_TCPWM_SUCCESS)
	{
		CY_ASSERT(0);
	}

	Cy_TCPWM_Counter_Enable(USER_TIMER_HW, USER_TIMER_NUM);

	/* Check if the desired interrupt is enabled prior to triggering */
	if (0UL != (CY_TCPWM_INT_ON_TC & Cy_TCPWM_GetInterruptMask(USER_TIMER_HW, USER_TIMER_NUM)))
	{
	   Cy_TCPWM_SetInterrupt(USER_TIMER_HW, USER_TIMER_NUM, CY_TCPWM_INT_ON_TC);
	}

	/* Set the timer period in milliseconds. To count N cycles, period should be
	 * set to N-1.
	 */
	Cy_TCPWM_Counter_SetPeriod(USER_TIMER_HW, USER_TIMER_NUM, TIMER_PERIOD_MSEC-1 );

	/* Trigger a software start on the counter instance. This is required when
	 * no other hardware input signal is connected to the component to act as
	 * a trigger source.
	 */
	Cy_TCPWM_TriggerStart(USER_TIMER_HW, USER_TIMER_MASK);


    printf("\x1b[2J\x1b[;H");
    printf("-----------------------------------------------------------\r\n");
    printf("*****START******\r\n");
    printf("-----------------------------------------------------------\r\n\n");


    for (;;)
    {
    	/* Wait till the sample is ready */
    	Cy_SAR_IsEndConversion(SAR0, CY_SAR_WAIT_FOR_RESULT);
    	/* Get the result from Input 0 */
    	adcResult0 = Cy_SAR_GetResult16(SAR0, 0);

    	printf("%d\r\n", Cy_SAR_CountsTo_mVolts(SAR0, 0, adcResult0));
    }
}

/*******************************************************************************
* Function Name: arrayInit
********************************************************************************
* Summary:
* This function creates an array with increasing and decreasing compare values to
* generate the breathing effect.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void arrayInit(void)
{
    uint32_t i=0u;
    /* Creating the array with increasing and decreasing compare value to generate the breathing */
    compareVal[i] = 0;

    for(i=1u; i < ARRAY_SIZE>>1; i++)
    {
        /* Increase compare values */
        compareVal[i] = compareVal[i-1u] + BREATHING_CHANGE;
    }
    for(i=(ARRAY_SIZE>>1); i < ARRAY_SIZE; i++)
    {
        /* Decrease compare value */
        compareVal[i] = compareVal[i-1u] - BREATHING_CHANGE;
    }
}

/* [] END OF FILE */
