/*******************************************************************************
 CLOCK PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_clock.c

  Summary:
    CLOCK PLIB Implementation File.

  Description:
    None

*******************************************************************************/

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

#include "plib_clock.h"
#include "device.h"

#if defined(__SAML21J18B__)
#define GCLK_CLKCTRL_ID_Pos GCLK_PCHCTRL_GEN_Pos
#define GCLK_CLKCTRL_GEN_GCLK0 GCLK_PCHCTRL_GEN_GCLK0
#define GCLK_CLKCTRL_CLKEN GCLK_PCHCTRL_CHEN
#endif
//#include "interrupts.h" Commented by manuel

// static void SYSCTRL_Initialize(void)
// {
// 
//     /* Configure 8MHz Oscillator */
//     SYSCTRL_REGS->SYSCTRL_OSC8M = (SYSCTRL_REGS->SYSCTRL_OSC8M & (SYSCTRL_OSC8M_CALIB_Msk | SYSCTRL_OSC8M_FRANGE_Msk)) | SYSCTRL_OSC8M_ENABLE_Msk | SYSCTRL_OSC8M_PRESC(0x0U) ;
// 
//     while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_OSC8MRDY_Msk) != SYSCTRL_PCLKSR_OSC8MRDY_Msk)
// {
//         /* Waiting for the OSC8M Ready state */
//     }
//     /****************** XOSC32K initialization  ******************************/
// 
//     /* Configure 32K External Oscillator */
//     SYSCTRL_REGS->SYSCTRL_XOSC32K = SYSCTRL_XOSC32K_STARTUP(0U) | SYSCTRL_XOSC32K_ENABLE_Msk | SYSCTRL_XOSC32K_EN32K_Msk | SYSCTRL_XOSC32K_XTALEN_Msk;
//     while(!((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_XOSC32KRDY_Msk) == SYSCTRL_PCLKSR_XOSC32KRDY_Msk))
//     {
//         /* Waiting for the XOSC32K Ready state */
//     }
// 
//     SYSCTRL_REGS->SYSCTRL_OSC32K = 0x0U;
// }




// static void GCLK0_Initialize(void)
//     {
// 
//     GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(6U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(0U);
// 
//     GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_DIV(2U) | GCLK_GENDIV_ID(0U);
//     while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
//     {
//         /* wait for the Generator 0 synchronization */
//     }
// }


// static void GCLK1_Initialize(void)
//     {
//     GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(6U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(1U);
// 
//     GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_DIV(4U) | GCLK_GENDIV_ID(1U);
//     while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
//     {
//         /* wait for the Generator 1 synchronization */
//     }
// }


// static void GCLK3_Initialize(void)
// {
//     GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(5U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(3U);
// 
//     GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_DIV(32U) | GCLK_GENDIV_ID(3U);
//     while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
//     {
//         /* wait for the Generator 3 synchronization */
//     }
// }

void CLOCK_Initialize (void)
{
    /* Function to Initialize the Oscillators */
//     SYSCTRL_Initialize();

//     GCLK3_Initialize();
//     GCLK0_Initialize();
//     GCLK1_Initialize();


    /* Selection of the Generator and write Lock for SERCOM4_CORE */
//     GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(24U) | GCLK_CLKCTRL_GEN(0x0U)  | GCLK_CLKCTRL_CLKEN_Msk;

	#if defined(__SAML21J18B__)
		GCLK->PCHCTRL[28].reg = GCLK_PCHCTRL_GEN(0x0U)  | GCLK_PCHCTRL_CHEN;

		while ((GCLK->PCHCTRL[28].reg & GCLK_PCHCTRL_CHEN) != GCLK_PCHCTRL_CHEN)
		{
			/* Wait for synchronization */
		}
	#else
	    //Utilisation du même generateur que le bus SPI
	    /* Selection of the Generator and write Lock for TC3 TCC2 */
	    hri_gclk_write_CLKCTRL_reg( GCLK, (TC3_GCLK_ID << GCLK_CLKCTRL_ID_Pos) | //GCLK_TC3
	    GCLK_CLKCTRL_GEN_GCLK0  | //Generic clock generator 0
	    GCLK_CLKCTRL_CLKEN );   //enable GCL

	    /* Configure the APBC Bridge Clocks */
	    hri_pm_set_APBCMASK_TC3_bit(PM);
	#endif



}
