/*
 * Config.h
 *
 *  Created on: 3 июл. 2022 г.
 *      Author: vector
 */

#ifndef LIBRARIES_SETTINGSVOOILER_H_
#define LIBRARIES_SETTINGSVOOILER_H_

#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>
#include <LittleFS.h>

//#define DEBUG 1               // Флаг отладочных сообщений

#define CONFIG_FILE_NAME	"/voiler.settings"

#define MODEMin 1  //Минимальн
#define MODEMax 4 //Максималь
#define MODEStep 1 //Шаг смены

#define MODE_TIMERMin 10  //Минимальн
#define MODE_TIMERMax 40 //Максималь
#define MODE_TIMERStep 10 //Шаг смены

#define MODE_MILAGEMin 10  //Минимальн
#define MODE_MILAGEMax 40 //Максималь
#define MODE_MILAGEStep 10 //Шаг смены

#define MIntervalTownMin 100  //Минимальный интервал смазки в режиме Город по пробегу в м
#define MIntervalTownMax 6000 //Максимальный интервал смазки в режиме Город по пробегу в м
#define MIntervalTownStep 100 //Шаг смены интервала смазки в режиме Город по пробегу в м

#define MIntervalRoadMin 100  //Минимальный интервал смазки в режиме Трасса по пробегу в м
#define MIntervalRoadMax 6000 //Максимальный интервал смазки в режиме Трасса по пробегу в м
#define MIntervalRoadStep 100 //Шаг смены интервала смазки в режиме Трасса по пробегу в м

#define MCoefRainMin 15       //Минимальный процент увеличения частоты смазки в дождь
#define MCoefRainMax 195      //Максимальный процент увеличения частоты смазки в дождь
#define MCoefRainStep 15      //Шаг смены процента увеличения частоты смазки в дождь

#define MMinSpeedMin 5        //Минимальное значение минимальной скорости смазки
#define MMinSpeedMax 40       //Максимальное значение минимальной скорости смазки
#define MMinSpeedStep 5       //Шаг смены значения минимальной скорости смазки

#define MRoadSpeedMin 50      //Минимальное значение скорости смены режима с Города на Трассу
#define MRoadSpeedMax 140     //Максимальное значение скорости смены режима с Города на Трассу
#define MRoadSpeedStep 10     //Шаг смены значения скорости смены режима с Города на Трассу

#define MWeelCircleMin 1500   //Минимальная длина окружности колеса
#define MWeelCircleMax 2500   //Максимальная длина окружности колеса
#define MWeelCircleStep 10    //Шаг смены значения длины окружности колеса

#define MSignalsOn1Min 1      //Минимальное число импульсов на оборот колеса
#define MSignalsOn1Max 25     //Максимальное чисто импульсов на оборот колеса
#define MSignalsOn1Step 1     //Шаг смены числа импульсов на оборот колеса

#define DurationImpMin 300    //Минимальная длительность импульса во всех режимах
#define DurationImpMax 1000   //Максимальная длительность импульса во всех режимах
#define DurationImpStep 100   //Шаг смены длительности импульса во всех режимах

#define TIntervalTownMin 10   //Минимальный интервал в сек, между импульсами в Городе
#define TIntervalTownMax 220  //Максимальный интервал в сек, между импульсами в Городе
#define TIntervalTownStep 20  //Шаг смены интервалов в сек, между импульсами в Городе

#define TIntervalRoadMin 10   //Минимальный интервал в сек, между импульсами на Трассе
#define TIntervalRoadMax 220  //Максимальный интервал в сек, между импульсами на Трассе
#define TIntervalRoadStep 20  //Шаг смены в сек, между импульсами на Трассе

#define TIntervalRainMin 10   //Минимальный интервал в сек, между импульсами в дождь
#define TIntervalRainMax 220  //Максимальный интервал в сек, между импульсами в дождь
#define TIntervalRainStep 10  //Шаг смены интервал в сек, между импульсами в дождь

#define PNumberImpMin 5       	//Минимальное количество импульсов за цикл прокачки
#define PNumberImpMax 200      	//Максимальное количество импульсов за цикл прокачки
#define PNumberImpStep 5      	//Шаг смены количества импульсов за цикл прокачки

#define PImpIntervalMin 800  	//Минимальный интервал между импульсами при прокачке
#define PImpIntervalMax 2600  	//Максимальный интервал между импульсами при прокачке
#define PImpIntervalStep 100  	//Шаг смены интервала между импульсами при прокачке

