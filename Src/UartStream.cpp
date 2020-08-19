#include "stm32F1xx_hal.h"
#include "UartStream.hpp"
#include <LowLevelIOInterface.h>

static int u2Handle = 3;
extern UART_HandleTypeDef huart2;

int u2Putchar(int x) {
	auto result = HAL_UART_Transmit(&huart2, (uint8_t*)&x, 1, HAL_MAX_DELAY);
	if (result==HAL_OK)
		return x;
	return -1;
}

size_t uart__write(int handle, const unsigned char * buffer, size_t size){
	size_t nChars = 0;

  /* This template only writes to "standard out" and "standard err",
   * for all other file handles it returns failure. */
  if (handle != u2Handle)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size != 0; --size)
  {
    if (u2Putchar(*buffer++) < 0)
    {
      return _LLIO_ERROR;
    }

    ++nChars;
  }

  return nChars;
}

FILE* u2open() {
	return (FILE*)&u2Handle;
}