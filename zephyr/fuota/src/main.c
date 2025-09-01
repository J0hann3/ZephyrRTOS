/*
 * LoRaWAN FUOTA sample application
 *
 * Copyright (c) 2022-2024 Libre Solar Technologies GmbH
 * Copyright (c) 2022-2024 tado GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/lorawan/lorawan.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/settings/settings.h>

LOG_MODULE_REGISTER(lorawan_fuota, CONFIG_LORAWAN_SERVICES_LOG_LEVEL);

/* Customize based on device configuration */
#define LORAWAN_DEV_EUI		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define LORAWAN_JOIN_EUI	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
// #define LORAWAN_APP_KEY		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

// #define LORAWAN_DEV_EUI		{ 0x70, 0xB3, 0xD5, 0x40, 0xFC, 0x05, 0x0D, 0x0D }
// #define LORAWAN_JOIN_EUI	{ 0x70, 0xB3, 0xD5, 0x40, 0xFC, 0x05, 0x0D, 0x00 }
#define LORAWAN_APP_KEY		{ 0x5B, 0xBB, 0x00, 0x36, 0xCF, 0x38, 0xBF, 0xC6, \
                  0x3B, 0x6D, 0x2C, 0x1D, 0xE1, 0x0A, 0xA1, 0x0A }
// 5BBB0036CF38BFC63B6D2C1DE10AA10A

#define DELAY K_SECONDS(180)

char data[] = {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};

static void downlink_info(uint8_t port, uint8_t flags, int16_t rssi, int8_t snr, uint8_t len,
			  const uint8_t *data)
{
	LOG_INF("Received from port %d, flags %d, RSSI %ddB, SNR %ddBm", port, flags, rssi, snr);
	if (data) {
		LOG_HEXDUMP_INF(data, len, "Payload: ");
	}
}

static void datarate_changed(enum lorawan_datarate dr)
{
	uint8_t unused, max_size;

	lorawan_get_payload_sizes(&unused, &max_size);
	LOG_INF("New Datarate: DR %d, Max Payload %d", dr, max_size);
}

int descriptor_cb(uint32_t descriptor)
{
	/*
	 * In an actual application the firmware may be able to handle
	 * the descriptor field
	 */

	LOG_INF("Received descriptor %u", descriptor);

	return 0;
}

static void fuota_finished(void)
{
	LOG_INF("FUOTA finished. Reset device to apply firmware upgrade.");
	k_sleep(K_SECONDS(5));
	sys_reboot(SYS_REBOOT_COLD);

	/*
	 * In an actual application the firmware should be rebooted here if
	 * no important tasks are pending
	 */
}
// #include <stdio.h>
// #include <zephyr/dfu/mcuboot.h>

int main(void)
{
	// printf("return : %d, swap_type: %d\n", boot_write_img_confirmed(), mcuboot_swap_type());
	// printf("Is image confirmed: %d\n", boot_is_img_confirmed());
	// printf("coucou\n");
	const struct device *lora_dev;
	struct lorawan_join_config join_cfg;
	uint8_t dev_eui[] = LORAWAN_DEV_EUI;
	uint8_t join_eui[] = LORAWAN_JOIN_EUI;
	uint8_t app_key[] = LORAWAN_APP_KEY;
	int ret;

	struct lorawan_downlink_cb downlink_cb = {
		.port = LW_RECV_PORT_ANY,
		.cb = downlink_info
	};

	lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));
	if (!device_is_ready(lora_dev)) {
		LOG_ERR("%s: device not ready.", lora_dev->name);
		return -ENODEV;
	}

	ret = lorawan_start();
	if (ret < 0) {
		LOG_ERR("lorawan_start failed: %d", ret);
		return ret;
	}

	ret = settings_subsys_init();
	if (ret) {
		LOG_ERR("settings subsys initialization: fail (err %d)", ret);
		return ret;
	}

	lorawan_register_downlink_callback(&downlink_cb);
	lorawan_register_dr_changed_callback(datarate_changed);

	join_cfg.mode = LORAWAN_ACT_OTAA;
	join_cfg.dev_eui = dev_eui;
	join_cfg.otaa.join_eui = join_eui;
	join_cfg.otaa.app_key = app_key;
	join_cfg.otaa.nwk_key = app_key;
	join_cfg.otaa.dev_nonce = 0;

	LOG_INF("Joining network over OTAA");

	while (true)
	{
		// get devnonce from storage partition, and increment it
		ret = settings_load_one("lorawan/devnonce", &join_cfg.otaa.dev_nonce,
			sizeof(join_cfg.otaa.dev_nonce));
		LOG_INF("devnonce: settings load one: %d", ret);
		join_cfg.otaa.dev_nonce++;
		ret = settings_save_one("lorawan/devnonce", &join_cfg.otaa.dev_nonce,
			sizeof(join_cfg.otaa.dev_nonce));
		LOG_INF("devnonce: settings save one: %d, devnonce: %d", ret, join_cfg.otaa.dev_nonce);

		ret = lorawan_join(&join_cfg);
		if (!(ret < 0))
			break ;
		LOG_ERR("lorawan_join_network failed: %d", ret);
		k_sleep(K_SECONDS(10));
	}

	lorawan_enable_adr(true);

	/*
	 * Clock synchronization is required to schedule the multicast session
	 * in class C mode. It can also be used independent of FUOTA.
	 */
	lorawan_clock_sync_run();

	/*
	 * The multicast session setup service is automatically started in the
	 * background. It is also responsible for switching to class C at a
	 * specified time.
	 */

	/*
	 * The fragmented data transport transfers the actual firmware image.
	 * It could also be used in a class A session, but would take very long
	 * in that case.
	 */
	lorawan_frag_transport_run(fuota_finished);

	lorawan_frag_transport_register_descriptor_callback(descriptor_cb);

	/*
	 * Regular uplinks are required to open downlink slots in class A for
	 * FUOTA setup by the server.
	 */
	while (1) {
		ret = lorawan_send(2, data, sizeof(data), LORAWAN_MSG_UNCONFIRMED);
		if (ret == 0) {
			LOG_INF("Hello World sent!");
		} else {
			LOG_ERR("lorawan_send failed: %d", ret);
		}

		k_sleep(DELAY);
	}

	return 0;
}
