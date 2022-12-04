/*
 * ActualValues.h
 *
 *  Created on: 3 ���. 2022 �.
 *      Author: vector
 */

#ifndef LIBRARIES_ACTUALVALUESVOILER_H_
#define LIBRARIES_ACTUALVALUESVOILER_H_

#pragma once

class ActualValuesVOiler {
public:
	// ���������, ����������� � ������������� ���������, ������� ����� ������������ � ����������
	unsigned long OdoLeft = 0;    		//������� ������� �� ������
	int TimeLeft = 0;             		//������� ������� �� ������
	int PUMP_count = 0; 				//���������� ���������� ��������� �� ������� ��������� ���� ��������
	bool PumpStat = false;      		//������� ��������� ������
	bool PowerIndStat = false;   		//������� ��������� ��������� ����������
//	boolean ImpFlag = false; 			//��������������� ���������� ��� �������� �������/���������� �������� � ������� ������
//	float Dist = 0;           			//��������� ���������� �� �������� ������ ��������

	// GPS data
	int gps_connect = -1;			// ���� ������� ����� � GPS-�������
	float gps_speed = -1.0;				// Speed, ��/�
	int gps_Satelites = -1;			// ���������� ���������, ������������ ��� ����������������
	int gps_fixmode = -2;				// ����� �������� =1 � �� - ��� ��������, 2=2D, 3=3D
	float gps_hdop = -1.0;				// ������� �������� ����������������
	double gps_lastpointdist = -1.0;		// ��������� �� ������� ����� ����������������, �
	double gps_DistFromLastOiling = -1.0;	// ���������� �������� ����������� ���������� � ��������� ������, �
	String gps_DiagMsg = "";			// ��������������� ��������� GPS

	void setGps_speed(float value, bool valid, uint32_t age) {
		if (valid) {
			if (age <= Stgs.gpsAgeMax) {
				gps_speed = value;
			}
			else {
				gps_speed = -1.0;
			}
		}
		else {
			gps_speed = -2.0;
		}
	}

	int alivebyte = 0;	// ��������� ���������� ����������� ��� ������� �� ���.

	ActualValuesVOiler() {

	}
	;

	//----------------------------------------------------------------------
	//             ������� ���������� ���� ������� �������� � JSON ������
	//----------------------------------------------------------------------
	StaticJsonDocument<1024> getSensorReadingsJSON() {
		StaticJsonDocument < 1024 > jReadings;

		jReadings["alivebyte"] = alivebyte;

		jReadings["AlvaysOne"] = String(1);
		jReadings["OdoLeft"] = String(OdoLeft);
		jReadings["PumpStat"] = String(PumpStat);
		jReadings["PUMP_count"] = String(PUMP_count);
		jReadings["TimeLeft"] = String(TimeLeft);
		jReadings["Speed"] = String(gps_speed);
		jReadings["PowerIndStat"] = String(PowerIndStat);
		jReadings["PumpStat"] = String(PumpStat);

		jReadings["gps_connect"] = String(gps_connect);
		jReadings["gps_speed"] = String(gps_speed);
		jReadings["gps_Satelites"] = String(gps_Satelites);
		jReadings["gps_fixmode"] = String(gps_fixmode);
		jReadings["gps_hdop"] = String(gps_hdop);
		jReadings["gps_lastpointdist"] = String(gps_lastpointdist);
		jReadings["gps_DistFromLastOiling"] = String(gps_DistFromLastOiling);
		jReadings["gps_DiagMsg"] = gps_DiagMsg;

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
