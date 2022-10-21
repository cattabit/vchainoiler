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
#include <Bounce2.h>          //библиотека устранения дребезга кнопки
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

//#define BUTTON_PIN_1 13     //пин подключения кнопки Режим - сигнал "земля"
//#define BUTTON_PIN_2 15     //пин подключения кнопки Setup - сигнал "земля"
//#define ImpP 3              //Пин подключения датчика скорости - сигнал "земля"
//Можно поменять на 3 , при этом изменить номер внешнего прерывания на "1" (http://arduino.ru/Reference/AttachInterrupt)
#define PumpP 4               //Пин подключения насоса  - сигнал "1" (+5в)
#define PowerInd 2            // Индикатор включения платы
#define pressed_long 1500     //продолжительность длительного нажатия кнопки 1,5 сек

#define DR 13                 //Пин подключения RGB светодиода, вывод R
#define DG 14                 //Пин подключения RGB светодиода, вывод G
#define DB 12                 //Пин подключения RGB светодиода, вывод B

//#define DEBUG 1               // Флаг отладочных сообщений

// Экземпляр класса управления конфигурацией
//Config settings = Config();

Timer timerBetweenImpulses(Stgs.PImpInterval);
Timer timerImpulseDuration(Stgs.DurationImp);

unsigned long OdoInt = 100000; //Пробег между включениями насоса (в метрах) - изменяется в зависимости от скорости, 100000 - выключен
unsigned long Pcur_ms = 0; //текущее значение интервала времени для управления импульсом прокачки
unsigned long Pprev_ms = 0; //предыдущее значение интервала времени для управления импульсом прокачки
unsigned long TPumpON = 0;  	//Время c момента включения насоса
float prevSpeed = 0;    	// предыдущее значение скорости для усреднения. * 10
unsigned long mSpeed = 0; //текущее значение интервала времени для замера скорости
unsigned long cur_ms = 0;    //текущее значение времени для замера скорости
unsigned long prev_ms = 0; 	//предыдущее значение времени для замера скорости
unsigned long IntrS = 400; //интервал времени, за который расчитывается скорость в мс
volatile unsigned long Imp = 0; //количество импульсов для расчета скорости

int MODEprev = 0; // Храним значение предыдущего режима для возвращнеия в него или контроля переключения.

int ind_on = LOW;             // состояние диода
int PowerIndCnt = 0;           //Количество мыргов светового индикатора

unsigned long currentInd = 0;
unsigned long previousInd = 0;

unsigned long pressed_moment1;  // момент нажатия кнопки Режим
Bounce bouncer1 = Bounce();     //создаем экземпляр класса Bounce кнопки Режим

////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------
//             Функция вызываемая внешним прерыванием
//                 (импульс с датчика скорости)
//----------------------------------------------------------------------
void ImpScore()
{
	Imp++; //Увеличиваем счетчик импульсов на 1
}

//----------------------------------------------------------------------
//             Функция замера скорости
// Рассчитываем ActualValuesVO.Speed-текущая скорость и ActualValuesVO.Odo -  пробег в метрах до следующего импульса смазки
//----------------------------------------------------------------------
void Speedometer()
{     //Расчитываем скорость
	if (Stgs.MODE == 1) {
		cur_ms = millis();       //Присвоили текущее время
		mSpeed = cur_ms - prev_ms; //Вычислили разницу текущего с предыдущим (prev_ms)
		if (mSpeed >= IntrS) //Если прошло достаточно времени, то вычисляем дистанцию
		        {
			prev_ms = cur_ms; //Переменной предыдущего значения времени присваевается текущее

			//ActualValuesVO.Speed = SettingsVO.MSignalsOn1;
			ActlVal.Speed = ((prevSpeed * 2 + float(Stgs.MSignalsOn1)) / 3); // усредненное значение от предыдущего.
			prevSpeed = ActlVal.Speed;

			if (ActlVal.Speed != 0) { //Если количество импульсов не равно 0

			}
			ActlVal.Dist = float(mSpeed) / 1000 / 3600 * ActlVal.Speed * 1000
			        * 1000; // пробег за контрольное время в мм
			ActlVal.Odo += ActlVal.Dist; // / 1000;     //считаем общий пробег в метрах, добавляем последний пробег к имеющемуся значению (в мм)

			ActlVal.Dist = 0;               //обнуляем пробег за интервал
			mSpeed = 0;              //обнуляем интервал времени замера скорости
		}
	}
}

