#include <stdio.h>
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

void modem::sendData(uint8_t* data, uint16_t len) {
	HAL_UART_Transmit(&huart, data, len, HAL_MAX_DELAY);
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
	printf("AT+CGREG?\r");
	sendString("AT+CGREG?\r");
	auto response0 = getResponseString();
	auto response1 = getResponseString();
	printf("%s", response0);
	printf("%s", response1);
	if (strcmp(response0, "+CGREG: 0,1") == 0)
		return true;
	return false;
}

bool modem::connectToAPN() {
	flush();
	printf("AT+QICSGP=1,1,\"internet.yota\",\"\",\"\",1\r");
	sendString("AT+QICSGP=1,1,\"internet.yota\",\"\",\"\",1\r");
	auto response0 = getResponseString();
	printf("%s", response0);
	if (strcmp(response0, "OK") == 0)
		return true;
	return false;
}

bool modem::qiact() {
	flush();
	printf("AT+QIACT=1\r");
	sendString("AT+QIACT=1\r");
	auto response0 = getResponseString();
	printf("%s", response0);
	if (strcmp(response0, "OK") == 0)
		return true;
	return false;
}

bool modem::isQIACT() {
	flush();
	printf("AT+QIACT?\r");
	sendString("AT+QIACT?\r");
	auto response0 = getResponseString();
	auto response1 = getResponseString();
	printf("%s", response0);
	printf("%s", response1);
	if (strncmp(response0, "+QIACT", 6) ==0)
		return true;
	return false;
}

bool modem::ATV1() {
	flush();
	printf("ATV1\r");
	sendString("ATV1\r");
	auto response0 = getResponseString();
	printf("%s", response0);
	if (strcmp(response0, "OK") == 0)
		return true;
	return false;
}

bool modem::QIOPEN() {
	const char* cmd = "AT+QIOPEN=1,0,\"TCP\",\"194.67.92.238\",65432,0,1\r";
	flush();
	printf("%s", cmd);
	sendString(cmd);
	auto response0 = getResponseString();
	auto response1 = getResponseString();
	printf("%s", response0);
	printf("%s", response1);
	if (strncmp(response1, "+QIOPEN:", 8) ==0)
		return true;
	return false;
}

bool modem::Send(uint8_t* data, uint16_t length) {
	const char* cmd = "AT+QISEND=0,";
	char buf[100] = "\0";
	char strlength[20];
	sprintf(strlength, "%d", length);
	strcat (buf, cmd);
	strcat (buf, strlength);
	strcat (buf, "\r");

	flush();
	printf("%s", buf);
	sendString(buf);
	sendData(data, length);

	auto response0 = getResponseString();
	printf("%s", response0);

	if (strcmp(response0, "SEND OK") ==0)
		return true;
	return false;
}