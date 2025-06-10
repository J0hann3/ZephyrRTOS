/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 1995 - 2021 SEGGER Microcontroller GmbH             *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       SEGGER SystemView * Real-time application analysis           *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* SEGGER strongly recommends to not make any changes                 *
* to or modify the source code of this software in order to stay     *
* compatible with the SystemView and RTT protocol, and J-Link.       *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* condition is met:                                                  *
*                                                                    *
* o Redistributions of source code must retain the above copyright   *
*   notice, this condition and the following disclaimer.             *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************
*                                                                    *
*       SystemView version: 3.32                                    *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER_SYSVIEW_Config_NoOS.c
Purpose : Sample setup configuration of SystemView without an OS for Cortex-M0.
Revision: $Rev: 18540 $
*/
#include "SEGGER_SYSVIEW.h"
#include "SEGGER_SYSVIEW_Conf.h"

#include <peripheral_clk_config.h>
#include <hpl_time_measure.h>
#include <stdio.h>

extern unsigned int SEGGER_SYSVIEW_TickCnt;

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/
#define SCB_ICSR  (*(volatile U32*) (0xE000ED04uL)) // Interrupt Control State Register
#define SCB_ICSR_PENDSTSET_MASK     (1UL << 26)     // SysTick pending bit
#define SYST_RVR  (*(volatile U32*) (0xE000E014uL)) // SysTick Reload Value Register
#define SYST_CVR  (*(volatile U32*) (0xE000E018uL)) // SysTick Current Value Register

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
// The application name to be displayed in SystemViewer
#define SYSVIEW_APP_NAME        "Nxg_court"

// The target device name
#define SYSVIEW_DEVICE_NAME     "nxg_court atmel_sam0 arm"

// Frequency of the timestamp. Must match SEGGER_SYSVIEW_Conf.h
#define SYSVIEW_TIMESTAMP_FREQ  (CONF_CPU_FREQUENCY)

// System Frequency. SystemcoreClock is used in most CMSIS compatible projects.
#define SYSVIEW_CPU_FREQ        (CONF_CPU_FREQUENCY)

// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE        (0x10000000)

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/                                // Cycle counter enable bit

/********************************************************************* 
*
*       _cbSendSystemDesc()
*
*  Function description
*    Sends SystemView description strings.
*/
static void _cbSendSystemDesc(void) {
  SEGGER_SYSVIEW_SendSysDesc("N="SYSVIEW_APP_NAME",O=NoOS,D="SYSVIEW_DEVICE_NAME);
  SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/
void SEGGER_SYSVIEW_Conf(void) {
  SysTick->CTRL |= (1 << SysTick_CTRL_TICKINT_Pos);
  SEGGER_SYSVIEW_Init(SYSVIEW_TIMESTAMP_FREQ, SYSVIEW_CPU_FREQ, 
                      0, _cbSendSystemDesc);
  SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);
}

/*********************************************************************
*
*       SEGGER_SYSVIEW_X_GetTimestamp()
*
* Function description
*   Returns the current timestamp in ticks using the system tick 
*   count and the SysTick counter.
*   All parameters of the SysTick have to be known and are set via
*   configuration defines on top of the file.
*
* Return value
*   The current timestamp.
*
* Additional information
*   SEGGER_SYSVIEW_X_GetTimestamp is always called when interrupts are
*   disabled. Therefore locking here is not required.
*/

static void *hardware;

#if CONF_CPU_FREQUENCY < 1000
#define CPU_FREQ_POWER 3
#elif CONF_CPU_FREQUENCY < 10000
#define CPU_FREQ_POWER 4
#elif CONF_CPU_FREQUENCY < 100000
#define CPU_FREQ_POWER 5
#elif CONF_CPU_FREQUENCY < 1000000
#define CPU_FREQ_POWER 6
#elif CONF_CPU_FREQUENCY < 10000000
#define CPU_FREQ_POWER 7
#elif CONF_CPU_FREQUENCY < 100000000
#define CPU_FREQ_POWER 8
#endif

static inline uint32_t _dummy_get_cycles_for_ms_internal(const uint16_t ms, const uint32_t freq, const uint8_t power)
{
	switch (power) {
	case 8:
		return (ms * (freq / 100000) + 2) / 3 * 100;
	case 7:
		return (ms * (freq / 10000) + 2) / 3 * 10;
	case 6:
		return (ms * (freq / 1000) + 2) / 3;
	case 5:
		return (ms * (freq / 100) + 29) / 30;
	case 4:
		return (ms * (freq / 10) + 299) / 300;
	default:
		return (ms * (freq / 1) + 2999) / 3000;
	}
}

