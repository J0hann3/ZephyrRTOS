#include <zephyr/drivers/lora.h>
#include <zephyr/lorawan/lorawan.h>
#include <zephyr/logging/log.h>
#include "spi.h"

LOG_MODULE_REGISTER(lorawan_test);

#define LORAWAN_DEV_EUI		{ 0x70, 0xB3, 0xD5, 0x40, 0xF6, 0x06,\
                    0x38, 0x02 }
#define LORAWAN_APP_EUI		{  0x70, 0xB3, 0xD5, 0x40, 0xFC, 0xAD,\
                    0x56, 0xDF }
#define LORAWAN_APP_KEY		{ 0xF2, 0x98, 0x99, 0xFA, 0x83, 0x64,\
                    0xA8, 0x5F, 0xE0, 0x8E, 0xA6, 0x55, 0x3B, 0xF9, 0xE1, 0x45 }

static void dl_callback(uint8_t port, uint8_t flags, int16_t rssi, int8_t snr, uint8_t len,
			const uint8_t *hex_data)
{
	printf("Port %d, Pending %d, RSSI %ddB, SNR %ddBm, Time %d, size %d\n", port,
		flags & LORAWAN_DATA_PENDING, rssi, snr, !!(flags & LORAWAN_TIME_UPDATED), len);
	if (hex_data) {
		LOG_HEXDUMP_INF(hex_data, len, "Payload: ");
	}
}

int lora_test_join()
{
    uint8_t dev_eui[] = LORAWAN_DEV_EUI;
	uint8_t app_eui[] = LORAWAN_APP_EUI;
	uint8_t app_key[] = LORAWAN_APP_KEY;
    struct lorawan_join_config config_lora;
    char data[] = "send message";
    struct lorawan_downlink_cb downlink_cb = {
		.port = LW_RECV_PORT_ANY,
		.cb = dl_callback
	};
    int ret;

    config_lora.mode = LORAWAN_ACT_OTAA;
    config_lora.dev_eui = dev_eui;
    config_lora.otaa.join_eui = app_eui;
    config_lora.otaa.app_key = app_key;
    config_lora.otaa.nwk_key = app_key;
    config_lora.otaa.dev_nonce = 0;

    spi_init();

    ret = lorawan_start();
    if (ret < 0) {
		printf("lorawan_start failed: %d", ret);
        spi_deinit();
		return 1;
	}
    
    lorawan_register_downlink_callback(&downlink_cb);
    ret = lorawan_join(&config_lora);
    if (ret < 0) {
		printf("lorawan_join_network failed: %d", ret);
        spi_deinit();
		return 1;
	}
    while (1)
    {
        ret = lorawan_send(2, data, sizeof(data),
                       LORAWAN_MSG_UNCONFIRMED);
        if (ret < 0) {
            printf("lorawan_send failed: %d", ret);
        }
        k_msleep(10000);
    }

    spi_deinit();
    return 0;
}

void lora_cw()
{
    const struct device *lora = DEVICE_DT_GET(DT_NODELABEL(lora0));

    spi_init();
    printf("begin continuous wave\n");
    int ret = lora_test_cw(lora, 868000000, 14, 2);     //Use EU868 norme for cw
    if (ret != 0)
    {
        printf("failed continuous wave lora: %d\n", ret);
        spi_deinit();
        return;
    }
    spi_deinit();
    printf("end continuous wave\n");
}