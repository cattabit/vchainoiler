/*
 * Config.h
 *
 *  Created on: 3 ���. 2022 �.
 *      Author: vector
 */

#ifndef LIBRARIES_SETTINGSVOOILER_H_
#define LIBRARIES_SETTINGSVOOILER_H_

#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>
#include <LittleFS.h>

//#define DEBUG 1               // ���� ���������� ���������

#define CONFIG_FILE_NAME	"/voiler.settings"

#define MODEMin 1  //���������
#define MODEMax 4 //���������
#define MODEStep 1 //��� �����

#define MODE_TIMERMin 10  //���������
#define MODE_TIMERMax 40 //���������
#define MODE_TIMERStep 10 //��� �����

#define MODE_MILAGEMin 10  //���������
#define MODE_MILAGEMax 40 //���������
#define MODE_MILAGEStep 10 //��� �����

#define MIntervalTownMin 100  //����������� �������� ������ � ������ ����� �� ������� � �
#define MIntervalTownMax 6000 //������������ �������� ������ � ������ ����� �� ������� � �
#define MIntervalTownStep 100 //��� ����� ��������� ������ � ������ ����� �� ������� � �

#define MIntervalRoadMin 100  //����������� �������� ������ � ������ ������ �� ������� � �
#define MIntervalRoadMax 6000 //������������ �������� ������ � ������ ������ �� ������� � �
#define MIntervalRoadStep 100 //��� ����� ��������� ������ � ������ ������ �� ������� � �

#define MCoefRainMin 15       //����������� ������� ���������� ������� ������ � �����
#define MCoefRainMax 195      //������������ ������� ���������� ������� ������ � �����
#define MCoefRainStep 15      //��� ����� �������� ���������� ������� ������ � �����

#define MMinSpeedMin 5        //����������� �������� ����������� �������� ������
#define MMinSpeedMax 40       //������������ �������� ����������� �������� ������
#define MMinSpeedStep 5       //��� ����� �������� ����������� �������� ������

#define MRoadSpeedMin 50      //����������� �������� �������� ����� ������ � ������ �� ������
#define MRoadSpeedMax 140     //������������ �������� �������� ����� ������ � ������ �� ������
#define MRoadSpeedStep 10     //��� ����� �������� �������� ����� ������ � ������ �� ������

#define MWeelCircleMin 1500   //����������� ����� ���������� ������
#define MWeelCircleMax 2500   //������������ ����� ���������� ������
#define MWeelCircleStep 10    //��� ����� �������� ����� ���������� ������

#define MSignalsOn1Min 1      //����������� ����� ��������� �� ������ ������
#define MSignalsOn1Max 25     //������������ ����� ��������� �� ������ ������
#define MSignalsOn1Step 1     //��� ����� ����� ��������� �� ������ ������

#define DurationImpMin 300    //����������� ������������ �������� �� ���� �������
#define DurationImpMax 1000   //������������ ������������ �������� �� ���� �������
#define DurationImpStep 100   //��� ����� ������������ �������� �� ���� �������

#define TIntervalTownMin 10   //����������� �������� � ���, ����� ���������� � ������
#define TIntervalTownMax 220  //������������ �������� � ���, ����� ���������� � ������
#define TIntervalTownStep 20  //��� ����� ���������� � ���, ����� ���������� � ������

#define TIntervalRoadMin 10   //����������� �������� � ���, ����� ���������� �� ������
#define TIntervalRoadMax 220  //������������ �������� � ���, ����� ���������� �� ������
#define TIntervalRoadStep 20  //��� ����� � ���, ����� ���������� �� ������

#define TIntervalRainMin 10   //����������� �������� � ���, ����� ���������� � �����
#define TIntervalRainMax 220  //������������ �������� � ���, ����� ���������� � �����
#define TIntervalRainStep 10  //��� ����� �������� � ���, ����� ���������� � �����

#define PNumberImpMin 5       	//����������� ���������� ��������� �� ���� ��������
#define PNumberImpMax 200      	//������������ ���������� ��������� �� ���� ��������
#define PNumberImpStep 5      	//��� ����� ���������� ��������� �� ���� ��������

#define PImpIntervalMin 800  	//����������� �������� ����� ���������� ��� ��������
#define PImpIntervalMax 2600  	//������������ �������� ����� ���������� ��� ��������
#define PImpIntervalStep 100  	//��� ����� ��������� ����� ���������� ��� ��������

#define gpsMinCalcSpeedMin 3  //����������� �������� ������� ��������� �������� �� GPS, ��/�
#define gpsMinCalcSpeedMax 15  //����������� �������� ������� ��������� �������� �� GPS, ��/�
#define gpsMinCalcSpeedStep 1  //���

#define gpsAgeMaxMin 1000  		//�������� ������������ �������� "��������" ������, �.�. ������������ ����� ������� ������ ����������, ��.
#define gpsAgeMaxMax 100000  	//�������� ������������ �������� "��������" ������, �.�. ������������ ����� ������� ������ ����������, ��.
#define gpsAgeMaxStep 1000  	//���

