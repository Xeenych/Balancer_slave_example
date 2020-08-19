#pragma once

#include "stm32f1xx_hal.h"

struct CAN_TxMsg {
  	CAN_TxHeaderTypeDef header;
  	uint8_t data[8];
};

struct CAN_RxMsg {
  CAN_RxHeaderTypeDef header;
  uint8_t data[8];
};

class ControllerCan  {
public:
	struct Errors {
		uint8_t lec;
		uint8_t tec;
		uint8_t rec;
	};

  	ControllerCan(CAN_HandleTypeDef* h, uint32_t mode);
  	ControllerCan(ControllerCan& other)=delete;

  	bool SendMessage(CAN_TxMsg& txmsg);
  	bool ReadMessage(CAN_RxMsg& msg);
  	uint32_t MessagesAvailable();
  	ControllerCan::Errors ReadErrors();
protected:

private:
  	void ConfigureBaud();
  	void ConfigureFilters();

  	CAN_HandleTypeDef* handle;
  	unsigned int bytes_sent=0;
  	unsigned int bytes_recd=0;
  	unsigned int SendMessageFaults = 0;
};



