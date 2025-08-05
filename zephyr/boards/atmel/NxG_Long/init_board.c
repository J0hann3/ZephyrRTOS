#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <soc.h>
#include <stdio.h>

static inline void osc8m_init(void)
{
	uint32_t reg;

	/* Save calibration */
	reg = SYSCTRL->OSC8M.reg
	    & (SYSCTRL_OSC8M_FRANGE_Msk | SYSCTRL_OSC8M_CALIB_Msk);

	SYSCTRL->OSC8M.reg = reg
				| SYSCTRL_OSC8M_PRESC(SYSCTRL_OSC8M_PRESC_0_Val)
				| SYSCTRL_OSC8M_ENABLE;
	
	while (!SYSCTRL->PCLKSR.bit.OSC8MRDY) {
	}

	SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ONDEMAND;
}

static inline void xosc32k_init(void)
{
	SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_STARTUP(0x0)
				| SYSCTRL_XOSC32K_RUNSTDBY
				| SYSCTRL_XOSC32K_EN32K
				| SYSCTRL_XOSC32K_XTALEN
				| SYSCTRL_XOSC32K_ENABLE;
				 
	while (!SYSCTRL->PCLKSR.bit.XOSC32KRDY) {
	}

	SYSCTRL->XOSC32K.reg |= SYSCTRL_XOSC32K_ONDEMAND;
}

static void gclk_connect(uint8_t gclk, uint32_t src, uint32_t div, uint32_t flags)
{
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(gclk)
			 | GCLK_GENDIV_DIV(div);

	while (GCLK->STATUS.bit.SYNCBUSY) {
	}

	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(gclk)
			  | GCLK_GENCTRL_GENEN
			  | src
			  | flags;

	while (GCLK->STATUS.bit.SYNCBUSY) {
	}
}

static inline void flash_waitstates_init(void)
{
	NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS(CONFIG_SOC_ATMEL_SAMD_NVM_WAIT_STATES);
}

static int soc_reset_hook(void)
{
	osc8m_init();
	xosc32k_init();
	flash_waitstates_init();
	gclk_connect(0, GCLK_GENCTRL_SRC_OSC8M, 1, 0);
	gclk_connect(1, GCLK_GENCTRL_SRC_OSC8M, 4, 0);
	gclk_connect(2, GCLK_GENCTRL_SRC_OSC8M, 16, 0);
	gclk_connect(3, GCLK_GENCTRL_SRC_XOSC32K, 32, GCLK_GENCTRL_RUNSTDBY);
	gclk_connect(4, GCLK_GENCTRL_SRC_OSC8M, 128, 0);
	return 0;
}

SYS_INIT(soc_reset_hook, PRE_KERNEL_1, 0);
