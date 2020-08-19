#include <string.h>
#include "modem.hpp"



modem::modem (UART_HandleTypeDef& handle) : huart(handle){
}


uint8_t modem::getChar() {
	uint8_t b;
	HAL_UART_Receive(&huart, &b, 1, HAL_MAX_DELAY);
	return b;
}

void modem::sendString(const char* cmd) {
	HAL_UART_Transmit(&huart, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);
}

void modem::flush() {
	huart.Instance->DR;
	huart.Instance->DR;
	huart.Instance->DR;
}

char buf[100];

char* modem::getResponseString() {
	auto ch0 = getChar();
	auto ch1 = getChar();

	if (ch0!='\r')
		return nullptr;
	if (ch1!='\n')
		return nullptr;

	uint32_t idx=0;
	while (idx<sizeof(buf)) {
		auto c = getChar();
		if (c=='\r')
			break;
		buf[idx++] = c;
	}
	buf[idx]='\0';

	getChar(); // '\n'
	return (char*)buf;
}

bool modem::isRegistered() {
	flush();
	sendString("AT+CGREG?\r");
	auto response = getResponseString();
	if (strcmp(response, "OK") == 0)
		return true;
	return false;
}