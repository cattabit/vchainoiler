/*
 * ActualValues.h
 *
 *  Created on: 3 июл. 2022 г.
 *      Author: vector
 */

#ifndef LIBRARIES_ACTUALVALUESVOILER_H_
#define LIBRARIES_ACTUALVALUESVOILER_H_

#pragma once

class ActualValuesVOiler {
public:
	// –асчетные, считываемые и промежуточные параметры, которые будут отображатьс€ в интерфейсе
	unsigned long OdoLeft = 0;    		//ќстаток пробега до смазки
	int TimeLeft = 0;             		//ќстаток времени до смазки
	int PUMP_count = 0; 				//оставшеес€ количество импульсов за текущий запущеный цикл прокачки
	bool PumpStat = false;      		//“екущее состо€ние насоса
	bool PowerIndStat = false;   		//“екущее состо€ние светового индикатора
//	boolean ImpFlag = false; 			//¬спомогательна€ переменна€ дл€ хранени€ наличи€/отсутстви€ импульса в текущий момент
//	float Dist = 0;           			//ѕройденое рассто€ние за интервал замера скорости

	// GPS data
	int gps_connect = -1;			// ‘лаг наличи€ св€зи с GPS-модулем
	float gps_speed = -1.0;				// Speed, км/ч
	int gps_Satelites = -1;			//  оличество спутников, используемых дл€ позиционировани€
	int gps_fixmode = -2;				// –ежим фиксации =1 и др - Ќет фиксации, 2=2D, 3=3D
	float gps_hdop = -1.0;				// текуща€ точность позиционировани€
	double gps_lastpointdist = -1.0;		// ƒистанци€ до прошлой точки позиционировани€, м
	double gps_DistFromLastOiling = -1.0;	// Ќакопитель счетчика пройденного рассто€ни€ с последней смазки, м
	String gps_DiagMsg = "";			// ƒиагностическое сообщение GPS

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

	int alivebyte = 0;	// индикатор активности контроллера дл€ отладки на веб.

	ActualValuesVOiler() {

	}
	;

	//----------------------------------------------------------------------
	//             ‘ункци€ упаковывки всех текущих значений в JSON формат
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
	//             ‘ункци€ упаковывки всех текущих значений в JSON формат
	//----------------------------------------------------------------------
	String getSensorReadingsJSON_String() {
		String jsonString;
		serializeJson(getSensorReadingsJSON(), jsonString);
		return jsonString;
	}
};

ActualValuesVOiler ActlVal;

#endif /* LIBRARIES_ACTUALVALUESVOILER_H_ */
