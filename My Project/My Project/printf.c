#include "printf.h"

// Function add to make printf works

uint8_t usart_dbg_put_buffer(uint8_t *buffer, uint16_t size)
{
	if (io_write(&USART_0.io, buffer, size) == 0)
	{
		return 1;  // Error
	}
	return 0;  // OK
}

/*
 * Function to be used by stdout for printf etc
 */
int _write(int fd, const void *buf, size_t count)
{
  while (usart_dbg_put_buffer((uint8_t *)buf, (uint16_t)count) != 0)
  {
  };
  return count;
}

uint8_t usart_dbg_get_buffer(uint8_t *buffer, uint16_t size, uint16_t *nbReadBytes)
{
	*nbReadBytes = io_read(&USART_0.io, buffer, size);
	if (*nbReadBytes == 0)
	{
		return 1;  // Empty
	}
	return 0;  // OK
}

/*
 * Function to be used by stdin for scanf etc
 */
int _read(int fd, const void *buf, size_t count)
{
  size_t bytesRead = 0;
  while (usart_dbg_get_buffer((uint8_t *)buf, count, (uint16_t *)&bytesRead) != 0)
  {
  };
  // Echo back the character
  while (usart_dbg_put_buffer((uint8_t *)buf, (uint16_t)bytesRead) != 0)
  {
  };
  return bytesRead;
}