#define gpsMinCalcSpeedMin 3  //Минимальная скорость расчета дистанции движения по GPS, км/ч
#define gpsMinCalcSpeedMax 15  //Минимальная скорость расчета дистанции движения по GPS, км/ч
#define gpsMinCalcSpeedStep 1  //Шаг

#define gpsAgeMaxMin 1000  		//Значение максимальной величины "старости" данных, т.е. определяется когда считать данные протухшими, мс.
#define gpsAgeMaxMax 100000  	//Значение максимальной величины "старости" данных, т.е. определяется когда считать данные протухшими, мс.
#define gpsAgeMaxStep 1000  	//Шаг

#define gpsUpdatePeriodMin 1000  //Значение периодичности обновления данных GPS, мс
#define gpsUpdatePeriodMax 15000  //Значение периодичности обновления данных GPS, мс
#define gpsUpdatePeriodStep 300  //Шаг

// TODO: удалить
#define gpsDataWaitMin 20  		//Длительность ожидания данных на порту обмена. Наверное должна быть не меньше частоты обновления данных в модуле
#define gpsDataWaitMax 1000  	//Длительность ожидания данных на порту обмена. Наверное должна быть не меньше частоты обновления данных в модуле
#define gpsDataWaitStep 1  		//Шаг

#define LevelMin 0            	//Минимальный уровень масла в %
#define LevelMax 100          	//Максимальный уровень масла в %
#define LevelStep 25          	//Шаг смены уровня масла в %

class SettingsVOoiler {
public:

	//Настраиваемые параметры, указанные значения используются для первой загрузки скетча
	int MODE = 2;              //текущий режим  1-MILAGE  2-TIMER  3-PUMP  4-OFF
	//меняется удержанием кнопки более 1,5 сек

	int MODE_MILAGE = 10; //текущий интервал режима MILAGE, при включении всегда AUTO
	//10-AUTO  20-RAIN  30-OFF
	//меняется однократным коротким нажатием кнопки

	int MODE_TIMER = 10; //текущий интервал режима TIMER, при включении всегда TOWN
	//10-TOWN  20-ROAD  30-RAIN  40-OFF
	//меняется однократным коротким нажатием кнопки

	int MIntervalTown = 800;    // Интервал смазки в режиме Город по пробегу в м
	int MIntervalRoad = 600;   // Интервал смазки в режиме Трасса по пробегу в м
	int MCoefRain = 60;           	// Процент увеличения частоты смазки в дождь
	int MMinSpeed = 20;           	// Минимальная скорость смазки
	int MRoadSpeed = 80;          	// Скорость смены режима с Города на Трассу
	int MWeelCircle = 2010;       	// длина окружности колеса
	int MSignalsOn1 = 21;         	// Число импульсов на оборот колеса
	int DurationImp = 400;        	// Длительность импульса во всех режимах
	unsigned long TIntervalTown = 10; //Интервал в сек, между импульсами в Городе
	unsigned long TIntervalRoad = 60; //Интервал в сек, между импульсами на Трассе
	unsigned long TIntervalRain = 40; //Интервал в сек в дождь
	int PNumberImp = 100;          	// Количество импульсов за цикл прокачки
	int PImpInterval = 600;       	// Интервал между импульсами при прокачке

	// Tank Level / Уровень в бачке
	int impPer1CC = 50;	// Колличество импульсов на 1мл (Замерить прокачкой в шприц)
	int tankVolumeCC = 200;			// Емкость бачка масла, мл или куб.см.
	float tankActualLevel = -1;	// Фактический остаток в бачке. Пересчитывается каждые 10-20 импульсов.

	// GPS
	int gpsMinCalcSpeed = 8;// Минимальная скорость расчета дистанции движения по GPS, км/ч
	int gpsMaxCalcDistance = 1000;// Максимальная дистанция с момента последнего обновления данных для расчета дистанции движения по GPS, метры
	uint32_t gpsAgeMax = 3000;	// Значение максимальной величины "старости" данных, т.е. определяется когда считать данные протухшими, мс.
	uint32_t gpsUpdatePeriod = 1500; // Значение контроля периодичности обновления данных GPS, мс
	uint32_t gpsDataWait = 10;	// Длительность ожидания данных на порту обмена. Наверное должна быть не меньше частоты обновления данных в модуле (в настройках модуля, u-center)

	SettingsVOoiler() {
	};

