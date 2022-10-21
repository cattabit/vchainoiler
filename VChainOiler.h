// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Template8266_H_
#define _Template8266_H_

#pragma once
#include "Arduino.h"
//add your includes for the project Template8266 here
#include <Arduino.h>
#include <Bounce2.h>          //���������� ���������� �������� ������
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "libraries/timer.h"
#include "libraries/SettingsVOoiler.h"
#include "libraries/ActualValuesVOiler.h"
#include "libraries/GPS_TinyGPSPP.h"
#include "libraries/WebServer.h"
#include "libraries/WiFi.h"

//end of add your includes here

//add your function definitions for the project Template8266 here

//https://randomnerdtutorials.com/install-esp8266-nodemcu-littlefs-arduino/  - Install ESP8266 NodeMCU LittleFS Filesystem Uploader in Arduino IDE
//https://randomnerdtutorials.com/esp8266-web-server-gauges/ - ESP8266 NodeMCU Web Server: Display Sensor Readings in Gauges

//#define BUTTON_PIN_1 13     //��� ����������� ������ ����� - ������ "�����"
//#define BUTTON_PIN_2 15     //��� ����������� ������ Setup - ������ "�����"
//#define ImpP 3              //��� ����������� ������� �������� - ������ "�����"
//����� �������� �� 3 , ��� ���� �������� ����� �������� ���������� �� "1" (http://arduino.ru/Reference/AttachInterrupt)
#define PumpP 4               //��� ����������� ������  - ������ "1" (+5�)
#define PowerInd 2            // ��������� ��������� �����
#define pressed_long 1500     //����������������� ����������� ������� ������ 1,5 ���

#define DR 13                 //��� ����������� RGB ����������, ����� R
#define DG 14                 //��� ����������� RGB ����������, ����� G
#define DB 12                 //��� ����������� RGB ����������, ����� B

//#define DEBUG 1               // ���� ���������� ���������

// ��������� ������ ���������� �������������
//Config settings = Config();

Timer timerBetweenImpulses(Stgs.PImpInterval);
Timer timerImpulseDuration(Stgs.DurationImp);

unsigned long OdoInt = 100000; //������ ����� ����������� ������ (� ������) - ���������� � ����������� �� ��������, 100000 - ��������
unsigned long Pcur_ms = 0; //������� �������� ��������� ������� ��� ���������� ��������� ��������
unsigned long Pprev_ms = 0; //���������� �������� ��������� ������� ��� ���������� ��������� ��������
unsigned long TPumpON = 0;  	//����� c ������� ��������� ������
float prevSpeed = 0;    	// ���������� �������� �������� ��� ����������. * 10
unsigned long mSpeed = 0; //������� �������� ��������� ������� ��� ������ ��������
unsigned long cur_ms = 0;    //������� �������� ������� ��� ������ ��������
unsigned long prev_ms = 0; 	//���������� �������� ������� ��� ������ ��������
unsigned long IntrS = 400; //�������� �������, �� ������� ������������� �������� � ��
volatile unsigned long Imp = 0; //���������� ��������� ��� ������� ��������

int MODEprev = 0; // ������ �������� ����������� ������ ��� ����������� � ���� ��� �������� ������������.

int ind_on = LOW;             // ��������� �����
int PowerIndCnt = 0;           //���������� ������ ��������� ����������

unsigned long currentInd = 0;
unsigned long previousInd = 0;

unsigned long pressed_moment1;  // ������ ������� ������ �����
Bounce bouncer1 = Bounce();     //������� ��������� ������ Bounce ������ �����

////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------
//             ������� ���������� ������� �����������
//                 (������� � ������� ��������)
//----------------------------------------------------------------------
void ImpScore()
{
	Imp++; //����������� ������� ��������� �� 1
}