//---------------------------------------------------------------------
//           Функция управления насосом в режиме "По пробегу"
//---------------------------------------------------------------------

void WorkPumpM()
{ //Функция управления циклами включения/выключения насоса
	//Для работы функции, должны быть установлены значения ActualValuesVO.Odo и OdoInt
	if (OdoInt > 0) {   //Если переменная интервала смазки имеет значение

		if (ActlVal.PumpStat) {            //Если статус насоса "включен"
			if (Stgs.MODE_MILAGE == 10 || Stgs.MODE_MILAGE == 20) { // смазку проводим только если текущий режим "авто" или "дождь"
				digitalWrite(PumpP, HIGH); //Включаем насос - Начало импульса работы насоса
				ActlVal.PumpStat = true;    //запоминаем состояние насоса
			}
			if ((millis() - TPumpON) > Stgs.DurationImp) //проверяем время работы насоса с момента включения, если пора выключать то:
			        {
				ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"
			}
		}
		if (!ActlVal.PumpStat) {          //Если статус насоса "выключен"
			digitalWrite(PumpP, LOW);       //Выключаем насос
			ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"

			if (ActlVal.gps_DistFromLastOiling > OdoInt ) //проверяем не прошел ли нужный интервал,если прошел то
			        {
				ActlVal.PumpStat = true; //Меняем статус насоса на "включен"
				TPumpON = millis();           //сохраняем время включения насоса
				ActlVal.gps_DistFromLastOiling = 0;               //Запускаем новый интервал
				ActlVal.gps_DistFromLastOiling	= 0;
			}
		}
	}
}

