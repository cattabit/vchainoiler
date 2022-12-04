/*
 * GPS.h
 *
 *  Created on: 5 июл. 2022 г.
 *      Author: vector
 */

#ifndef GPS_TINYGPS_H_
#define GPS_TINYGPS_H_

//#define DEBUG 1               // Флаг отладочных сообщений
//#define DEBUG_MNEA 1               // Флаг отладочных сообщений MNEA

#pragma once
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "ActualValuesVOiler.h"
#include "SettingsVOoiler.h"

float latPrev = 56.7558, lngPrev = 37.1503; // create variable for latitude and longitude object
TinyGPSPlus gps;                  // create gps object
TinyGPSCustom fixmode(gps, "GNGGA", 6); // Значение режима фиксации
	//0 — Определение местоположения не возможно или не верно;
	//1 — GPS режим обычной точности, возможно определение местоположения;
	//2 — Дифференциальный GPS режим, точность обычная, возможно определение местоположения;
	//3— GPS режим прецизионной точности, возможно определение местоположения.

SoftwareSerial gpsSerial(12, 13);   //rx,tx pind 3 and 4

Timer timerUpdateGPSValuesCheck(1000);
Timer timerCheckGPSConnection(1000);

static void smartdelay(unsigned long ms);
static bool getNewGPSData();





//----------------------------------------------------------------------
//             Функция инициализации обмена с приемником GPS
//----------------------------------------------------------------------
void initGPS() {
	Logger_printad("TinyGPS", "TinyGPSPlus library v. ");
	Logger_println(TinyGPSPlus::libraryVersion());

	gpsSerial.begin(9600);

	ActlVal.gps_DistFromLastOiling = 0.0;
	delay(500);

	timerUpdateGPSValuesCheck.setPeriod(Stgs.gpsUpdatePeriod); // Установка таймера контроля переодичности обновления данных GPS
	timerCheckGPSConnection.setPeriod(Stgs.gpsUpdatePeriod);
}

//----------------------------------------------------------------------
//             Функция получения, обработки, обновления данных GPS
//----------------------------------------------------------------------
void GPSMainLoop() {

	// Ожидане получения свежих данных в течении заданного времени
	//	smartdelay(Stgs.gpsDataWait);
	if (getNewGPSData()) {
		timerUpdateGPSValuesCheck.reset(); // Сбрасываем таймер контроля переодичности обновления данных.
		timerCheckGPSConnection.reset();
		ActlVal.gps_connect = 1;
		ActlVal.gps_DiagMsg = "";

		// Сохраняем значение качества фиксации местоположения
		ActlVal.gps_fixmode = atoi(fixmode.value());
		// Сохраняем значение текущей скорости
		ActlVal.setGps_speed((float) gps.speed.kmph(), gps.speed.isValid(), gps.speed.age());
		ActlVal.gps_Satelites = gps.satellites.value();
		ActlVal.gps_hdop = gps.hdop.value() / 100.0;

		if (gps.location.isUpdated()) { // Обновилось значение местоположения
			if (gps.location.isValid() // если новые данные валидны, и признак fixmode имеет значение (0 = отсутствует фикация положения)
					&& gps.speed.isValid()	// Данные скорости валидны
					&& (atoi(fixmode.value()) > 0) // факсция положения есть
					) {
				ActlVal.gps_lastpointdist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), latPrev, lngPrev);// Расчитываем дистанцию от прошлой валидной точки
				// и сохраняем значение текущей точки
				latPrev = gps.location.lat();
				lngPrev = gps.location.lng();

				if (gps.speed.kmph() > Stgs.gpsMinCalcSpeed && ActlVal.gps_lastpointdist < Stgs.gpsMaxCalcDistance) { // Если скорость больше минимальной для расчета (из настроек)
					ActlVal.gps_DistFromLastOiling += ActlVal.gps_lastpointdist;
				}

#ifdef DEBUG
			Serial.println("");
			Serial.print(F("	LastValidPoint Distance= "));
			Serial.print(ActlVal.gps_lastpointdist, 6);
			Serial.print(F(" m	"));
			Serial.print(F("	SPEED= "));
			Serial.print(gps.speed.kmph());
			Serial.print(F(" km/h"));
			Serial.print(gps.speed.isValid() ? " (OK) " : " (Err) ");
			Serial.print(F("	FixMode= "));
			Serial.print(gps.fixmode.value());
			Serial.print(gps.fixmode.isValid() ? " (OK) " : " (Err) ");
			Serial.print(F("	ODO= "));
			Serial.print(ActlVal.gps_DistFromLastOiling);
			Serial.print(F(" m	"));
#endif
			}
			else {
#ifdef DEBUG
			Serial.println(F("LOCATION 	InValid"));
#endif
			}
		} // location.isUpdated()

		if (timerUpdateGPSValuesCheck.ready()) { // Таймер контроля обновления данных GPS вышел
#ifdef DEBUG
		Serial.println();
#endif
			String msg = F("DIAGS Chars=") + String(gps.charsProcessed()) + F(" Sentences-with-Fix=")
					+ String(gps.sentencesWithFix()) + F(" FCS=") + String(gps.failedChecksum()) + F(" PCS=")
					+ String(gps.passedChecksum());
			if (gps.charsProcessed() < 10)
				msg = F("WARNING: No GPS data.  Check wiring.");
			ActlVal.gps_DiagMsg = msg;

#ifdef DEBUG
		Serial.print(F("DIAGS      Chars="));
		Serial.print(gps.charsProcessed());
		Serial.print(F(" Sentences-with-Fix="));
		Serial.print(gps.sentencesWithFix());
		Serial.print(F(" Failed-checksum="));
		Serial.print(gps.failedChecksum());
		Serial.print(F(" Passed-checksum="));
		Serial.println(gps.passedChecksum());

		if (gps.charsProcessed() < 10)
		Serial.println(F("WARNING: No GPS data.  Check wiring."));

		Serial.println();
#endif
		}
	}
	else {
		if (timerCheckGPSConnection.ready()){
			ActlVal.gps_connect = 0;
			ActlVal.gps_hdop = -1.0;
			ActlVal.gps_Satelites = -1;
			ActlVal.gps_fixmode = -2;
			ActlVal.gps_speed = -1.0;
			ActlVal.gps_lastpointdist = -1.0;
			ActlVal.gps_DistFromLastOiling = -1.0;
		}
	}

	ActlVal.alivebyte = gps.charsProcessed() / 1000;
}

//----------------------------------------------------------------------
//             Функция ожидания нового пакета данных от GPS-приемника
//----------------------------------------------------------------------
static void smartdelay(unsigned long ms) {
	unsigned long start = millis();
#ifdef DEBUG_MNEA
	do {
		while (gpsSerial.available()) {
			char c = gpsSerial.read();
			Serial.write(c);
			gps.encode(c);
		}
	} while (millis() - start < ms);
	Serial.println();
#else
	do {
		while (gpsSerial.available()) {
			gps.encode(gpsSerial.read());
		}
	} while (millis() - start < ms);
#endif
}

//----------------------------------------------------------------------
//             Функция получения нового пакета данных от GPS-приемника
//----------------------------------------------------------------------
static bool getNewGPSData() {
	unsigned long start = millis();
	bool res = false;
	do {
		while (gpsSerial.available()) {
			if (gps.encode(gpsSerial.read())) {
				res = true;
			}
		}
	} while ((millis() - start) < Stgs.gpsDataWait);
	return res;
}

#endif /* GPS_TINYGPS_H_ */
