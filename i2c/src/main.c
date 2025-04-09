#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#define I2C0_NODE DT_NODELABEL(lum_sensor)

#if !DT_NODE_HAS_STATUS_OKAY(I2C0_NODE)
#error "Unsupported board: sensor devicetree alias is not defined"
#endif

static const struct i2c_dt_spec device = I2C_DT_SPEC_GET(I2C0_NODE);
static const struct gpio_dt_spec vcc_sensor = GPIO_DT_SPEC_GET(DT_ALIAS(vcc_sensor), gpios);

int routine_read_temp(const struct i2c_dt_spec temp_device)
{
    while (1)
    {
        uint8_t sensor_reg = 0x00;
        uint8_t temp_reading[2]= {0};	
        int ret = i2c_write_read_dt(&temp_device, &sensor_reg, 1, &temp_reading[0], 2);
        if(ret != 0){
            printk("Failed to write/read I2C device address %x at Reg. %x \n\r", temp_device.addr, sensor_reg);
            return 1;
        }
        printk("Temperature: %d.%d\n", temp_reading[0], (temp_reading[1] >> 7)? 5 : 0);
        k_msleep(1000);
    }
    return 0;
}

int init_lum_sensor(const struct i2c_dt_spec lum_device)
{
    // Init Vcc sensor to then configure it
    if (!gpio_is_ready_dt(&vcc_sensor))
        return 1;
    if (gpio_pin_configure_dt(&vcc_sensor, GPIO_OUTPUT_ACTIVE) < 0) 
        return 1;

    k_msleep(200);

    uint8_t config[3] = {0x00, 0x00, 0x00}; // { commmand config, config LSB, config MSB }, config => gain 1, IT => 100ms, Pers => 1

    if (i2c_write_dt(&lum_device, config, 3) != 0)
        return 1;
    return 0;
}

int routine_read_lum(const struct i2c_dt_spec lum_device)
{
    uint16_t light;
    while (1)
    {
        k_msleep(1000);
        uint8_t sensor_reg = 0x04;
        uint8_t lum_reading[2]= {0};	
        int ret = i2c_write_read_dt(&lum_device, &sensor_reg, 1, &lum_reading[0], 2);
        if(ret != 0){
            printk("Failed to write/read I2C device address %x at Reg. %x \n\r", lum_device.addr, sensor_reg);
            return 1;
        }
        light = ((uint16_t)lum_reading[1] << 8) + lum_reading[0];
        printk("Light sensor: %d\n", light);
    }
    return 0;
}

int main()
{
    if (!device_is_ready(device.bus)) {
        printf("I2C bus %s is not ready!\n\r",device.bus->name);
        return 1;
    }
    if (init_lum_sensor(device) == 1){
        printf("Init i2c error\n");
        return 1;
    }
    // routine_read_temp(device);
    routine_read_lum(device);
    return 0;
}