//----------------------------------------------------------------------
//             Функция смазки
//----------------------------------------------------------------------
void Oiling()
{             //Функция управления смазкой в зависимости от режима

	switch (Stgs.MODE)
	{

	//Режим "По пробегу" - Автоматическая смазка по пробегу
	case 1:
		if (MODEprev != Stgs.MODE) {
			MODEprev = Stgs.MODE;
			Web_SendSettingsData();
#ifdef DEBUG
			Serial.printf("\nMODE=1..");
#endif
		}
		ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
		ActlVal.TimeLeft = 0; //запоминаем остаток времени до смазки

		if (Stgs.MODE_MILAGE == 10 || Stgs.MODE_MILAGE == 20) { // смазку проводим только если текущий режим "авто" или "дождь"
			if (ActlVal.Speed > Stgs.MMinSpeed) //Если скорость больше минимальной скорости смазки
			        {
				if (ActlVal.Speed > Stgs.MRoadSpeed) //Если скорость выше городской
				        {
					if (ActlVal.Speed > 150) //Если скорость больше 150 км/ч - отключаем смазку
					        {
						digitalWrite(PumpP, LOW); //Выключаем насос (защита от ошибок)
						ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"
						OdoInt = 100000; //задаем периодичность 100км (защита от ошибок)
					} else       //Если скорость выше городской (едем по трассе)
					{
						OdoInt = Stgs.MIntervalRoad; //задаем периодичность смазки для трассы
						if (Stgs.MODE_MILAGE == 20)
						    OdoInt = OdoInt / ((100 + Stgs.MCoefRain) / 100); //Если активировани режим дождя, то увеличиваем переодичность уменьшая OdoInt на заданный %
						WorkPumpM();      //запускаем функцию управления насосом
					}
				} else            //Если скорость городская
				{
					OdoInt = Stgs.MIntervalTown; //задаем периодичность смазки для города
					if (Stgs.MODE_MILAGE == 20)
					    OdoInt = OdoInt / ((100 + Stgs.MCoefRain) / 100); //Если активировани режим дождя, то увеличиваем переодичность уменьшая OdoInt на заданный %
					WorkPumpM();          //запускаем функцию управления насосом
				}
			} else            //Если скорость меньше минимальной скорости смазки
			{
				digitalWrite(PumpP, LOW);   //Выключаем насос (защита от ошибок)
				ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"
				OdoInt = 100000; //задаем периодичность 100км (защита от ошибок)
			}
			break;
		}
		if (Stgs.MODE_MILAGE == 30) {            //Если выключили
			digitalWrite(PumpP, LOW);       //Выключаем насос (защита от ошибок)
			ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"
			OdoInt = 100000;     //задаем периодичность 100км (защита от ошибок)
		}
		break;

		//Режим "По таймеру"  Принудительная смазка по интервалу времени
	case 2:		// MODE_TIMER
		if (MODEprev != Stgs.MODE) {
			MODEprev = Stgs.MODE;
			timerBetweenImpulses.reset();
			Web_SendSettingsData();
#ifdef DEBUG
			Serial.printf("\nMODE=2..");
#endif
		}
		ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
		timerImpulseDuration.setPeriod(Stgs.DurationImp);// Устанавливаем длительность импульса включения (из настроек)

		if (Stgs.MODE_TIMER == 40) {
			ActlVal.TimeLeft = 0; //запоминаем остаток времени до смазки

			digitalWrite(PumpP, LOW);  //выключаем насос
			ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"
		} else {
			switch (Stgs.MODE_TIMER)
			{ //В зависимости от выбранного интервала текущего режима:
			case 10:
				ActlVal.TimeLeft = Stgs.TIntervalTown
				        - timerBetweenImpulses.getTimeLeft() / 1000; //запоминаем остаток времени до смазки
				timerBetweenImpulses.setPeriod(Stgs.TIntervalTown * 1000);
				break;
			case 20:
				ActlVal.TimeLeft = Stgs.TIntervalRoad
				        - timerBetweenImpulses.getTimeLeft() / 1000; //запоминаем остаток времени до смазки
				timerBetweenImpulses.setPeriod(Stgs.TIntervalRoad * 1000);
				break;
			case 30:
				ActlVal.TimeLeft = Stgs.TIntervalRain
				        - timerBetweenImpulses.getTimeLeft() / 1000; //запоминаем остаток времени до смазки
				timerBetweenImpulses.setPeriod(Stgs.TIntervalRain * 1000);
				break;
			case 40:
				break;
			default:
				break;
			}

			if (timerBetweenImpulses.ready()) { //проверяем не прошел ли нужный интервал, если прошел то начинаем новый импульс
				Pprev_ms = Pcur_ms;          // сохраняем время начала импульса
				digitalWrite(PumpP, HIGH);   //Включаем насос
#ifdef DEBUG
				if (ActlVal.PumpStat == false)
					Serial.println("ActualValuesVO.PumpStat = true;  ");
#endif
				ActlVal.PumpStat = true;    //запоминаем состояние насоса
				timerImpulseDuration.reset();

			} else {                        //если импульс еще не окончился, то
				if (timerImpulseDuration.ready()) { //проверяем, достаточное ли время отработал насос
					digitalWrite(PumpP, LOW); //если достаточно - выключаем насос

#ifdef DEBUG
					if (ActlVal.PumpStat == true)
						Serial.println("ActualValuesVO.PumpStat = false;  ");
#endif
					ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"

				}
			}
		}
//		case 10:                         //10-TOWN
//			Pcur_ms = millis();            //Запомнили текущее время
//
//			ActualValuesVO.TimeLeft = (SettingsVO.TIntervalTown
//					- (Pcur_ms - Pprev_ms) / 1000); //запоминаем остаток времени до смазки
//
//			if ((Pcur_ms - Pprev_ms) > (SettingsVO.TIntervalTown * 1000)) { //проверяем не прошел ли нужный интервал, если прошел то начинаем новый импульс
//				Pprev_ms = Pcur_ms;          // сохраняем время начала импульса
//				digitalWrite(PumpP, HIGH);   //Включаем насос
//				ActualValuesVO.PumpStat = true;    //запоминаем состояние насоса
//			} else {                        //если импульс еще не окончился, то
//				if (Pcur_ms - Pprev_ms > SettingsVO.DurationImp) { //проверяем, достаточное ли время отработал насос
//					digitalWrite(PumpP, LOW); //если достаточно - выключаем насос
//					ActualValuesVO.PumpStat = false; //Переводим статус насоса в "выключен"
//				}
//			}
//			break;
//		case 20:         //20-ROAD
//			Pcur_ms = millis(); //Запомнили текущее время
//
//			ActualValuesVO.TimeLeft = (SettingsVO.TIntervalRoad
//					- (Pcur_ms - Pprev_ms) / 1000); //запоминаем остаток времени до смазки
//
//			if (Pcur_ms - Pprev_ms > (SettingsVO.TIntervalRoad * 1000)) { //проверяем не прошел ли нужный интервал, если прошел то начинаем новый импульс
//				Pprev_ms = Pcur_ms;     // сохраняем время начала импульса
//				digitalWrite(PumpP, HIGH);            //Включаем насос
//				ActualValuesVO.PumpStat = true;    //запоминаем состояние насоса
//			} else { //если импульс еще не окончился, то
//				if (Pcur_ms - Pprev_ms > SettingsVO.DurationImp) { //проверяем, достаточное ли время отработал насос
//					digitalWrite(PumpP, LOW); //если достаточно - выключаем насос
//					ActualValuesVO.PumpStat = false; //Переводим статус насоса в "выключен"
//				}
//			}
//
//			break;
//		case 30:         //30-RAIN
//			Pcur_ms = millis(); //Запомнили текущее время
//
//			ActualValuesVO.TimeLeft = (SettingsVO.TIntervalRain
//					- (Pcur_ms - Pprev_ms) / 1000); //запоминаем остаток времени до смазки
//
//			if ((Pcur_ms - Pprev_ms) > (SettingsVO.TIntervalRain * 1000)) { //проверяем не прошел ли нужный интервал, если прошел то начинаем новый импульс
//				Pprev_ms = Pcur_ms;     // сохраняем время начала импульса
//				digitalWrite(PumpP, HIGH);            //Включаем насос
//				ActualValuesVO.PumpStat = true;    //запоминаем состояние насоса
//
//				Serial.println("ActualValuesVO.PumpStat = true;  ");
//			} else { //если импульс еще не окончился, то
//				if (Pcur_ms - Pprev_ms > SettingsVO.DurationImp) { //проверяем, достаточное ли время отработал насос
//					digitalWrite(PumpP, LOW); //если достаточно - выключаем насос
//					ActualValuesVO.PumpStat = false; //Переводим статус насоса в "выключен"
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

		//Режим "PUMP"  Принудительная подача заданного количества импульсов
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

		ActlVal.TimeLeft = timerBetweenImpulses.getTimeLeft() / 1000; //запоминаем остаток времени до смазки
		timerImpulseDuration.setPeriod(Stgs.DurationImp);// Устанавливаем длительность импульса включения (из настроек)
		timerBetweenImpulses.setPeriod(Stgs.PImpInterval);

		if (ActlVal.PUMP_count > 0) {
			if (timerBetweenImpulses.ready()) { //проверяем не прошел ли нужный интервал, если прошел то начинаем новый импульс
				digitalWrite(PumpP, HIGH);               	// Включаем насос
				ActlVal.PumpStat = true;    //запоминаем состояние насоса
				timerImpulseDuration.reset();

#ifdef DEBUG
				if (ActlVal.PumpStat == false)
					Serial.println("ActualValuesVO.PumpStat = true;  ");
#endif

			} else { 						//если импульс еще не окончился, то
				if (timerImpulseDuration.ready()) { //проверяем, достаточное ли время отработал насос
					if (ActlVal.PumpStat == true) {
						digitalWrite(PumpP, LOW); //если достаточно - выключаем насос

						ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"
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
			// Когда пркоачка закончилась - переводим в предыдущий режим? или выключаем?
			Serial.printf(
			        "\nMODE=3.. PUMP_count=0. So pump finished. Set MODE = OFF");
			Stgs.MODE = 4;
		}
		break;

		//Режим "OFF" - Выключение всех режимов смазки
	case 4:
		if (MODEprev != Stgs.MODE) {
			MODEprev = Stgs.MODE;
			Web_SendSettingsData();
#ifdef DEBUG
			Serial.printf("\nMODE=4..");
#endif
		}
//		Serial.printf("\nMODE=4..");
		ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
		ActlVal.TimeLeft = 0; //запоминаем остаток времени до смазки
		digitalWrite(PumpP, LOW);  //выключаем насос (защита от ошибок)
		ActlVal.PumpStat = false;  //Переводим статус насоса в "выключен"
		break;
	}
	MODEprev = Stgs.MODE;
}

//----------------------------------------------------------------------
//             Функция индикации текущего режима работы
//----------------------------------------------------------------------
//для индикации используется RGB светодиод с общим анодом (общий "+")
//для замены на диод с общим катодом, заменить все LOW на HIGH и наоборот HIGH на LOW.
void Indicator()
{
	/* if (ActualValuesVO.PumpStat == true) {     //если насос включен, включаем красный диод не зависимо от текущего режима
	 digitalWrite(DR, LOW);    //включаем красный
	 digitalWrite(DB, HIGH);   //выключаем синий
	 digitalWrite(DG, HIGH);   //выключаем зеленый
	 }
	 else
	 {
	 switch (SettingsVO.MODE) {

	 case 1:                             //индикация режима "по пробегу"
	 switch (SettingsVO.MODE_MILAGE) {
	 case 10:                        //"Авто" - мигание зеленого диода - длинные интервалы (0,8 сек)
	 digitalWrite(DR, HIGH);       //выключаем красный
	 digitalWrite(DB, HIGH);       //выключаем синий
	 currentInd = millis();        //запоминаем текущее время
	 if (currentInd - previousInd > 800) {   //если прошло 0,8сек (интервал мигания)
	 previousInd = currentInd  ; //устанавливаем предыдущее значение времени
	 if (ind_on == LOW)          //если диод горел
	 ind_on = HIGH;            //устанавливаем состояние диода в включен
	 else
	 ind_on = LOW;             //иначе устанавливаем состояние диода в выключен
	 digitalWrite(DG, ind_on);   //управляем диодом
	 }

	 break;
	 case 20:                        //"Дождь" - мигание зеленого диода - короткие интервалы (0,4 сек)
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
	 case 30:                        //"Выкл" - постоянно горит красный диод
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, HIGH);
	 break;
	 }
	 break;
	 case 2:                               //индикация режима "по таймеру"
	 switch (SettingsVO.MODE_TIMER) {               //В зависимости от выбранного интервала текущего режима:
	 case 10:                         //"Город" - мигание Синего диода - длинные интервалы (1 сек)
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
	 case 20:                         //"Трасса" - мигание Синего диода - средние интервалы (0,6 сек)
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
	 case 30:                         //"Дождь" - мигание Синего диода - короткие интервалы (0,3 сек)
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
	 case 40:                        //"Выкл" - постоянно горит красный диод
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, HIGH);
	 break;
	 }
	 break;
	 case 3:                              //индикация режима "Прокачка" - горят красный и зеленый одновременно
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, LOW);
	 break;

	 case 4:                              //"Выкл" - постоянно горит красный диод
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, HIGH);
	 break;
	 }
	 if (ActualValuesVO.PumpStat == true) {     //если насос включен, включаем красный диод не зависимо от текущего режима
	 digitalWrite(DR, LOW);
	 digitalWrite(DB, HIGH);
	 digitalWrite(DG, HIGH);
	 }
	 }
	 */

	int timeoff = 1000;    // Длительность состояния диода индикации "выключено"
	switch (Stgs.MODE)
	{
	case 1:                        //По пробегу
		currentInd = millis();        //запоминаем текущее время
		if (ActlVal.PowerIndStat) {
			if (currentInd - previousInd > 500) { //если прошло 0,5сек (интервал мигания)
				previousInd = currentInd;
				digitalWrite(PowerInd, LOW);    //включаем
				ActlVal.PowerIndStat = false;
			}
		} else {
			if (currentInd - previousInd > timeoff) { //если прошло 0,5сек (интервал мигания)
				previousInd = currentInd;
				digitalWrite(PowerInd, HIGH);    //включаем
				ActlVal.PowerIndStat = true;
			}
		}
		PowerIndCnt = 0;

		break;
	case 2:                        // по Таймеру
		currentInd = millis();
		if (ActlVal.PowerIndStat) {
			if (currentInd - previousInd > 500) { //если прошло 0,5сек (интервал мигания)
				previousInd = currentInd;
				digitalWrite(PowerInd, LOW);    //включаем
				ActlVal.PowerIndStat = false;
				if (PowerIndCnt < 3) {
					timeoff = 300; // 2 мырга
					PowerIndCnt++;
				} else {
					timeoff = 1000;
					PowerIndCnt = 0;
				}
			}
		} else {
			if (currentInd - previousInd > timeoff) { //если прошло 0,5сек (интервал мигания)
				previousInd = currentInd;
				digitalWrite(PowerInd, HIGH);    //включаем
				ActlVal.PowerIndStat = true;
			}
		}
		break;
	case 3:                        //"Выкл" -
		currentInd = millis();
		if (ActlVal.PowerIndStat) {
			if (currentInd - previousInd > 500) { //если прошло 0,5сек (интервал мигания)
				previousInd = currentInd;
				digitalWrite(PowerInd, LOW);    //включаем
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
			if (currentInd - previousInd > timeoff) { //если прошло 0,5сек (интервал мигания)
				previousInd = currentInd;
				digitalWrite(PowerInd, HIGH);    //включаем
				ActlVal.PowerIndStat = true;
			}
		}
		break;
	}
}

//----------------------------------------------------------------------
//             Функция обрабатывающая команды нажатий кнопки
//----------------------------------------------------------------------
void serialHandler()
{
	char inChar;
	if (bouncer1.update()) //если произошло событие, кнопка Режим изменила свое состояние
	{
		if (bouncer1.read() == 0)
			pressed_moment1 = millis(); //Если кнопка Режим нажата,запоминаем время нажатия
		else //Если кнопка была отпущена
		{
			if ((millis() - pressed_moment1) < pressed_long) // если кнопка была нажата кратковременно - меняем режим:
			{
				switch (Stgs.MODE)
				{ //Переключаем интервал текущего режима
				case 1:             //1-MILAGE
					Stgs.MODE_MILAGE = Stgs.MODE_MILAGE + 10; //переключаем текущий интервал режима MILAGE
					events.send(Stgs.getSettingsJSON_String().c_str(),
					        "new_settings", millis());
					if (Stgs.MODE_MILAGE > 30) Stgs.MODE_MILAGE = 10; //перебор по кругу

					ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
					break;
				case 2:             //2-TIMER
					Stgs.MODE_TIMER = Stgs.MODE_TIMER + 10; //переключаем текущий интервал режима TIMER
					events.send(Stgs.getSettingsJSON_String().c_str(),
					        "new_settings", millis());
					if (Stgs.MODE_TIMER > 40) Stgs.MODE_TIMER = 10; //перебор по кругу

					ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
					break;
				case 3:             //3-PUMP
					if (ActlVal.PUMP_count > 0) {
						ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
						digitalWrite(PumpP, LOW);  // выключаем насос
						ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"
					} else ActlVal.PUMP_count = Stgs.PNumberImp; //запускаем цикл прокачки
					break;
				case 4:      //4-OFF в режиме OFF ни как не реагируем на нажатие
					ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
					break;
				default:
					break;
				}
			} else //удержание
			{
				//ДЕЙСТВИЕ НА УДЕРЖАНИЕ КНОПКИ
				Stgs.MODE = Stgs.MODE + 1; //переключаем  режим
				events.send(Stgs.getSettingsJSON_String().c_str(),
				        "new_settings", millis());
				if (Stgs.MODE > 4) Stgs.MODE = 1; //перебор режимов по кругу
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