	//----------------------------------------------------------------------
	//             Функция обновления настроечных параметров, значениями,
	//          полученными от веб-интерфейса
	//----------------------------------------------------------------------
	String setSettingsValue(String par, String value) {
#ifdef DEBUG
//		Serial.println("setSettingsValue(" + par + ", " + value + ")..");
#endif
		/// говнокод какой-то.. потом переделать нужно.
		int ival;
		unsigned long ulval;

		String result = "Atribute " + par + " not found!";

		if (par.length() < 2 || value.length() < 1) {
			result = "par.length = " + String(par.length())
					+ ",  value.length = " + String(value.length())
					+ "! Failed parce values!";
		} else {

			if (par.equals("MODE")) {
				ival = atoi(value.c_str());
				if (MODEMin <= ival <= MODEMax) {
					MODE = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MODEMin + " and " + MODEMax + "!";
				}
			}

			if (par.equals("MODE_MILAGE")) {
				ival = atoi(value.c_str());
				if (MODE_MILAGEMin <= ival <= MODE_MILAGEMax) {
					MODE_MILAGE = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MODE_MILAGEMin + " and " + MODE_MILAGEMax + "!";
				}
			}

			if (par.equals("MODE_TIMER")) {
				ival = atoi(value.c_str());
				if (MODE_TIMERMin <= ival <= MODE_TIMERMax) {
					MODE_TIMER = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MODE_TIMERMin + " and " + MODE_TIMERMax + "!";
				}
			}

			if (par.equals("MIntervalTown")) {
				ival = value.toInt();
				if (MIntervalTownMin <= ival <= MIntervalTownMax) {
					MIntervalTown = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MIntervalTownMin + " and " + MIntervalTownMax
							+ "!";
				}
			}

			if (par.equals("MIntervalRoad")) {
				ival = atoi(value.c_str());
				if (MIntervalRoadMin <= ival <= MIntervalRoadMax) {
					MIntervalRoad = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MIntervalRoadMin + " and " + MIntervalRoadMax
							+ "!";
				}
			}

			if (par.equals("MCoefRain")) {
				ival = atoi(value.c_str());
				if (MCoefRainMin <= ival <= MCoefRainMax) {
					MCoefRain = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MCoefRainMin + " and " + MCoefRainMax + "!";
				}
			}

			if (par.equals("MMinSpeed")) {
				ival = atoi(value.c_str());
				if (MMinSpeedMin <= ival <= MMinSpeedMax) {
					MMinSpeed = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MMinSpeedMin + " and " + MMinSpeedMax + "!";
				}
			}

			if (par.equals("MRoadSpeed")) {
				ival = atoi(value.c_str());
				if (MRoadSpeedMin <= ival <= MRoadSpeedMax) {
					MRoadSpeed = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MRoadSpeedMin + " and " + MRoadSpeedMax + "!";
				}
			}

			if (par.equals("MWeelCircle")) {
				ival = atoi(value.c_str());
				if (MWeelCircleMin <= ival <= MWeelCircleMax) {
					MWeelCircle = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MWeelCircleMin + " and " + MWeelCircleMax + "!";
				}
			}

			if (par.equals("MSignalsOn1")) {
				ival = atoi(value.c_str());
				if (MSignalsOn1Min <= ival <= MSignalsOn1Max) {
					MSignalsOn1 = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ MSignalsOn1Min + " and " + MSignalsOn1Max + "!";
				}
			}

			if (par.equals("DurationImp")) {
				ival = atoi(value.c_str());
//				if ((ival * 2) >= PImpInterval) { // Устанавливаем значение равное не более чем половине времени между импульсами
//					ival = PImpInterval / 2 - DurationImpStep;
//				}
				if (DurationImpMin > ival) {
					DurationImp = DurationImpMin;
					result = "Atribute " + par + " - Set MIN";
				} else if (ival > DurationImpMax) {
					DurationImp = DurationImpMax;
					result = "Atribute " + par + " - Set MAX";
				} else if (DurationImpMin <= ival <= DurationImpMax) {
					DurationImp = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ DurationImpMin + " and " + DurationImpMax + "!";
				}
			}

			//TIntervalTown
			if (par.equals("TIntervalTown")) {
				ulval = atol(value.c_str());
				if (TIntervalTownMin <= ulval <= TIntervalTownMax) {
					TIntervalTown = ulval;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ TIntervalTownMin + " and " + TIntervalTownMax
							+ "!";
				}
			}

			//TIntervalRoad
			if (par.equals("TIntervalRoad")) {
				ulval = atol(value.c_str());
				if (TIntervalRoadMin <= ulval <= TIntervalRoadMax) {
					TIntervalRoad = ulval;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ TIntervalRoadMin + " and " + TIntervalRoadMax
							+ "!";
				}
			}

			//TIntervalRain
			if (par.equals("TIntervalRain")) {
				ulval = atol(value.c_str());
				if (TIntervalRainMin <= ulval <= TIntervalRainMax) {
					TIntervalRain = ulval;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ TIntervalRainMin + " and " + TIntervalRainMax
							+ "!";
				}
			}

			if (par.equals("PNumberImp")) {
				ival = atoi(value.c_str());
				if (PNumberImpMin <= ival <= PNumberImpMax) {
					PNumberImp = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ PNumberImpMin + " and " + PNumberImpMax + "!";
				}
			}

			if (par.equals("PImpInterval")) {
				ival = atoi(value.c_str());
				if ((DurationImp * 2) >= ival) {
					ival = DurationImp * 2 + PImpIntervalStep;
					result = "Atribute " + par + " - set DurationImp*2! ("
							+ ival + ")";
				}
				if (PImpIntervalMin > ival) {
					ival = PImpIntervalMin;
					result = "Atribute " + par + " - Set MIN";
				} else if (PImpIntervalMax < ival) {
					ival = PImpIntervalMax;
					result = "Atribute " + par + " - Set MAX";
				} else if (PImpIntervalMin <= ival <= PImpIntervalMax) {

					PImpInterval = ival;
					result = "Atribute " + par + " - OK";

				} else {
					result = "Atribute " + par + " value should be between "
							+ PImpIntervalMin + " and " + PImpIntervalMax + "!";
				}
			}

			if (par.equals("gpsMinCalcSpeed")) {
				ival = atoi(value.c_str());
				if (gpsMinCalcSpeedMin <= ival <= gpsMinCalcSpeedMax) {
					gpsMinCalcSpeed = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ gpsMinCalcSpeedMin + " and " + gpsMinCalcSpeedMax
							+ "!";
				}
			}
			if (par.equals("gpsAgeMax")) {
				ival = atoi(value.c_str());
				if (gpsAgeMaxMin <= ival <= gpsAgeMaxMax) {
					gpsAgeMax = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ gpsAgeMaxMin + " and " + gpsAgeMaxMax + "!";
				}
			}
			if (par.equals("gpsUpdatePeriod")) {
				ival = atoi(value.c_str());
				if (gpsUpdatePeriodMin > ival)
					gpsUpdatePeriod = gpsUpdatePeriodMin;
				else if (gpsUpdatePeriodMax < ival)
					gpsUpdatePeriod = gpsUpdatePeriodMax;
				else if (gpsUpdatePeriodMin <= ival <= gpsUpdatePeriodMax) {
					gpsUpdatePeriod = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ gpsUpdatePeriodMin + " and " + gpsUpdatePeriodMax
							+ "!";
				}
			}
			if (par.equals("gpsDataWait")) {
				ival = atoi(value.c_str());
				if (gpsDataWaitMin <= ival <= gpsDataWaitMax) {
					gpsDataWait = ival;
					result = "Atribute " + par + " - OK";
				} else {
					result = "Atribute " + par + " value should be between "
							+ gpsDataWaitMin + " and " + gpsDataWaitMax + "!";
				}
			}

		}
		return result;
	}

