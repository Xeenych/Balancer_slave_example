#include <assert.h>
#include "ControllerCan.hpp"

ControllerCan::ControllerCan(CAN_HandleTypeDef* h, uint32_t mode) : handle(h){ 
  	ConfigureBaud();
	handle->Init.Mode = mode;
	
	SET_BIT(handle->Instance->MCR, CAN_MCR_DBF);
	__HAL_DBGMCU_FREEZE_CAN1();
	
	{
  		HAL_StatusTypeDef res = HAL_CAN_Init(handle);
  		assert(res == HAL_OK);
	}
	
	ConfigureFilters();
	
	{
  		HAL_StatusTypeDef res =  HAL_CAN_Start(handle);
		assert(res == HAL_OK);
	}
};

void ControllerCan::ConfigureBaud() {
  	// 500 kbit/s
	//handle.Init.Prescaler = 4;
  	//handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
  	//handle.Init.TimeSeg1 = CAN_BS1_13TQ;
  	//handle.Init.TimeSeg2 = CAN_BS2_2TQ;
	
	// 125 kbit/s
  	handle->Init.Prescaler = 16;
  	handle->Init.SyncJumpWidth = CAN_SJW_1TQ;
  	handle->Init.TimeSeg1 = CAN_BS1_13TQ;
  	handle->Init.TimeSeg2 = CAN_BS2_2TQ;
	
  	handle->Init.TimeTriggeredMode = DISABLE;
  	handle->Init.AutoBusOff = ENABLE;
  	handle->Init.AutoWakeUp = ENABLE;
  	handle->Init.AutoRetransmission = ENABLE;
  	handle->Init.ReceiveFifoLocked = DISABLE;
  	handle->Init.TransmitFifoPriority = DISABLE;
}

void ControllerCan::ConfigureFilters() {
  CAN_FilterTypeDef fconfig; // устанавливаем фильтр на прием всех сообщений
  fconfig.FilterActivation = CAN_FILTER_ENABLE;
  fconfig.FilterBank = 0;
  fconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  fconfig.FilterIdHigh = 0x0000;
  fconfig.FilterIdLow = 0x0000;
  fconfig.FilterMaskIdHigh = 0x0000;
  fconfig.FilterMaskIdLow = 0x0000;
  fconfig.FilterMode = CAN_FILTERMODE_IDMASK;
  fconfig.FilterScale = CAN_FILTERSCALE_32BIT;
  fconfig.SlaveStartFilterBank = 27;
  HAL_StatusTypeDef result = HAL_CAN_ConfigFilter(handle, &fconfig);
  assert(result == HAL_OK);
};

bool ControllerCan::SendMessage(CAN_TxMsg& txmsg) {
  	uint32_t pTxMailbox; // не используем
  	HAL_StatusTypeDef result = HAL_CAN_AddTxMessage(handle, &txmsg.header, txmsg.data, &pTxMailbox);
  	if (result == HAL_OK) {
	    bytes_sent += txmsg.header.DLC;
    	return true;
  	}
  	SendMessageFaults++;
    return false;
};

// возвращает количество еще не прочитанных сообщений
uint32_t ControllerCan::MessagesAvailable() {
  uint32_t fifo0 = HAL_CAN_GetRxFifoFillLevel(handle, CAN_RX_FIFO0);
  uint32_t fifo1 = HAL_CAN_GetRxFifoFillLevel(handle, CAN_RX_FIFO1);
  return fifo0 + fifo1;
};

// возвращает true в случае успеха
bool ControllerCan::ReadMessage(CAN_RxMsg& msgout) {
  	HAL_StatusTypeDef result0 = HAL_CAN_GetRxMessage(handle, CAN_RX_FIFO0, &msgout.header, msgout.data);
  	if (result0 == HAL_OK) {
		bytes_recd+=msgout.header.DLC;
	  	return true;
  	}
  
  	HAL_StatusTypeDef result1 = HAL_CAN_GetRxMessage(handle, CAN_RX_FIFO1, &msgout.header, msgout.data);
  	if (result1 == HAL_OK) {
		bytes_recd+=msgout.header.DLC;
		return true;
  	}

  return false;
};

ControllerCan::Errors ControllerCan::ReadErrors() {
	  uint32_t esrflags = READ_REG(handle->Instance->ESR);

	  uint8_t lec = (esrflags & CAN_ESR_LEC) >> CAN_ESR_LEC_Pos;
	  CLEAR_BIT(handle->Instance->ESR, CAN_ESR_LEC);
	  
	  uint8_t tec = (esrflags & CAN_ESR_TEC) >> CAN_ESR_TEC_Pos;
	  uint8_t rec = (esrflags & CAN_ESR_REC) >> CAN_ESR_REC_Pos;
	  
	  return {lec, tec, rec};
}