/*
 * ActualValues.h
 *
 *  Created on: 3 ���. 2022 �.
 *      Author: vector
 */

#ifndef LIBRARIES_ACTUALVALUESVOILER_H_
#define LIBRARIES_ACTUALVALUESVOILER_H_

//#define DEBUG 1               // ���� ���������� ���������

#pragma once

class ActualValuesVOiler {
public:
	// ���������, ����������� � ������������� ���������, ������� ����� ������������ � ����������
	unsigned long OdoLeft = 0;    		//������� ������� �� ������
	int TimeLeft = 0;             		//������� ������� �� ������
	int PUMP_count = 0; //���������� ���������� ��������� �� ������� ��������� ���� ��������
	float Speed = 0;             		//���������� ��������
	float Odo = 0;     //����������� ������, ���������� ���������� ������ (� ��)
	bool PumpStat = false;      		//������� ��������� ������
	bool PowerIndStat = false;   		//������� ��������� ��������� ����������
	boolean ImpFlag = false; //��������������� ���������� ��� �������� �������/���������� �������� � ������� ������
	float Dist = 0;           //��������� ���������� �� �������� ������ ��������

	// GPS data
	String gps_dataisvalid = "1";		// ���� ���������� ��������� ����������
	float gps_speedog = -0.1;			// Speed on ground
	float gps_speed = -0.1;				// Speed, ��/�
	double gps_lastpointdist = 0.0;	// ��������� �� ������� ����� ����������������, �
	uint32_t gps_Satelites = 0;	// ���������� ���������, ������������ ��� ����������������
	String gps_SatelitesVisible = "*";	// ���������� ������� ���������
	int gps_fixmode = -1;	// ����� �������� =1 � �� - ��� ��������, 2=2D, 3=3D
	int32_t gps_hdop = -1;				// ������� �������� ����������������
	double gps_DistFromLastOiling = 0.0;// ���������� �������� ����������� ���������� � ��������� ������, �
	String gps_DiagMsg	= "";			// ��������������� ��������� GPS

	void setGps_speedog(float value) {
		gps_speedog = value;
	}

	void setGps_speed(float value, bool valid, uint32_t age) {
		if (valid) {
			if (age <= Stgs.gpsAgeMax) {
				gps_speed = value;
			} else {
				gps_speed = -1.0;
			}
		} else {
			gps_speed = -2.0;
		}

	}

	// ��������� ���������� � �������� �������� GPS
	void setFixMode(const char *value, bool valid, uint32_t age) {
		if (valid) {
			if (age <= Stgs.gpsAgeMax) {
				if (value != "") {
					gps_fixmode = atoi(value);
				} else
					gps_fixmode = -1;
			} else {
				gps_fixmode = -2;
			}
		} else
			gps_fixmode = -3;
	}

#ifdef DEBUG
	int alivebyte = 0;	// ��������� ���������� ����������� ��� ������� �� ���.
#endif

	ActualValuesVOiler() {

	}
	;

	//----------------------------------------------------------------------
	//             ������� ���������� ���� ������� �������� � JSON ������
	//----------------------------------------------------------------------
	StaticJsonDocument<1024> getSensorReadingsJSON() {
		StaticJsonDocument < 1024 > jReadings;

#ifdef DEBUG
		jReadings["alivebyte"] = alivebyte++;
#endif

		jReadings["AlvaysOne"] = String(1);
		jReadings["OdoLeft"] = String(OdoLeft);
		jReadings["PumpStat"] = String(PumpStat);
		jReadings["PUMP_count"] = String(PUMP_count);
		jReadings["TimeLeft"] = String(TimeLeft);
		jReadings["Speed"] = String(Speed);
		jReadings["Odo"] = String(Odo);
		jReadings["PowerIndStat"] = String(PowerIndStat);
		jReadings["PumpStat"] = String(PumpStat);

		jReadings["gps_speedog"] = String(gps_speedog);
		jReadings["gps_speed"] = String(gps_speed);
		jReadings["gps_lastpointdist"] = String(gps_lastpointdist);
		jReadings["gps_Satelites"] = String(gps_Satelites);
		jReadings["gps_SatelitesVisible"] = gps_SatelitesVisible;
		jReadings["gps_fixmode"] = String(gps_fixmode);
		jReadings["gps_hdop"] = String(gps_hdop);
		jReadings["gps_DistFromLastOiling"] = String(gps_DistFromLastOiling);
		jReadings["gps_DiagMsg"] = gps_DiagMsg;
		jReadings["gps_dataisvalid"] = gps_dataisvalid;

		return jReadings;
	}

	//----------------------------------------------------------------------
	//             ������� ���������� ���� ������� �������� � JSON ������
	//----------------------------------------------------------------------
	String getSensorReadingsJSON_String() {
		String jsonString;
		serializeJson(getSensorReadingsJSON(), jsonString);
		return jsonString;
	}
};

ActualValuesVOiler ActlVal;

#endif /* LIBRARIES_ACTUALVALUESVOILER_H_ */
