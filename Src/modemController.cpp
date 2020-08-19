#include <stdio.h>
#include "modemController.hpp"


modemController::modemController(modem& modem) : m(modem){
}

void modemController::Send(uint8_t* data, uint16_t len) {
	if (canSend())
		m.Send(data, len);
}

void modemController::Run() {

	switch (state) {
	case mcState::reset:
		printf ("mcState reset\n");
		if (m.isRegistered()) {
			m.connectToAPN();
			m.qiact();
			state = mcState::registered;
		}
		break;
	case mcState::registered:
		printf ("mcState registered\n");
		if (m.isQIACT())
			m.ATV1();
			state = mcState::open;
		break;
	case mcState::open:
		printf ("mcState open\n");
		if (m.QIOPEN())
			state = mcState::cansend;
		break;
	case mcState::cansend:
		printf ("mcState cansend\n");
		break;
	default:
		state = mcState::reset;
		break;
	}
}

bool modemController::canSend() {
	if (state == mcState::cansend)
		return true;
	return false;
}