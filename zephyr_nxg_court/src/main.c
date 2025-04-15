// #include <zephyr/drivers/i2c.h>
// #include <zephyr/drivers/gpio.h>
#include "lum_sensor.h"
#include "temp_hum_sensor.h"
#include "sd_card.h"

static FATFS fat_fs;

static struct fs_mount_t mp = {
    .type = FS_FATFS,
    .fs_data = &fat_fs,
    .mnt_point = "/SD:",
};

int main()
{
    // const struct i2c_dt_spec lum_device = I2C_DT_SPEC_GET(I2C_LUM_NODE);
    const struct i2c_dt_spec temp_device = I2C_DT_SPEC_GET(I2C_TEMP_NODE);
    // const struct gpio_dt_spec vcc_sensor = GPIO_DT_SPEC_GET(DT_NODELABEL(vcc_sensor), gpios);
    
    // uint16_t light;
    uint16_t temp;
    uint16_t humidity;

    if (!device_is_ready(temp_device.bus)) 
        return 1;
    // if (!device_is_ready(lum_device.bus)) 
    //     return 1;
    // if (!gpio_is_ready_dt(&vcc_sensor))
    //     return 1;
    test(&fat_fs, &mp);
    while (1)
    {
        k_sleep(K_MSEC(1000));
        // if (init_and_read_lum_sensor(&lum_device, &vcc_sensor, &light) == 1)
            // return 1;
        if (read_temp_sensor(&temp_device, &temp, &humidity) == 1)
            return 1;
    }
    return 0;
}