//----------------------------------------------------------------------
//             ������� ������ ��������
// ������������ ActualValuesVO.Speed-������� �������� � ActualValuesVO.Odo -  ������ � ������ �� ���������� �������� ������
//----------------------------------------------------------------------
void Speedometer()
{     //����������� ��������
	if (Stgs.MODE == 1) {
		cur_ms = millis();       //��������� ������� �����
		mSpeed = cur_ms - prev_ms; //��������� ������� �������� � ���������� (prev_ms)
		if (mSpeed >= IntrS) //���� ������ ���������� �������, �� ��������� ���������
		        {
			prev_ms = cur_ms; //���������� ����������� �������� ������� ������������� �������

			//ActualValuesVO.Speed = SettingsVO.MSignalsOn1;
			ActlVal.Speed = ((prevSpeed * 2 + float(Stgs.MSignalsOn1)) / 3); // ����������� �������� �� �����������.
			prevSpeed = ActlVal.Speed;

			if (ActlVal.Speed != 0) { //���� ���������� ��������� �� ����� 0

			}
			ActlVal.Dist = float(mSpeed) / 1000 / 3600 * ActlVal.Speed * 1000
			        * 1000; // ������ �� ����������� ����� � ��
			ActlVal.Odo += ActlVal.Dist; // / 1000;     //������� ����� ������ � ������, ��������� ��������� ������ � ���������� �������� (� ��)

			ActlVal.Dist = 0;               //�������� ������ �� ��������
			mSpeed = 0;              //�������� �������� ������� ������ ��������
		}
	}
}

//---------------------------------------------------------------------
//           ������� ���������� ������� � ������ "�� �������"
//---------------------------------------------------------------------

void WorkPumpM()
{ //������� ���������� ������� ���������/���������� ������
	//��� ������ �������, ������ ���� ����������� �������� ActualValuesVO.Odo � OdoInt
	if (OdoInt > 0) {   //���� ���������� ��������� ������ ����� ��������

		if (ActlVal.PumpStat) {            //���� ������ ������ "�������"
			if (Stgs.MODE_MILAGE == 10 || Stgs.MODE_MILAGE == 20) { // ������ �������� ������ ���� ������� ����� "����" ��� "�����"
				digitalWrite(PumpP, HIGH); //�������� ����� - ������ �������� ������ ������
				ActlVal.PumpStat = true;    //���������� ��������� ������
			}
			if ((millis() - TPumpON) > Stgs.DurationImp) //��������� ����� ������ ������ � ������� ���������, ���� ���� ��������� ��:
			        {
				ActlVal.PumpStat = false; //��������� ������ ������ � "��������"
			}
		}
		if (!ActlVal.PumpStat) {          //���� ������ ������ "��������"
			digitalWrite(PumpP, LOW);       //��������� �����
			ActlVal.PumpStat = false; //��������� ������ ������ � "��������"

			if (ActlVal.gps_DistFromLastOiling > OdoInt ) //��������� �� ������ �� ������ ��������,���� ������ ��
			        {
				ActlVal.PumpStat = true; //������ ������ ������ �� "�������"
				TPumpON = millis();           //��������� ����� ��������� ������
				ActlVal.gps_DistFromLastOiling = 0;               //��������� ����� ��������
				ActlVal.gps_DistFromLastOiling	= 0;
			}
		}
	}
}

