/*
 * Logger.h
 *
 *  Created on: 1 дек. 2022 г.
 *      Author: vector
 */

#ifndef LIBRARIES_LOGGER_H_
#define LIBRARIES_LOGGER_H_

void Logger_print(const char value[]) {
#ifdef DEBUG
//	Serial.print(millis());
	Serial.print(value);
#endif
}

void Logger_println(const String& value) {
#ifdef DEBUG
//	Serial.print(millis());
	Serial.println(value);
#endif
}
void Logger_println(const char value[]) {
#ifdef DEBUG
//	Serial.print(millis());
	Serial.println(value);
#endif
}

void Logger_printad(const char module[], const char value[]) {
#ifdef DEBUG
	Serial.print(String(millis()) + " ");
	Serial.print("[");
	Serial.print(module);
	Serial.print("]: ");
	Serial.print(value);
	//Serial.print("[" + module + "]: " + value);
#endif
}
void Logger_printad(const char module[], const String& value) {
#ifdef DEBUG
	Serial.print(String(millis()) + " ");
	Serial.print("[");
	Serial.print(module);
	Serial.print("]: ");
	Serial.print(value);
	//Serial.print("[" + module + "]: " + value);
#endif
}


void Logger_printadln(const char module[], const char value[]) {
#ifdef DEBUG
	Logger_printad(module, value);
	Serial.println();
#endif
}
void Logger_printadln(const char module[], const String& value) {
#ifdef DEBUG
	Logger_printad(module, value);
	Serial.println();
#endif
}

#endif /* LIBRARIES_LOGGER_H_ */
