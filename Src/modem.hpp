#pragma once
#include "stm32f1xx_hal.h"



class modem {
public:
	modem (UART_HandleTypeDef& handle);
	bool isRegistered();
private:
	uint8_t getChar();
	void flush();
	void sendString(const char* cmd);
	char* getResponseString();
	UART_HandleTypeDef& huart;
};