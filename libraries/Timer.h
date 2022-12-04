#pragma once
#include <Arduino.h>

class Timer {
public:
	Timer(uint16_t nprd = 0) {
		setPeriod(nprd);
	}
	void setPeriod(uint16_t nprd) {
		periodValue = nprd;
	}
	void reset() {
		timerValue = millis();
	}
	uint16_t getPeriod() {
		return periodValue;
	}
	uint16_t getTimeLeft() {
		return millis() - timerValue;
	}
	bool ready() {
		if (periodValue > 0 && millis() - timerValue >= periodValue) {
			timerValue = millis();
			return true;
		}
		return false;
	}
private:
	uint32_t timerValue = 0; // значение таймера отсчета
	uint16_t periodValue = 0; // Значение в мс, после которого отрабатывает ready()
};
