#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <hal_i2c_m_sync.h>

extern struct i2c_m_sync_desc       I2C_0;

void I2C_0_init(void);

#endif