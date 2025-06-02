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

uint8_t get_disk_info()
{
    const char *disk_pdrv = "SD";
    uint64_t memory_size_mb;
    uint32_t block_count;
    uint32_t block_size;

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
    return 0;
}

bool is_card_detected()
{
    const struct gpio_dt_spec cd = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), cd_gpios);

    gpio_pin_configure_dt(&cd, GPIO_INPUT);
	k_msleep(1);
    bool is_detected = gpio_pin_get_raw(cd.port, cd.pin);
#ifdef DEBUG
    printf("card detected %d\n", !is_detected);
#endif

    return !is_detected;
}

static void close_unmount_file(struct fs_file_t *file, struct fs_mount_t *mp)
{
    fs_close(file);
    fs_unmount(mp);
} 

int write_in_sd_card(FATFS *fat_fs, struct fs_mount_t *mp)
{
    char currentfilename[32] = {'\0'};
    char csvline[168] = {'\0'};
    struct fs_file_t file;
    struct fs_dirent empty_file;

    if(!is_card_detected())
    {
    #ifdef DEBUG
        printf("Card not detected\n");
    #endif
        return 1;
    }

    if (fs_mount(mp) != 0) {
    #ifdef DEBUG
        printf("Error mounting disk\n");
    #endif
        return 1;
    }
    
    fs_file_t_init(&file);
    snprintf(currentfilename, 29, "/SD:/b.csv");
    if (fs_open(&file, currentfilename, FS_O_CREATE | FS_O_APPEND | FS_O_WRITE) != 0)
    {
    #ifdef DEBUG
        printf("Error failed to open filename: %s\n", currentfilename);
    #endif
        return close_unmount_file(&file, mp), 1;
    }
    if (fs_stat(currentfilename, &empty_file) != 0)
    {
    #ifdef DEBUG
        printf("Error stat file size\n");
    #endif
        return close_unmount_file(&file, mp), 1;
    }
    if (empty_file.size == 0)
    {
        fs_write(&file, "T(degC);H(%%);Luminosity(lux)\r\n", strlen("T(degC);H(%%);Luminosity(lux)\r\n"));
    }
    uint16_t ml_size = measures_logger_get_size();
    for (int i = 0; i < ml_size; i++)
    {
        memset(csvline, 0, sizeof(csvline));
        bool ml_read_err = measures_logger_read_CSV(csvline);
        if (ml_read_err == MEASURES_LOGGER_RES_FAILURE)
        {
            MEASURES_LOGGER_ERROR ml_err = measures_logger_get_error();
            if (ml_err == MEASURES_LOGGER_ERROR_OK)
            {}
            else if (ml_err == MEASURES_LOGGER_NOTHING_TO_READ)
                return close_unmount_file(&file, mp), 1;
            else
                return close_unmount_file(&file, mp), 1;
        }
        strcat(csvline, "\r\n");
    
    #ifdef DEBUG
        printf("res : %s\n", csvline);
    #endif
        fs_write(&file, csvline, strlen(csvline));
    }
    close_unmount_file(&file, mp);
    return 0;
}