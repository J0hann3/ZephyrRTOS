#ifndef SD_CARD_H
# define SD_CARD_H

#include <zephyr/drivers/spi.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/fs/fs.h>
#include <ff.h>
#include "measures_logger.h"

int lsdir(const char *path);
uint8_t get_disk_info();
int write_in_sd_card(FATFS *fat_fs, struct fs_mount_t *mp);

#endif