#include <string.h>
#include "modem.hpp"



modem::modem (UART_HandleTypeDef& handle) : huart(handle){
}


uint8_t modem::getChar() {
	uint8_t b;
	HAL_UART_Receive(&huart, &b, 1, HAL_MAX_DELAY);
	return b;
}

void modem::sendString(char* cmd) {
	HAL_UART_Transmit(&huart, (uint8_t*)cmd, strlen(cmd), HAL_MAX_DELAY);
}

void modem::flush() {
	huart.Instance->DR;
	huart.Instance->DR;
	huart.Instance->DR;
}

char* getResponseString() {
}

bool modem::isRegistered() {
	flush();
	sendString("AT+CGREG?\r");

}