//----------------------------------------------------------------------
//             ������� ������
//----------------------------------------------------------------------
void Oiling()
{             //������� ���������� ������� � ����������� �� ������

	switch (Stgs.MODE)
	{

	//����� "�� �������" - �������������� ������ �� �������
	case 1:
		if (MODEprev != Stgs.MODE) {
			MODEprev = Stgs.MODE;
			Web_SendSettingsData();
#ifdef DEBUG
			Serial.printf("\nMODE=1..");
#endif
		}
		ActlVal.PUMP_count = 0; //�������� ���������� ���������� ��������� ��������, ������������ ��������
		ActlVal.TimeLeft = 0; //���������� ������� ������� �� ������

		if (Stgs.MODE_MILAGE == 10 || Stgs.MODE_MILAGE == 20) { // ������ �������� ������ ���� ������� ����� "����" ��� "�����"
			if (ActlVal.Speed > Stgs.MMinSpeed) //���� �������� ������ ����������� �������� ������
			        {
				if (ActlVal.Speed > Stgs.MRoadSpeed) //���� �������� ���� ���������
				        {
					if (ActlVal.Speed > 150) //���� �������� ������ 150 ��/� - ��������� ������
					        {
						digitalWrite(PumpP, LOW); //��������� ����� (������ �� ������)
						ActlVal.PumpStat = false; //��������� ������ ������ � "��������"
						OdoInt = 100000; //������ ������������� 100�� (������ �� ������)
					} else       //���� �������� ���� ��������� (���� �� ������)
					{
						OdoInt = Stgs.MIntervalRoad; //������ ������������� ������ ��� ������
						if (Stgs.MODE_MILAGE == 20)
						    OdoInt = OdoInt / ((100 + Stgs.MCoefRain) / 100); //���� ������������ ����� �����, �� ����������� ������������� �������� OdoInt �� �������� %
						WorkPumpM();      //��������� ������� ���������� �������
					}
				} else            //���� �������� ���������
				{
					OdoInt = Stgs.MIntervalTown; //������ ������������� ������ ��� ������
					if (Stgs.MODE_MILAGE == 20)
					    OdoInt = OdoInt / ((100 + Stgs.MCoefRain) / 100); //���� ������������ ����� �����, �� ����������� ������������� �������� OdoInt �� �������� %
					WorkPumpM();          //��������� ������� ���������� �������
				}
			} else            //���� �������� ������ ����������� �������� ������
			{
				digitalWrite(PumpP, LOW);   //��������� ����� (������ �� ������)
				ActlVal.PumpStat = false; //��������� ������ ������ � "��������"
				OdoInt = 100000; //������ ������������� 100�� (������ �� ������)
			}
			break;
		}
		if (Stgs.MODE_MILAGE == 30) {            //���� ���������
			digitalWrite(PumpP, LOW);       //��������� ����� (������ �� ������)
			ActlVal.PumpStat = false; //��������� ������ ������ � "��������"
			OdoInt = 100000;     //������ ������������� 100�� (������ �� ������)
		}
		break;

		//����� "�� �������"  �������������� ������ �� ��������� �������
	case 2:		// MODE_TIMER
		if (MODEprev != Stgs.MODE) {
			MODEprev = Stgs.MODE;
			timerBetweenImpulses.reset();
			Web_SendSettingsData();
#ifdef DEBUG
			Serial.printf("\nMODE=2..");
#endif
		}
		ActlVal.PUMP_count = 0; //�������� ���������� ���������� ��������� ��������, ������������ ��������
		timerImpulseDuration.setPeriod(Stgs.DurationImp);// ������������� ������������ �������� ��������� (�� ��������)

		if (Stgs.MODE_TIMER == 40) {
			ActlVal.TimeLeft = 0; //���������� ������� ������� �� ������

			digitalWrite(PumpP, LOW);  //��������� �����
			ActlVal.PumpStat = false; //��������� ������ ������ � "��������"
		} else {
			switch (Stgs.MODE_TIMER)
			{ //� ����������� �� ���������� ��������� �������� ������:
			case 10:
				ActlVal.TimeLeft = Stgs.TIntervalTown
				        - timerBetweenImpulses.getTimeLeft() / 1000; //���������� ������� ������� �� ������
				timerBetweenImpulses.setPeriod(Stgs.TIntervalTown * 1000);
				break;
			case 20:
				ActlVal.TimeLeft = Stgs.TIntervalRoad
				        - timerBetweenImpulses.getTimeLeft() / 1000; //���������� ������� ������� �� ������
				timerBetweenImpulses.setPeriod(Stgs.TIntervalRoad * 1000);
				break;
			case 30:
				ActlVal.TimeLeft = Stgs.TIntervalRain
				        - timerBetweenImpulses.getTimeLeft() / 1000; //���������� ������� ������� �� ������
				timerBetweenImpulses.setPeriod(Stgs.TIntervalRain * 1000);
				break;
			case 40:
				break;
			default:
				break;
			}

			if (timerBetweenImpulses.ready()) { //��������� �� ������ �� ������ ��������, ���� ������ �� �������� ����� �������
				Pprev_ms = Pcur_ms;          // ��������� ����� ������ ��������
				digitalWrite(PumpP, HIGH);   //�������� �����
#ifdef DEBUG
				if (ActlVal.PumpStat == false)
					Serial.println("ActualValuesVO.PumpStat = true;  ");
#endif
				ActlVal.PumpStat = true;    //���������� ��������� ������
				timerImpulseDuration.reset();

			} else {                        //���� ������� ��� �� ���������, ��
				if (timerImpulseDuration.ready()) { //���������, ����������� �� ����� ��������� �����
					digitalWrite(PumpP, LOW); //���� ���������� - ��������� �����

#ifdef DEBUG
					if (ActlVal.PumpStat == true)
						Serial.println("ActualValuesVO.PumpStat = false;  ");
#endif
					ActlVal.PumpStat = false; //��������� ������ ������ � "��������"

				}
			}
		}
//		case 10:                         //10-TOWN
//			Pcur_ms = millis();            //��������� ������� �����
//
//			ActualValuesVO.TimeLeft = (SettingsVO.TIntervalTown
//					- (Pcur_ms - Pprev_ms) / 1000); //���������� ������� ������� �� ������
//
//			if ((Pcur_ms - Pprev_ms) > (SettingsVO.TIntervalTown * 1000)) { //��������� �� ������ �� ������ ��������, ���� ������ �� �������� ����� �������
//				Pprev_ms = Pcur_ms;          // ��������� ����� ������ ��������
//				digitalWrite(PumpP, HIGH);   //�������� �����
//				ActualValuesVO.PumpStat = true;    //���������� ��������� ������
//			} else {                        //���� ������� ��� �� ���������, ��
//				if (Pcur_ms - Pprev_ms > SettingsVO.DurationImp) { //���������, ����������� �� ����� ��������� �����
//					digitalWrite(PumpP, LOW); //���� ���������� - ��������� �����
//					ActualValuesVO.PumpStat = false; //��������� ������ ������ � "��������"
//				}
//			}
//			break;
//		case 20:         //20-ROAD
//			Pcur_ms = millis(); //��������� ������� �����
//
//			ActualValuesVO.TimeLeft = (SettingsVO.TIntervalRoad
//					- (Pcur_ms - Pprev_ms) / 1000); //���������� ������� ������� �� ������
//
//			if (Pcur_ms - Pprev_ms > (SettingsVO.TIntervalRoad * 1000)) { //��������� �� ������ �� ������ ��������, ���� ������ �� �������� ����� �������
//				Pprev_ms = Pcur_ms;     // ��������� ����� ������ ��������
//				digitalWrite(PumpP, HIGH);            //�������� �����
//				ActualValuesVO.PumpStat = true;    //���������� ��������� ������
//			} else { //���� ������� ��� �� ���������, ��
//				if (Pcur_ms - Pprev_ms > SettingsVO.DurationImp) { //���������, ����������� �� ����� ��������� �����
//					digitalWrite(PumpP, LOW); //���� ���������� - ��������� �����
//					ActualValuesVO.PumpStat = false; //��������� ������ ������ � "��������"
//				}
//			}
//
//			break;
//		case 30:         //30-RAIN
//			Pcur_ms = millis(); //��������� ������� �����
//
//			ActualValuesVO.TimeLeft = (SettingsVO.TIntervalRain
//					- (Pcur_ms - Pprev_ms) / 1000); //���������� ������� ������� �� ������
//
//			if ((Pcur_ms - Pprev_ms) > (SettingsVO.TIntervalRain * 1000)) { //��������� �� ������ �� ������ ��������, ���� ������ �� �������� ����� �������
//				Pprev_ms = Pcur_ms;     // ��������� ����� ������ ��������
//				digitalWrite(PumpP, HIGH);            //�������� �����
//				ActualValuesVO.PumpStat = true;    //���������� ��������� ������
//
//				Serial.println("ActualValuesVO.PumpStat = true;  ");
//			} else { //���� ������� ��� �� ���������, ��
//				if (Pcur_ms - Pprev_ms > SettingsVO.DurationImp) { //���������, ����������� �� ����� ��������� �����
//					digitalWrite(PumpP, LOW); //���� ���������� - ��������� �����
//					ActualValuesVO.PumpStat = false; //��������� ������ ������ � "��������"
//
//					Serial.println("ActualValuesVO.PumpStat = false;  ");
//				}
//			}
//			break;
//
//		default:
//			break;
//		}
		break;

		//����� "PUMP"  �������������� ������ ��������� ���������� ���������
	case 3:
		if (MODEprev != Stgs.MODE) {
			ActlVal.PUMP_count = Stgs.PNumberImp;
			timerBetweenImpulses.reset();
			MODEprev = Stgs.MODE;
			Web_SendSettingsData();
#ifdef DEBUG
			Serial.printf("\nMODE=3.. PUMP_count=");
			Serial.println(String(ActlVal.PUMP_count));
#endif;
		}

		ActlVal.TimeLeft = timerBetweenImpulses.getTimeLeft() / 1000; //���������� ������� ������� �� ������
		timerImpulseDuration.setPeriod(Stgs.DurationImp);// ������������� ������������ �������� ��������� (�� ��������)
		timerBetweenImpulses.setPeriod(Stgs.PImpInterval);

		if (ActlVal.PUMP_count > 0) {
			if (timerBetweenImpulses.ready()) { //��������� �� ������ �� ������ ��������, ���� ������ �� �������� ����� �������
				digitalWrite(PumpP, HIGH);               	// �������� �����
				ActlVal.PumpStat = true;    //���������� ��������� ������
				timerImpulseDuration.reset();

#ifdef DEBUG
				if (ActlVal.PumpStat == false)
					Serial.println("ActualValuesVO.PumpStat = true;  ");
#endif

			} else { 						//���� ������� ��� �� ���������, ��
				if (timerImpulseDuration.ready()) { //���������, ����������� �� ����� ��������� �����
					if (ActlVal.PumpStat == true) {
						digitalWrite(PumpP, LOW); //���� ���������� - ��������� �����

						ActlVal.PumpStat = false; //��������� ������ ������ � "��������"
						ActlVal.PUMP_count--;

#ifdef DEBUG
						if (ActlVal.PumpStat == true)
							Serial.println(
									"ActualValuesVO.PumpStat = false;  ");
						Serial.printf("\nMODE=3.. PUMP_count=");
												Serial.println(String(ActlVal.PUMP_count));
#endif
					}
				}
			}
		} else {
			// ����� �������� ����������� - ��������� � ���������� �����? ��� ���������?
			Serial.printf(
			        "\nMODE=3.. PUMP_count=0. So pump finished. Set MODE = OFF");
			Stgs.MODE = 4;
		}
		break;

		//����� "OFF" - ���������� ���� ������� ������
	case 4:
		if (MODEprev != Stgs.MODE) {
			MODEprev = Stgs.MODE;
			Web_SendSettingsData();
#ifdef DEBUG
			Serial.printf("\nMODE=4..");
#endif
		}
//		Serial.printf("\nMODE=4..");
		ActlVal.PUMP_count = 0; //�������� ���������� ���������� ��������� ��������, ������������ ��������
		ActlVal.TimeLeft = 0; //���������� ������� ������� �� ������
		digitalWrite(PumpP, LOW);  //��������� ����� (������ �� ������)
		ActlVal.PumpStat = false;  //��������� ������ ������ � "��������"
		break;
	}
	MODEprev = Stgs.MODE;
}

