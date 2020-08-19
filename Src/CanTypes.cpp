#pragma once
#include <string.h>
#include "cantypes.hpp"

#define PARAMETER_GROUP_NR 8

void BSP_parseCANRxBuffer(CAN_RxMsg& msg, board_t* board) {
	uint8_t idx = msg.header.ExtId & 0xff;
	void* src = msg.data;
	int offset = (msg.header.ExtId >> 8) & 0xff;

	if (offset>=PARAMETER_GROUP_NR)
		offset=0;
	memcpy(&(board[idx]) + offset, src, 8);
}

void BSP_fillCANTxBuffer(CAN_TxMsg* frame_buffer, board_t* board, uint8_t board_idx)
{
    CAN_TxMsg empty_frame =
    {
        .data   = { 0 },
        .header =
        {
            .StdId = 0,
            .ExtId = 0,
            .IDE = CAN_ID_EXT,
            .RTR = CAN_RTR_DATA,
            .DLC = 8
        }
    };
    int offset = 0;

    for (int i = 0; i < PARAMETER_GROUP_NR; i++)
    {
        memcpy((void*)&empty_frame.data, (void*)((int)board + offset), 8);
        empty_frame.header.ExtId = board_idx | (i << 8);
        offset += 8;
        memcpy((void*)&frame_buffer[i], (void*)&empty_frame, sizeof(CAN_TxMsg));
    }
}
