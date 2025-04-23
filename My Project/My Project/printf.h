#ifndef PRINTF_H
# define PRINTF_H

#include <driver_init.h>
#include <stdio.h>

uint8_t usart_dbg_put_buffer(uint8_t *buffer, uint16_t size);
int     _write(int fd, const void *buf, size_t count);
uint8_t usart_dbg_get_buffer(uint8_t *buffer, uint16_t size, uint16_t *nbReadBytes);
int     _read(int fd, const void *buf, size_t count);

#endif