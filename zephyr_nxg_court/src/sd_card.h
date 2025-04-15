#ifndef SD_CARD_H
# define SD_CARD_H

#include <zephyr/drivers/spi.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/fs/fs.h>
#include <ff.h>

int lsdir(const char *path);
int test(FATFS *fat_fs, struct fs_mount_t *mp);

#endif