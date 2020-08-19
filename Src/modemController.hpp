#pragma once
#include "modem.hpp"


class modemController {
	enum class mcState {
		reset,
		registered,
		activated,
		open,
		cansend
	};
public:
	modemController(modem& m);
	void Run();
	bool canSend();
	void Send(uint8_t* data, uint16_t len);
private:
	modem& m;
	mcState state = mcState::reset;
};