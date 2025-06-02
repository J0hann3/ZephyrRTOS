#include "lum_sensor.h"
#include "measures_logger.h"
#include "temp_hum_sensor.h"
#include "sd_card.h"

static FATFS fat_fs;

static struct fs_mount_t mp = {
    .type = FS_FATFS,
    .fs_data = &fat_fs,
    .mnt_point = "/SD:",
};


bool is_board_ready(const struct i2c_dt_spec *lum_device, const struct i2c_dt_spec *temp_device)
{
    if (!device_is_ready(temp_device->bus))
        return false; 
    if (!device_is_ready(lum_device->bus))
        return false;
    return true;
}

int main()
{
    const struct i2c_dt_spec lum_device = I2C_DT_SPEC_GET(I2C_LUM_NODE);
    const struct i2c_dt_spec temp_device = I2C_DT_SPEC_GET(I2C_TEMP_NODE);
    
    measure_t current_measure = {.TEMP_HUM_SENSOR_EN = 0, .LUM_SENSOR_EN = 0};
    
    if (!is_board_ready(&lum_device, &temp_device))
    {
    #ifdef DEBUG
        printf("Board is not ready\n");
    #endif
        return 1;
    }

	measures_logger_init(1);
    
    while (1)
    {
        k_sleep(K_MSEC(5000));
        printf("loop\n");

        if (current_measure.LUM_SENSOR_EN && init_and_read_lum_sensor(&lum_device,
                    &current_measure.brightness) == 1)
            return 1;
        if (current_measure.TEMP_HUM_SENSOR_EN && read_temp_sensor(&temp_device, &current_measure.temperature,
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