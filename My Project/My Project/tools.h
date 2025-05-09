#ifndef TOOLS_H

extern struct spi_m_sync_descriptor Spi0;

void SpiInit();
void SpiDeInit();
void spi_go_to_sleep();
void HwTimerInit(void);
void HwTimerDisable(void);

#endif