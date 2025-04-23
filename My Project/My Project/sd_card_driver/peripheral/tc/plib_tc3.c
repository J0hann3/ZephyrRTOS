/*******************************************************************************
  Timer/Counter(TC3) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_tc3.c

  Summary
    TC3 PLIB Implementation File.

  Description
    This file defines the interface to the TC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "plib_tc3.h"
#if defined(__SAML21J18B__)
#define GCLK_CLKCTRL_ID_Pos GCLK_GENCTRL_SRC_Pos
#define TC_STATUS_SYNCBUSY TC_SYNCBUSY_STATUS
#define TC_CTRLA_WAVEGEN_MPWM TC_WAVE_WAVEGEN_MPWM
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

static TC_TIMER_CALLBACK_OBJ TC3_CallbackObject;

// *****************************************************************************
// *****************************************************************************
// Section: TC3 Implementation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Initialize the TC module in Timer mode */
void TC3_TimerInitialize( void )
{
    /* Reset TC */
//    hri_tc_set_CTRLA_SWRST_bit(TC3);
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;

//     hri_tc_wait_for_sync(TC4);
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
    {
        /* Wait for Write Synchronization */
    }

    /* Configure counter mode & prescaler */
//     TC3_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_WAVEGEN_MPWM ; //remplacé par la ligne ci-dessous
 	hri_tc_write_CTRLA_reg( TC3, TC_CTRLA_MODE_COUNT16 |	//Choose 16 bits mode
 	                             TC_CTRLA_PRESCALER_DIV1	|	//GCLK0
 	                             TC_CTRLA_WAVEGEN_MPWM	// Compare with CC0
	                            );

    /* Configure timer period */
//     TC3_REGS->COUNT16.TC_CC[0U] = 4000U;
    TC3->COUNT16.CC[0].reg = 4000U;

    /* Clear all interrupt flags */
    TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_MASK;

    TC3_CallbackObject.callback = NULL;
    /* Enable interrupt*/
    TC3->COUNT16.INTENSET.reg = TC_INTENSET_MC1;

//     hri_tc_wait_for_sync(TC4);
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Enable the TC counter */
void TC3_TimerStart( void )
{
//     hri_tc_set_CTRLA_ENABLE_bit(TC4);
    TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
//     hri_tc_wait_for_sync(TC4);
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Disable the TC counter */
void TC3_TimerStop( void )
{
    TC3->COUNT16.CTRLA.reg = ((TC3->COUNT16.CTRLA.reg) &(uint16_t)(~TC_CTRLA_ENABLE));
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
    {
        /* Wait for Write Synchronization */
    }
}

uint32_t TC3_TimerFrequencyGet( void )
{
    return (uint32_t)(4000000UL);
}

void TC3_TimerCommandSet(TC_COMMAND command)
{
    TC3->COUNT16.CTRLBSET.reg = (uint8_t)command << TC_CTRLBSET_CMD_Pos;
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
    {
        /* Wait for Write Synchronization */
    }    
}

/* Get the current timer counter value */
uint16_t TC3_Timer16bitCounterGet( void )
{
    /* Write command to force COUNT register read synchronization */
	#if !defined(__SAML21J18B__) /*Il n'a pas de READREQ Register sur un SAML*/
		TC3->COUNT16.READREQ.reg = TC_READREQ_RREQ | (uint16_t)TC_COUNT16_COUNT_OFFSET;
	#endif

    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
    {
        /* Wait for Write Synchronization */
    }
	
    /* Read current count value */
    return (uint16_t)TC3->COUNT16.COUNT.reg;
}

/* Configure timer counter value */
void TC3_Timer16bitCounterSet( uint16_t count )
{
    TC3->COUNT16.COUNT.reg = count;

    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Configure timer period */
void TC3_Timer16bitPeriodSet( uint16_t period )
{
    TC3->COUNT16.CC[0].reg = period;
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Read the timer period value */
uint16_t TC3_Timer16bitPeriodGet( void )
{
    /* Write command to force CC register read synchronization */
	#if !defined(__SAML21J18B__) /*Il n'a pas de READREQ Register sur un SAML*/
		TC3->COUNT16.READREQ.reg = TC_READREQ_RREQ | (uint16_t)TC_COUNT16_COUNT_OFFSET; 
	

	while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
	{
		/* Wait for Write Synchronization */
	}
	#endif
	
	return (uint16_t)TC3->COUNT16.CC[0U].reg;
	
}

void TC3_Timer16bitCompareSet( uint16_t compare )
{
    TC3->COUNT16.CC[1].reg = compare;
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY)!= 0U)
    {
        /* Wait for Write Synchronization */
    }
}


/* Register callback function */
void TC3_TimerCallbackRegister( TC_TIMER_CALLBACK callback, uintptr_t context )
{
    TC3_CallbackObject.callback = callback;

    TC3_CallbackObject.context = context;
}

/* Timer Interrupt handler */
void TC3_Handler( void )
{
    TC_TIMER_STATUS status;
    status = (TC_TIMER_STATUS) (TC3->COUNT16.INTFLAG.reg);
    /* Clear interrupt flags */
    TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_MASK;
    if(TC3_CallbackObject.callback != NULL)
    {
        TC3_CallbackObject.callback(status, TC3_CallbackObject.context);
    }
}

