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
#include "TinyGPS++.h"
#include "ActualValuesVOiler.h"
#include "SettingsVOoiler.h"

float latPrev = 28.5458, lngPrev = 77.1703; // create variable for latitude and longitude object
TinyGPSPlus gps;                  // create gps object
SoftwareSerial gpsSerial(12, 13);   //rx,tx pind 3 and 4

Timer timerUpdateGPSValues(1000);

//Custom fields config
TinyGPSCustom fixmode(gps, "GPGSA", 2); // $GPGSA sentence, Mode: 1=Fix not available, 2=2D, 3=3D
TinyGPSCustom SVsinView(gps, "GPGSV", 3); // Total number of SVs in view
TinyGPSCustom SpeedOverGround(gps, "GPVTG", 7); // Speed over ground in kilometers/hour

static void smartdelay(unsigned long ms);

unsigned long odo = 0UL; 			// Odometer, when speed over 20km/h
double distanceToPrev = 0.0; // distance to last valid point / ��������� �� ������� �������� �����, ������������ ��� ������� ������� ����� ��������

//----------------------------------------------------------------------
//             ������� ������������� ������ � ���������� GPS
//----------------------------------------------------------------------
void initGPS()
{
#ifdef DEBUG
	Serial.print("TinyGPSPlus library v. ");
	Serial.println(TinyGPSPlus::libraryVersion());
#endif

	gpsSerial.begin(4800);
	odo = 0UL;
}

//----------------------------------------------------------------------
//             ������� ���������, ���������, ���������� ������ GPS
//----------------------------------------------------------------------
void GPSMainLoop()
{

	// ��������� �������� �������� �������� ��������������
	ActlVal.setFixMode(fixmode.value(), fixmode.isValid(), fixmode.age());
	// ��������� �������� ������� ��������
	ActlVal.setGps_speed((float) gps.speed.kmph(), gps.speed.isValid(),
	        gps.speed.age());

	if (SpeedOverGround.isUpdated()) {
		ActlVal.gps_speedog = atof(SpeedOverGround.value());
	}

	if (gps.satellites.isUpdated()) {
//			Serial.print(F("SATELLITES Fix Age="));
//			Serial.print(gps.satellites.age());
//			Serial.print(F("ms Value="));
//			Serial.println(gps.satellites.value());

		ActlVal.gps_Satelites = gps.satellites.value();
	}

	if (gps.hdop.isUpdated()) {
//			Serial.print(F("HDOP       Fix Age="));
//			Serial.print(gps.hdop.age());
//			Serial.print(F("ms Value="));
//			Serial.println(gps.hdop.value());

		ActlVal.gps_hdop = gps.hdop.value();
	}

	if (gps.location.isUpdated()) { // ���������� �������� ��������������
		ActlVal.gps_SatelitesVisible = String(SVsinView.value());

		if (gps.location.isValid()) {	// ���� ����� ������ �������
			distanceToPrev = TinyGPSPlus::distanceBetween(gps.location.lat(),
			        gps.location.lng(), latPrev, lngPrev); // ����������� ��������� �� ������� �������� �����
			// � ��������� �������� ������� �����
			latPrev = gps.location.lat();
			lngPrev = gps.location.lng();
			ActlVal.gps_lastpointdist = distanceToPrev; // ��������� ���������� �������� ��� ����������� � ����������

			if (ActlVal.gps_speed > Stgs.gpsMinCalcSpeed // ���� �������� �������� ������� � �������� ������ ����������� ��� ������� (�� ��������)
			&& (ActlVal.gps_fixmode == 2 || ActlVal.gps_fixmode == 3) // ���� ������� GPS ���������� (2D ��� 3D)
			        ) {
				odo += distanceToPrev;
				ActlVal.gps_DistFromLastOiling += distanceToPrev;
			}

#ifdef DEBUG
			Serial.println("");
			Serial.print(F("	LastValidPoint Distance= "));
			Serial.print(distanceToPrev, 6);
			Serial.print(F(" m	"));
			Serial.print(F("	SPEED= "));
			Serial.print(gps.speed.kmph());
			Serial.print(F(" km/h"));
			Serial.print(gps.speed.isValid() ? " (OK) " : " (Err) ");
			Serial.print(F("	FixMode= "));
			Serial.print(fixmode.value());
			Serial.print(fixmode.isValid() ? " (OK) " : " (Err) ");
			Serial.print(F("	ODO= "));
			Serial.print(odo);
			Serial.print(F(" m	"));
#endif
		} else {
#ifdef DEBUG
			Serial.println(F("LOCATION 	InValid"));
#endif
		}
	} // location.isUpdated()

	timerUpdateGPSValues.setPeriod(Stgs.gpsUpdatePeriod);
	// ������ ���������� ������ GPS
	if (timerUpdateGPSValues.ready()) {
#ifdef DEBUG
		Serial.println();
#endif
		String msg = F("DIAGS Chars=") + String(gps.charsProcessed())
		        + F(" Sentences-with-Fix=") + String(gps.sentencesWithFix())
		        + F(" FCS=") + String(gps.failedChecksum()) + F(" PCS=")
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
		smartdelay(Stgs.gpsDataWait);
	}
}

//----------------------------------------------------------------------
//             ������� �������� ������ ������ ������ �� GPS-���������
//----------------------------------------------------------------------
static void smartdelay(unsigned long ms)
{
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

#endif /* GPS_TINYGPS_H_ */
