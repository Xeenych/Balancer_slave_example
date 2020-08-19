#pragma once
#include "stm32f1xx_hal.h"



class modem {
public:
	modem (UART_HandleTypeDef& handle);
	bool isRegistered();
	bool connectToAPN();
	bool qiact();
	bool isQIACT();
	bool ATV1();
	bool QIOPEN();
	bool Send(uint8_t* data, uint16_t length);

private:
	uint8_t getChar();
	void flush();
	void sendString(const char* cmd);
	void sendData(uint8_t* data, uint16_t len);

	char* getResponseString();
	UART_HandleTypeDef& huart;
};