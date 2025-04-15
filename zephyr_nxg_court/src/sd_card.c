#include "sd_card.h"

int lsdir(const char *path)
{
	struct fs_dir_t dirp;
	static struct fs_dirent entry;

	fs_dir_t_init(&dirp);

	if (fs_opendir(&dirp, path)) {
		printf("Error opening dir %s\n", path);
		return 1;
	}

	printf("\nListing dir %s ...\n", path);
	for (;;) {
		if (fs_readdir(&dirp, &entry) || entry.name[0] == 0)
			break;

		if (entry.type == FS_DIR_ENTRY_DIR) {
			printf("[DIR ] %s\n", entry.name);
		} else {
			printf("[FILE] %s (size = %zu)\n",
				entry.name, entry.size);
		}
	}
	fs_closedir(&dirp);
	return 0;
}

int test(FATFS *fat_fs, struct fs_mount_t *mp)
{
    const struct gpio_dt_spec vcc_sd = GPIO_DT_SPEC_GET(DT_NODELABEL(vcc_sd), gpios);
    const char *disk_pdrv = "SD";
    uint64_t memory_size_mb;
    uint32_t block_count;
    uint32_t block_size;

    // Init Vcc sd card to then configure spi device(sd card)
    if (gpio_pin_configure_dt(&vcc_sd, GPIO_OUTPUT_ACTIVE) < 0) 
        return 1;
    k_msleep(50);
    if (disk_access_ioctl(disk_pdrv, DISK_IOCTL_CTRL_INIT, NULL) != 0) {
        printf("Error init device\n");
        return 1;
    }
    if (disk_access_ioctl(disk_pdrv,DISK_IOCTL_GET_SECTOR_COUNT, &block_count) != 0) {
        printf("Error get sector count\n");
        return 1;
    }
    printf("Block count %u\n", block_count);
    if (disk_access_ioctl(disk_pdrv, DISK_IOCTL_GET_SECTOR_SIZE, &block_size)) {
        printf("Unable to get sector size\n");
        return 1;
    }
    printf("Sector size %u bytes\n", block_size);

    memory_size_mb = (uint64_t)block_count * block_size;
	printf("Memory Size(MB) %u\n", (uint32_t)(memory_size_mb >> 20));

    if (disk_access_ioctl(disk_pdrv, DISK_IOCTL_CTRL_DEINIT, NULL) != 0) {
        printf("Storage deinit ERROR!\n");
        return 1;
    }
    if (fs_mount(mp) != 0) {
        printf("Error mounting disk\n");
        return 1;
    }
    lsdir("/SD:");
    fs_unmount(mp);
    return 0;
}