//----------------------------------------------------------------------
//             ������� ��������� �������� ������ ������
//----------------------------------------------------------------------
//��� ��������� ������������ RGB ��������� � ����� ������ (����� "+")
//��� ������ �� ���� � ����� �������, �������� ��� LOW �� HIGH � �������� HIGH �� LOW.
void Indicator()
{
	/* if (ActualValuesVO.PumpStat == true) {     //���� ����� �������, �������� ������� ���� �� �������� �� �������� ������
	 digitalWrite(DR, LOW);    //�������� �������
	 digitalWrite(DB, HIGH);   //��������� �����
	 digitalWrite(DG, HIGH);   //��������� �������
	 }
	 else
	 {
	 switch (SettingsVO.MODE) {

	 case 1:                             //��������� ������ "�� �������"
	 switch (SettingsVO.MODE_MILAGE) {
	 case 10:                        //"����" - ������� �������� ����� - ������� ��������� (0,8 ���)
	 digitalWrite(DR, HIGH);       //��������� �������
	 digitalWrite(DB, HIGH);       //��������� �����
	 currentInd = millis();        //���������� ������� �����
	 if (currentInd - previousInd > 800) {   //���� ������ 0,8��� (�������� �������)
	 previousInd = currentInd  ; //������������� ���������� �������� �������
	 if (ind_on == LOW)          //���� ���� �����
	 ind_on = HIGH;            //������������� ��������� ����� � �������
	 else
	 ind_on = LOW;             //����� ������������� ��������� ����� � ��������
	 digitalWrite(DG, ind_on);   //��������� ������
	 }

	 break;
	 case 20:                        //"�����" - ������� �������� ����� - �������� ��������� (0,4 ���)
	 digitalWrite(DR, HIGH);
	 digitalWrite(DB, HIGH);
	 currentInd = millis();
	 if (currentInd - previousInd > 400) {
	 previousInd = currentInd  ;
	 if (ind_on == LOW)
	 ind_on = HIGH;
	 else
	 ind_on = LOW;
	 digitalWrite(DG, ind_on);
	 }
	 break;
	 case 30:                        //"����" - ��������� ����� ������� ����
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, HIGH);
	 break;
	 }
	 break;
	 case 2:                               //��������� ������ "�� �������"
	 switch (SettingsVO.MODE_TIMER) {               //� ����������� �� ���������� ��������� �������� ������:
	 case 10:                         //"�����" - ������� ������ ����� - ������� ��������� (1 ���)
	 digitalWrite(DG, HIGH);
	 digitalWrite(DR, HIGH);
	 currentInd = millis();
	 if (currentInd - previousInd > 1000) {
	 previousInd = currentInd  ;
	 if (ind_on == LOW)
	 ind_on = HIGH;
	 else
	 ind_on = LOW;
	 digitalWrite(DB, ind_on);
	 }
	 break;
	 case 20:                         //"������" - ������� ������ ����� - ������� ��������� (0,6 ���)
	 digitalWrite(DG, HIGH);
	 digitalWrite(DR, HIGH);
	 currentInd = millis();
	 if (currentInd - previousInd > 600) {
	 previousInd = currentInd  ;
	 if (ind_on == LOW)
	 ind_on = HIGH;
	 else
	 ind_on = LOW;
	 digitalWrite(DB, ind_on);
	 }
	 break;
	 case 30:                         //"�����" - ������� ������ ����� - �������� ��������� (0,3 ���)
	 digitalWrite(DG, HIGH);
	 digitalWrite(DR, HIGH);
	 currentInd = millis();
	 if (currentInd - previousInd > 300) {
	 previousInd = currentInd  ;
	 if (ind_on == LOW)
	 ind_on = HIGH;
	 else
	 ind_on = LOW;
	 digitalWrite(DB, ind_on);
	 }
	 break;
	 case 40:                        //"����" - ��������� ����� ������� ����
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, HIGH);
	 break;
	 }
	 break;
	 case 3:                              //��������� ������ "��������" - ����� ������� � ������� ������������
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, LOW);
	 break;

	 case 4:                              //"����" - ��������� ����� ������� ����
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, HIGH);
	 break;
	 }
	 if (ActualValuesVO.PumpStat == true) {     //���� ����� �������, �������� ������� ���� �� �������� �� �������� ������
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, HIGH);
	 }
	 }
	 */

	int timeoff = 1000;    // ������������ ��������� ����� ��������� "���������"
	switch (Stgs.MODE)
	{
	case 1:                        //�� �������
		currentInd = millis();        //���������� ������� �����
		if (ActlVal.PowerIndStat) {
			if (currentInd - previousInd > 500) { //���� ������ 0,5��� (�������� �������)
				previousInd = currentInd;
				digitalWrite(PowerInd, LOW);    //��������
				ActlVal.PowerIndStat = false;
			}
		} else {
			if (currentInd - previousInd > timeoff) { //���� ������ 0,5��� (�������� �������)
				previousInd = currentInd;
				digitalWrite(PowerInd, HIGH);    //��������
				ActlVal.PowerIndStat = true;
			}
		}
		PowerIndCnt = 0;

		break;
	case 2:                        // �� �������
		currentInd = millis();
		if (ActlVal.PowerIndStat) {
			if (currentInd - previousInd > 500) { //���� ������ 0,5��� (�������� �������)
				previousInd = currentInd;
				digitalWrite(PowerInd, LOW);    //��������
				ActlVal.PowerIndStat = false;
				if (PowerIndCnt < 3) {
					timeoff = 300; // 2 �����
					PowerIndCnt++;
				} else {
					timeoff = 1000;
					PowerIndCnt = 0;
				}
			}
		} else {
			if (currentInd - previousInd > timeoff) { //���� ������ 0,5��� (�������� �������)
				previousInd = currentInd;
				digitalWrite(PowerInd, HIGH);    //��������
				ActlVal.PowerIndStat = true;
			}
		}
		break;
	case 3:                        //"����" -
		currentInd = millis();
		if (ActlVal.PowerIndStat) {
			if (currentInd - previousInd > 500) { //���� ������ 0,5��� (�������� �������)
				previousInd = currentInd;
				digitalWrite(PowerInd, LOW);    //��������
				ActlVal.PowerIndStat = false;
				if (PowerIndCnt < 4) {
					timeoff = 300;
					PowerIndCnt++;
				} else {
					timeoff = 1000;
					PowerIndCnt = 0;
				}
			}
		} else {
			if (currentInd - previousInd > timeoff) { //���� ������ 0,5��� (�������� �������)
				previousInd = currentInd;
				digitalWrite(PowerInd, HIGH);    //��������
				ActlVal.PowerIndStat = true;
			}
		}
		break;
	}
}

