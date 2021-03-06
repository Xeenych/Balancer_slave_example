#pragma once
#include <stdint.h>
#include "ControllerCan.hpp"

struct board_t {
  float v_cell[10];
  float t_therm[4];
  float t_a;
  bool switching_active;
};

enum class field_t : uint8_t {
	v_cell_0_1 = 0,
	v_cell_2_3,
	v_cell_4_5,
	v_cell_6_7,
	v_cell_8_9,
	t_therm_0_1,
	t_therm_2_3,
	t_a_switching
};

void BSP_parseCANRxBuffer(CAN_RxMsg& msg, board_t* board);
void BSP_fillCANTxBuffer(CAN_TxMsg* frame_buffer, board_t* board, uint8_t board_idx);
