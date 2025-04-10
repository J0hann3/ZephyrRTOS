#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#define I2C_LUM_NODE DT_NODELABEL(lum_sensor)
#define I2C_TEMP_NODE DT_NODELABEL(temp_sensor)

#define VCC_SENSOR_NODE DT_ALIAS(vcc_sensor)

#if !DT_NODE_HAS_STATUS_OKAY(VCC_SENSOR_NODE)
#error "Unsupported board: vcc sensor devicetree alias is not defined"
#endif

#if (!DT_NODE_HAS_STATUS_OKAY(I2C_LUM_NODE) || !DT_NODE_HAS_STATUS_OKAY(I2C_TEMP_NODE))
#error "Unsupported board: sensor devicetree alias is not defined"
#endif

uint8_t read_temp_sensor(const struct i2c_dt_spec *temp_device, uint16_t *temp, uint16_t *humidity)
{
    uint8_t sensor_reg = 0xFD;
    uint8_t reading[6]= {0};

    // function i2c_write_read_dt doesn't generate a stop condition before switching to a read, no supported by all i2c sensor
    if(i2c_write_dt(temp_device, &sensor_reg, 1) != 0){
        printf("Failed to write/read I2C device address %x at Reg. %x \n\r", temp_device->addr, sensor_reg);
        return 1;
    }
    k_msleep(10);
    i2c_read_dt(temp_device, reading, 6);
    (*temp) = reading[0] * 256 + reading[1];
    (*temp) = -45 + 175 * (*temp) / 65535;

    (*humidity) = reading[3] * 256 + reading[4];
    (*humidity) = -6 + 125 * (*humidity) / 65535;
    printf("Temperature: %d, Humidity %d\n", (*temp), (*humidity));
    return 0;
}


int routine_read_temp(const struct i2c_dt_spec *temp_device)
{
    uint16_t temp;
    uint16_t humidity;

    while (1)
    {
        k_msleep(1000);
        read_temp_sensor(temp_device, &temp, &humidity);
    }
    return 0;
}

int init_lum_sensor(const struct i2c_dt_spec *lum_device, const struct gpio_dt_spec *vcc_sensor)
{
    // Init Vcc sensor to then configure i2c device(light sensor)
    if (gpio_pin_configure_dt(vcc_sensor, GPIO_OUTPUT_ACTIVE) < 0) 
        return 1;

    k_msleep(5);
    
    uint8_t config[3] = {0x00, 0x00, 0x00}; // { commmand config, config LSB, config MSB }, config => gain 1, IT => 100ms, Pers => 1

    if (i2c_write_dt(lum_device, config, 3) != 0)
        return 1;
    return 0;
}

uint8_t read_lum_sensor(const struct i2c_dt_spec *lum_device, uint16_t *light)
{
    uint8_t sensor_reg = 0x04;
    uint8_t lum_reading[2]= {0};

    int ret = i2c_write_read_dt(lum_device, &sensor_reg, 1, &lum_reading[0], 2);
    if(ret != 0){
        printf("Failed to write/read I2C device address %x at Reg. %x \n\r", lum_device->addr, sensor_reg);
        return 1;
    }
    *light = ((uint16_t)lum_reading[1] << 8) + lum_reading[0];
    printf("Light sensor: %d\n", *light);
    return 0;
}

uint8_t init_and_read_lum_sensor(const struct i2c_dt_spec *lum_device, const struct gpio_dt_spec *vcc_sensor, uint16_t *light)
{
	if (init_lum_sensor(lum_device, vcc_sensor) == 1){
		printf("Init i2c error\n");
		return 1;
	}
    k_msleep(150);
	if (read_lum_sensor(lum_device, light))
		return 1;
	
    gpio_pin_configure_dt(vcc_sensor, GPIO_INPUT | GPIO_PULL_UP);
	return 0;
}

int routine_read_lum(const struct i2c_dt_spec *lum_device)
{
    uint16_t light;
    while (1)
    {
        k_msleep(1000);
        if (read_lum_sensor(lum_device, &light) == 1)
            return 1;
    }
    return 0;
}

int main()
{
    const struct i2c_dt_spec lum_device = I2C_DT_SPEC_GET(I2C_LUM_NODE);
    const struct i2c_dt_spec temp_device = I2C_DT_SPEC_GET(I2C_TEMP_NODE);
    const struct gpio_dt_spec vcc_sensor = GPIO_DT_SPEC_GET(DT_ALIAS(vcc_sensor), gpios);
    uint16_t light;
    uint16_t temp;
    uint16_t humidity;

    k_msleep(50);
    if (!device_is_ready(temp_device.bus)) 
        return 1;
    if (!device_is_ready(lum_device.bus)) 
        return 1;
    if (!gpio_is_ready_dt(&vcc_sensor))
        return 1;

    while (1)
    {
        k_msleep(1000);
        if (init_and_read_lum_sensor(&lum_device, &vcc_sensor, &light) == 1)
            return 1;
        if (read_temp_sensor(&temp_device, &temp, &humidity) == 1)
            return 1;
    }
    return 0;
}