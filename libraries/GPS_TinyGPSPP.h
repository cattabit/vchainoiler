/*
 * GPS.h
 *
 *  Created on: 5 ���. 2022 �.
 *      Author: vector
 */

#ifndef GPS_TINYGPS_H_
#define GPS_TINYGPS_H_

//#define DEBUG 1               // ���� ���������� ���������
//#define DEBUG_MNEA 1               // ���� ���������� ��������� MNEA

#pragma once
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "ActualValuesVOiler.h"
#include "SettingsVOoiler.h"

float latPrev = 56.7558, lngPrev = 37.1503; // create variable for latitude and longitude object
TinyGPSPlus gps;                  // create gps object
TinyGPSCustom fixmode(gps, "GNGGA", 6); // �������� ������ ��������
	//0 � ����������� �������������� �� �������� ��� �� �����;
	//1 � GPS ����� ������� ��������, �������� ����������� ��������������;
	//2 � ���������������� GPS �����, �������� �������, �������� ����������� ��������������;
	//3� GPS ����� ������������ ��������, �������� ����������� ��������������.

SoftwareSerial gpsSerial(12, 13);   //rx,tx pind 3 and 4

Timer timerUpdateGPSValuesCheck(1000);
Timer timerCheckGPSConnection(1000);

static void smartdelay(unsigned long ms);
static bool getNewGPSData();





//----------------------------------------------------------------------
//             ������� ������������� ������ � ���������� GPS
//----------------------------------------------------------------------
void initGPS() {
	Logger_printad("TinyGPS", "TinyGPSPlus library v. ");
	Logger_println(TinyGPSPlus::libraryVersion());

	gpsSerial.begin(9600);

	ActlVal.gps_DistFromLastOiling = 0.0;
	delay(500);

	timerUpdateGPSValuesCheck.setPeriod(Stgs.gpsUpdatePeriod); // ��������� ������� �������� ������������� ���������� ������ GPS
	timerCheckGPSConnection.setPeriod(Stgs.gpsUpdatePeriod);
}

//----------------------------------------------------------------------
//             ������� ���������, ���������, ���������� ������ GPS
//----------------------------------------------------------------------
void GPSMainLoop() {

	// ������� ��������� ������ ������ � ������� ��������� �������
	//	smartdelay(Stgs.gpsDataWait);
	if (getNewGPSData()) {
		timerUpdateGPSValuesCheck.reset(); // ���������� ������ �������� ������������� ���������� ������.
		timerCheckGPSConnection.reset();
		ActlVal.gps_connect = 1;
		ActlVal.gps_DiagMsg = "";

		// ��������� �������� �������� �������� ��������������
		ActlVal.gps_fixmode = atoi(fixmode.value());
		// ��������� �������� ������� ��������
		ActlVal.setGps_speed((float) gps.speed.kmph(), gps.speed.isValid(), gps.speed.age());
		ActlVal.gps_Satelites = gps.satellites.value();
		ActlVal.gps_hdop = gps.hdop.value() / 100.0;

		if (gps.location.isUpdated()) { // ���������� �������� ��������������
			if (gps.location.isValid() // ���� ����� ������ �������, � ������� fixmode ����� �������� (0 = ����������� ������� ���������)
					&& gps.speed.isValid()	// ������ �������� �������
					&& (atoi(fixmode.value()) > 0) // ������� ��������� ����
					) {
				ActlVal.gps_lastpointdist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), latPrev, lngPrev);// ����������� ��������� �� ������� �������� �����
				// � ��������� �������� ������� �����
				latPrev = gps.location.lat();
				lngPrev = gps.location.lng();

				if (gps.speed.kmph() > Stgs.gpsMinCalcSpeed && ActlVal.gps_lastpointdist < Stgs.gpsMaxCalcDistance) { // ���� �������� ������ ����������� ��� ������� (�� ��������)
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

		if (timerUpdateGPSValuesCheck.ready()) { // ������ �������� ���������� ������ GPS �����
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
//             ������� �������� ������ ������ ������ �� GPS-���������
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
//             ������� ��������� ������ ������ ������ �� GPS-���������
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