//----------------------------------------------------------------------
//             ������� �������������� ������� ������� ������
//----------------------------------------------------------------------
void serialHandler()
{
	char inChar;
	if (bouncer1.update()) //���� ��������� �������, ������ ����� �������� ���� ���������
	{
		if (bouncer1.read() == 0)
			pressed_moment1 = millis(); //���� ������ ����� ������,���������� ����� �������
		else //���� ������ ���� ��������
		{
			if ((millis() - pressed_moment1) < pressed_long) // ���� ������ ���� ������ �������������� - ������ �����:
			{
				switch (Stgs.MODE)
				{ //����������� �������� �������� ������
				case 1:             //1-MILAGE
					Stgs.MODE_MILAGE = Stgs.MODE_MILAGE + 10; //����������� ������� �������� ������ MILAGE
					events.send(Stgs.getSettingsJSON_String().c_str(),
					        "new_settings", millis());
					if (Stgs.MODE_MILAGE > 30) Stgs.MODE_MILAGE = 10; //������� �� �����

					ActlVal.PUMP_count = 0; //�������� ���������� ���������� ��������� ��������, ������������ ��������
					break;
				case 2:             //2-TIMER
					Stgs.MODE_TIMER = Stgs.MODE_TIMER + 10; //����������� ������� �������� ������ TIMER
					events.send(Stgs.getSettingsJSON_String().c_str(),
					        "new_settings", millis());
					if (Stgs.MODE_TIMER > 40) Stgs.MODE_TIMER = 10; //������� �� �����

					ActlVal.PUMP_count = 0; //�������� ���������� ���������� ��������� ��������, ������������ ��������
					break;
				case 3:             //3-PUMP
					if (ActlVal.PUMP_count > 0) {
						ActlVal.PUMP_count = 0; //�������� ���������� ���������� ��������� ��������, ������������ ��������
						digitalWrite(PumpP, LOW);  // ��������� �����
						ActlVal.PumpStat = false; //��������� ������ ������ � "��������"
					} else ActlVal.PUMP_count = Stgs.PNumberImp; //��������� ���� ��������
					break;
				case 4:      //4-OFF � ������ OFF �� ��� �� ��������� �� �������
					ActlVal.PUMP_count = 0; //�������� ���������� ���������� ��������� ��������, ������������ ��������
					break;
				default:
					break;
				}
			} else //���������
			{
				//�������� �� ��������� ������
				Stgs.MODE = Stgs.MODE + 1; //�����������  �����
				events.send(Stgs.getSettingsJSON_String().c_str(),
				        "new_settings", millis());
				if (Stgs.MODE > 4) Stgs.MODE = 1; //������� ������� �� �����
			}
		}
	}
}

// WebServer additional handlers

//----------------------------------------------------------------------
//             Initialize LittleFS
//----------------------------------------------------------------------
void initFS()
{
#ifdef DEBUG
	Serial.println(
			LittleFS.begin() ?
					"LittleFS mounted successfully" :
					"An error has occurred while mounting LittleFS!");
#else
	LittleFS.begin();
#endif
}

//Do not add code below this line
#endif /* _Template8266_H_ */