#define gpsUpdatePeriodMin 1000  //�������� ������������� ���������� ������ GPS, ��
#define gpsUpdatePeriodMax 15000  //�������� ������������� ���������� ������ GPS, ��
#define gpsUpdatePeriodStep 300  //���

// TODO: �������
#define gpsDataWaitMin 20  		//������������ �������� ������ �� ����� ������. �������� ������ ���� �� ������ ������� ���������� ������ � ������
#define gpsDataWaitMax 1000  	//������������ �������� ������ �� ����� ������. �������� ������ ���� �� ������ ������� ���������� ������ � ������
#define gpsDataWaitStep 1  		//���

#define LevelMin 0            	//����������� ������� ����� � %
#define LevelMax 100          	//������������ ������� ����� � %
#define LevelStep 25          	//��� ����� ������ ����� � %

class SettingsVOoiler {
public:

	//������������� ���������, ��������� �������� ������������ ��� ������ �������� ������
	int MODE = 2;              //������� �����  1-MILAGE  2-TIMER  3-PUMP  4-OFF
	//�������� ���������� ������ ����� 1,5 ���

	int MODE_MILAGE = 10; //������� �������� ������ MILAGE, ��� ��������� ������ AUTO
	//10-AUTO  20-RAIN  30-OFF
	//�������� ����������� �������� �������� ������

	int MODE_TIMER = 10; //������� �������� ������ TIMER, ��� ��������� ������ TOWN
	//10-TOWN  20-ROAD  30-RAIN  40-OFF
	//�������� ����������� �������� �������� ������

	int MIntervalTown = 800;    // �������� ������ � ������ ����� �� ������� � �
	int MIntervalRoad = 600;   // �������� ������ � ������ ������ �� ������� � �
	int MCoefRain = 60;           	// ������� ���������� ������� ������ � �����
	int MMinSpeed = 20;           	// ����������� �������� ������
	int MRoadSpeed = 80;          	// �������� ����� ������ � ������ �� ������
	int MWeelCircle = 2010;       	// ����� ���������� ������
	int MSignalsOn1 = 21;         	// ����� ��������� �� ������ ������
	int DurationImp = 400;        	// ������������ �������� �� ���� �������
	unsigned long TIntervalTown = 10; //�������� � ���, ����� ���������� � ������
	unsigned long TIntervalRoad = 60; //�������� � ���, ����� ���������� �� ������
	unsigned long TIntervalRain = 40; //�������� � ��� � �����
	int PNumberImp = 100;          	// ���������� ��������� �� ���� ��������
	int PImpInterval = 600;       	// �������� ����� ���������� ��� ��������

	// Tank Level / ������� � �����
	int impPer1CC = 50;	// ����������� ��������� �� 1�� (�������� ��������� � �����)
	int tankVolumeCC = 200;			// ������� ����� �����, �� ��� ���.��.
	float tankActualLevel = -1;	// ����������� ������� � �����. ��������������� ������ 10-20 ���������.

	// GPS
	int gpsMinCalcSpeed = 8;// ����������� �������� ������� ��������� �������� �� GPS, ��/�
	int gpsMaxCalcDistance = 1000;// ������������ ��������� � ������� ���������� ���������� ������ ��� ������� ��������� �������� �� GPS, �����
	uint32_t gpsAgeMax = 3000;	// �������� ������������ �������� "��������" ������, �.�. ������������ ����� ������� ������ ����������, ��.
	uint32_t gpsUpdatePeriod = 1500; // �������� �������� ������������� ���������� ������ GPS, ��
	uint32_t gpsDataWait = 10;	// ������������ �������� ������ �� ����� ������. �������� ������ ���� �� ������ ������� ���������� ������ � ������ (� ���������� ������, u-center)

	SettingsVOoiler() {
	};

	//----------------------------------------------------------------------
	//             ������� ���������� ����������� ����������, ����������,
	//          ����������� �� ���-����������
	//----------------------------------------------------------------------
	String setSettingsValue(String par, String value) {
#ifdef DEBUG
//		Serial.println("setSettingsValue(" + par + ", " + value + ")..");
#endif
		/// �������� �����-��.. ����� ���������� �����.
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
//				if ((ival * 2) >= PImpInterval) { // ������������� �������� ������ �� ����� ��� �������� ������� ����� ����������
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
	//             ������� ������������ JSON-������� � �������� ���������� ��������
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
	//             ������� ���������� ���� ������� �������� � JSON ������
	//          ������������ ��� ��������
	//----------------------------------------------------------------------
	String getSettingsJSON_String() {
		String jsonString;
		serializeJson(getSettingsJSON(), jsonString);
		return jsonString;
	}

	//----------------------------------------------------------------------
	//    ������� �������� ����� ����������� �������� ���������� (��������)
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
			// ���� ���� � ����������� ����������� - ������� ��� � ���������� ������� �������� ��������.
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
//             ������� ���������� ������� �������� ���������� (��������)
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
	//             ������� �������������� �������� ��-���������
	//----------------------------------------------------------------------
	bool ResetSettings() {
		LittleFS.remove(CONFIG_FILE_NAME);
	}

	//----------------------------------------------------------------------
	//             ������������ �����������
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
