/*
 * ActualValues.h
 *
 *  Created on: 3 июл. 2022 г.
 *      Author: vector
 */

#ifndef LIBRARIES_ACTUALVALUESVOILER_H_
#define LIBRARIES_ACTUALVALUESVOILER_H_

//#define DEBUG 1               // Флаг отладочных сообщений

#pragma once

class ActualValuesVOiler {
public:
	// Расчетные, считываемые и промежуточные параметры, которые будут отображаться в интерфейсе
	unsigned long OdoLeft = 0;    		//Остаток пробега до смазки
	int TimeLeft = 0;             		//Остаток времени до смазки
	int PUMP_count = 0; //оставшееся количество импульсов за текущий запущеный цикл прокачки
	float Speed = 0;             		//Замеренная скорость
	float Odo = 0;     //Посчитанный пробег, обнуляется включением насоса (в мм)
	bool PumpStat = false;      		//Текущее состояние насоса
	bool PowerIndStat = false;   		//Текущее состояние светового индикатора
	boolean ImpFlag = false; //Вспомогательная переменная для хранения наличия/отсутствия импульса в текущий момент
	float Dist = 0;           //Пройденое расстояние за интервал замера скорости

	// GPS data
	String gps_dataisvalid = "1";		// Флаг валидности последней геопозиции
	float gps_speedog = -0.1;			// Speed on ground
	float gps_speed = -0.1;				// Speed, км/ч
	double gps_lastpointdist = 0.0;	// Дистанция до прошлой точки позиционирования, м
	uint32_t gps_Satelites = 0;	// Количество спутников, используемых для позиционирования
	String gps_SatelitesVisible = "*";	// Количество видимых спутников
	int gps_fixmode = -1;	// Режим фиксации =1 и др - Нет фиксации, 2=2D, 3=3D
	int32_t gps_hdop = -1;				// текущая точность позиционирования
	double gps_DistFromLastOiling = 0.0;// Накопитель счетчика пройденного расстояния с последней смазки, м
	String gps_DiagMsg	= "";			// Диагностическое сообщение GPS

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

	// Установка информации о качестве фиксации GPS
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
	int alivebyte = 0;	// индикатор активности контроллера для отладки на веб.
#endif

	ActualValuesVOiler() {

	}
	;

	//----------------------------------------------------------------------
	//             Функция упаковывки всех текущих значений в JSON формат
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
	//             Функция упаковывки всех текущих значений в JSON формат
	//----------------------------------------------------------------------
	String getSensorReadingsJSON_String() {
		String jsonString;
		serializeJson(getSensorReadingsJSON(), jsonString);
		return jsonString;
	}
};

ActualValuesVOiler ActlVal;

#endif /* LIBRARIES_ACTUALVALUESVOILER_H_ */
