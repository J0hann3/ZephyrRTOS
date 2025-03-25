#include <zephyr/drivers/i2c.h>

#define I2C0_NODE DT_NODELABEL(temperature_sensor)

static const struct i2c_dt_spec device = I2C_DT_SPEC_GET(I2C0_NODE);

int main()
{
    if (!device_is_ready(device.bus)) {
        printk("I2C bus %s is not ready!\n\r",device.bus->name);
        return 1;
    }
    
    while (1)
    {
        uint8_t sensor_reg = 0x00;
        uint8_t temp_reading[2]= {0};	
        int ret = i2c_write_read_dt(&device, &sensor_reg, 1, &temp_reading[0], 2);
        if(ret != 0){
            printk("Failed to write/read I2C device address %x at Reg. %x \n\r", device.addr, sensor_reg);
            return 1;
        }
        printk("Temperature: %d.%d\n", temp_reading[0], (temp_reading[1] >> 7)? 5 : 0);
        k_msleep(1000);
    }

    return 0;
}