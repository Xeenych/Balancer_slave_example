#pragma once
#include "cantypes.hpp"

void FillFieldByType(uint8_t board_idx, const board_t* board_data, field_t field_type, CAN_TxMsg& output) {
  	output.header.TransmitGlobalTime = DISABLE;
  	output.header.IDE = CAN_ID_EXT;
  	output.header.RTR = CAN_RTR_DATA;
  	output.header.StdId = 0;
  	output.header.DLC = 8;
  
  	uint32_t extid = board_idx;
  	extid |= ((uint32_t)(field_type) << 8);
  	output.header.ExtId = extid; 
	
	uint32_t* ptr0 = (uint32_t*)&output.data[0];
	uint32_t* ptr1 = (uint32_t*)&output.data[4];
	switch (field_type) {
	case field_t::v_cell_0_1:
		*ptr0 = *((uint32_t*)&board_data->v_cell[0]);
		*ptr1 = *((uint32_t*)&board_data->v_cell[1]);
		break;
	case field_t::v_cell_2_3:
		*ptr0 = *((uint32_t*)&board_data->v_cell[2]);
		*ptr1 = *((uint32_t*)&board_data->v_cell[3]);
		break;
	case field_t::v_cell_4_5:
		*ptr0 = *((uint32_t*)&board_data->v_cell[4]);
		*ptr1 = *((uint32_t*)&board_data->v_cell[5]);
		break;		
	case field_t::v_cell_6_7:
		*ptr0 = *((uint32_t*)&board_data->v_cell[6]);
		*ptr1 = *((uint32_t*)&board_data->v_cell[7]);
		break;		
	case field_t::t_therm_0_1:
		*ptr0 = *((uint32_t*)&board_data->t_therm[0]);
		*ptr1 = *((uint32_t*)&board_data->t_therm[1]);
		break;		
	case field_t::t_therm_2_3:
		*ptr0 = *((uint32_t*)&board_data->t_therm[2]);
		*ptr1 = *((uint32_t*)&board_data->t_therm[3]);
		break;				
	case field_t::t_a_switching:
		*ptr0 = *((uint32_t*)&board_data->t_a);
		*ptr1 = *((uint32_t*)&board_data->switching_active);
		break;				
	}
}

void MakeCanBuffer (uint8_t board_idx, const board_t* board_data, CAN_TxMsg (&output)[8]) {
	FillFieldByType(board_idx, board_data, field_t::v_cell_0_1, output[0]);
	FillFieldByType(board_idx, board_data, field_t::v_cell_2_3, output[1]);
	FillFieldByType(board_idx, board_data, field_t::v_cell_4_5, output[2]);
	FillFieldByType(board_idx, board_data, field_t::v_cell_6_7, output[3]);
	FillFieldByType(board_idx, board_data, field_t::v_cell_8_9, output[4]);
	FillFieldByType(board_idx, board_data, field_t::t_therm_0_1, output[5]);
	FillFieldByType(board_idx, board_data, field_t::t_therm_2_3, output[6]);
	FillFieldByType(board_idx, board_data, field_t::t_a_switching, output[7]);
};