static inline uint32_t _dummy_get_cycles_for_us_internal(const uint16_t us, const uint32_t freq, const uint8_t power)
{
	switch (power) {
	case 8:
		return (us * (freq / 100000) + 29) / 30;
	case 7:
		return (us * (freq / 10000) + 299) / 300;
	case 6:
		return (us * (freq / 1000) + 2999) / 3000;
	case 5:
		return (us * (freq / 100) + 29999) / 30000;
	case 4:
		return (us * (freq / 10) + 299999) / 300000;
	default:
		return (us * freq + 2999999) / 3000000;
	}
}

static uint32_t _dummy_get_cycles_for_ms(const uint16_t ms)
{
	return _dummy_get_cycles_for_ms_internal(ms, CONF_CPU_FREQUENCY, CPU_FREQ_POWER);
}

uint32_t _dummy_get_cycles_for_us(const uint16_t us)
{
	return _dummy_get_cycles_for_us_internal(us, CONF_CPU_FREQUENCY, CPU_FREQ_POWER);
}

static void _dummy_delay_cycles(void *const hw, uint32_t cycles)
{
#ifndef _UNIT_TEST_
	(void)hw;
	(void)cycles;
#if defined __GNUC__
	__asm(".syntax unified\n"
	      "__delay:\n"
	      "subs r1, r1, #1\n"
	      "bhi __delay\n"
	      ".syntax divided");
#elif defined __CC_ARM
	__asm("__delay:\n"
	      "subs cycles, cycles, #1\n"
	      "bhi __delay\n");
#elif defined __ICCARM__
	__asm("__delay:\n"
	      "subs r1, r1, #1\n"
	      "bhi __delay\n");
#endif
#endif
}

#ifdef SEGGER_SYSTEM_VIEW

void delay_us(const uint16_t us)
{
	_dummy_delay_cycles(hardware, _dummy_get_cycles_for_us(us));
}
void delay_ms(const uint16_t ms)
{
	_dummy_delay_cycles(hardware, _dummy_get_cycles_for_ms(ms));
}

void SysTick_Handler()
{
  printf("Systick Handler: %d\n", SEGGER_SYSVIEW_TickCnt);
  SEGGER_SYSVIEW_TickCnt++;
}
#endif

U32 SEGGER_SYSVIEW_X_GetTimestamp(void) {
  U32 TickCount;
  U32 Cycles;
  U32 CyclesPerTick;
  //
  // Get the cycles of the current system tick.
  // SysTick is down-counting, subtract the current value from the number of cycles per tick.
  //
  CyclesPerTick = SYST_RVR + 1;
  Cycles = (CyclesPerTick - SYST_CVR);
  //
  // Get the system tick count.
  //
  TickCount = SEGGER_SYSVIEW_TickCnt;
  //
  // If a SysTick interrupt is pending, re-read timer and adjust result
  //
  if ((SCB_ICSR & SCB_ICSR_PENDSTSET_MASK) != 0) {
    Cycles = (CyclesPerTick - SYST_CVR);
    TickCount++;
  }
  Cycles += TickCount * CyclesPerTick;
  printf("cycles: %u\n", Cycles);
  return Cycles;
}

/*********************************************************************
*
*       SEGGER_SYSVIEW_X_GetInterruptId()
*
* Function description
*   Return the currently active interrupt Id,
*   which ist the active vector taken from IPSR[5:0].
*
* Return value
*   The current currently active interrupt Id.
*
* Additional information
*   This function is not used by default, as the active vector can be 
*   read from ICSR instead on Cortex-M0.
*   For Cortex-M0+ devices, change SEGGER_SYSVIEW_GET_INTERRUPT_ID
*   in SEGGER_SYSVIEW_Conf.h to call this function instead.
*/
U32 SEGGER_SYSVIEW_X_GetInterruptId(void) {
  U32 Id;

  __asm volatile ("mrs %0, ipsr"
                  : "=r" (Id)
                  );
  Id &= 0x3F;

  return Id;
}

/*************************** End of file ****************************/