	//----------------------------------------------------------------------
	//             Функция формирования JSON-объекта с текущими значениями настроек
	//----------------------------------------------------------------------
	StaticJsonDocument<512> getSettingsJSON() {
		StaticJsonDocument<512> doc;

		// Set the values in the document
		doc["MODE"] = MODE;
		doc["MODE_MILAGE"] = MODE_MILAGE;
		doc["MODE_TIMER"] = MODE_TIMER;
		doc["MIntervalTown"] = MIntervalTown;
		doc["MIntervalRoad"] = MIntervalRoad;
		doc["MCoefRain"] = MCoefRain;
		doc["MMinSpeed"] = MMinSpeed;
		doc["MRoadSpeed"] = MRoadSpeed;
		doc["MWeelCircle"] = MWeelCircle;
		doc["MSignalsOn1"] = MSignalsOn1;
		doc["DurationImp"] = DurationImp;
		doc["TIntervalTown"] = TIntervalTown;
		doc["TIntervalRoad"] = TIntervalRoad;
		doc["TIntervalRain"] = TIntervalRain;
		doc["PNumberImp"] = PNumberImp;
		doc["PImpInterval"] = PImpInterval;

		doc["impPer1CC"] = impPer1CC;
		doc["tankVolueCC"] = tankVolumeCC;
		doc["tankActualLevel"] = tankActualLevel;
		doc["gpsMinCalcSpeed"] = gpsMinCalcSpeed;
		doc["gpsAgeMax"] = gpsAgeMax;
		doc["gpsUpdatePeriod"] = gpsUpdatePeriod;
		doc["gpsDataWait"] = gpsDataWait;

		return doc;
	}

