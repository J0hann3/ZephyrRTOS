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
    const struct gpio_dt_spec vcc_sd = GPIO_DT_SPEC_GET(DT_NODELABEL(vcc_sd), gpios);
    
    measure_t current_measure = {.TEMP_HUM_SENSOR_EN = 1, .LUM_SENSOR_EN = 0};

    if (!device_is_ready(temp_device.bus))
    {
        return 1; 
    }
    // if (!device_is_ready(lum_device.bus))
    // {
    //     return 1;
    // }
    // if (!gpio_is_ready_dt(&vcc_sensor))
    // {
    //     return 1;
    // }
    if (!gpio_is_ready_dt(&vcc_sd))
    {
        return 1;
    }

	measures_logger_init(1);
    
    while (1)
    {
        k_sleep(K_MSEC(1000));

        // if (init_and_read_lum_sensor(&lum_device, &vcc_sensor, &current_measure.brightness) == 1)
        //     return 1;
        if (read_temp_sensor(&temp_device, &current_measure.temperature,
            &current_measure.humidity) == 1)
            return 1;
        measures_logger_write(&current_measure);
        size_t count = measures_logger_count();
        uint16_t size = measures_logger_get_size();
        if (count >= size)
        {
            write_in_sd_card(&fat_fs, &mp);
        }
    }
    return 0;
}