	//----------------------------------------------------------------------
	//             Функция упаковывки всех текущих значений в JSON формат
	//          Используется для передачи
	//----------------------------------------------------------------------
	String getSettingsJSON_String() {
		String jsonString;
		serializeJson(getSettingsJSON(), jsonString);
		return jsonString;
	}

	//----------------------------------------------------------------------
	//    Функция загрузки ранее сохраненных значений параметров (настроек)
	//----------------------------------------------------------------------
	bool LoadSettings() {
#ifdef DEBUG
		Serial.println("LoadSettings() started.");
#endif
		// Allocate a temporary JsonDocument
		// Don't forget to change the capacity to match your requirements.
		// Use https://arduinojson.org/v6/assistant to compute the capacity.
		StaticJsonDocument<512> doc;

		File file = LittleFS.open(CONFIG_FILE_NAME, "r");
		if (!file) {
#ifdef DEBUG
			Serial.println("Failed to open file  for reading");
#endif
			// Если файл с настройками отсутствует - создаем его и записываем текущие значения настроек.
			SaveSettings();
		}

		// Deserialize the JSON document
		DeserializationError error = deserializeJson(doc, file);
		file.close();
		if (error) {
#ifdef DEBUG
			Serial.println(
					F("Failed to read file, using default configuration"));
#endif
			return false;
		} else {

#ifdef DEBUG
			String jsonString;
			serializeJson(doc, jsonString);
			Serial.println(jsonString);
			Serial.printf("Parce setting pairs from file:\n");
#endif

			// Copy values from the JsonDocument to the Config
			JsonObject root = doc.as<JsonObject>();

			// using C++11 syntax (preferred):
			for (JsonPair kv : root) {
#ifdef DEBUG
				Serial.printf("('%s': %s)\n", kv.key().c_str(),
						kv.value().as<String>());
				Serial.println(
						setSettingsValue(kv.key().c_str(),
								kv.value().as<String>()));
#else
				setSettingsValue(kv.key().c_str(), kv.value().as<String>());
#endif
			}

#ifdef DEBUG
			Serial.println(F("Configuration loaded successfully:"));
			Serial.println(getSettingsJSON_String());
#endif
			return true;
		}
	}

//----------------------------------------------------------------------
//             Функция сохранения текущих значений параметров (настроек)
//----------------------------------------------------------------------
	bool SaveSettings() {
		// Allocate a temporary JsonDocument
		// Don't forget to change the capacity to match your requirements.
		// Use https://arduinojson.org/assistant to compute the capacity.
		StaticJsonDocument<256> doc = getSettingsJSON();

#ifdef DEBUG
		Serial.printf("saveSettings( ");
		Serial.printf( CONFIG_FILE_NAME);
		Serial.println(")..");
		if (!LittleFS.exists(CONFIG_FILE_NAME))
			Serial.println("File doesn't exist ");
		else
			Serial.println("File exists ");
#endif
		LittleFS.remove(CONFIG_FILE_NAME);

		File file = LittleFS.open(CONFIG_FILE_NAME, "w");
#ifdef DEBUG
		if (!file) {
			Serial.println("Failed to open file for writing");
		}
#endif

		// Serialize JSON to file
		if (serializeJson(doc, file) == 0) {
#ifdef DEBUG
			Serial.println("Failed to write to file ");
#endif
			return false;
		}
		file.close();

		return true;
	}

	//----------------------------------------------------------------------
	//             Функция восстановления значений по-умолчанию
	//----------------------------------------------------------------------
	bool ResetSettings() {
		LittleFS.remove(CONFIG_FILE_NAME);
	}

	//----------------------------------------------------------------------
	//             Перезагрузка контроллера
	//----------------------------------------------------------------------
	void reboot() {
		wdt_disable();
		wdt_enable(WDTO_15MS);
		while (1) {
		}
	}

private:

};

SettingsVOoiler Stgs;

#endif /* LIBRARIES_SETTINGSVOOILER_H_ */
