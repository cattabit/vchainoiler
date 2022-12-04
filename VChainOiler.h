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

#include "libraries/Logger.h"

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
int timeoff = 1000;    		// Длительность состояния диода индикации "выключено"
Timer timerPowerInd(1000); // Таймер для индикации

unsigned long currentInd = 0;
unsigned long previousInd = 0;

unsigned long pressed_moment1;  // момент нажатия кнопки Режим
Bounce bouncer1 = Bounce();     //создаем экземпляр класса Bounce кнопки Режим

////////////////////////////////////////////////////////////////////////////////////////////////

void setPumpOff() {
	ActlVal.PumpStat = false;
}

void setPumpImp() {
	ActlVal.PumpStat = true;
	timerImpulseDuration.reset();
	ActlVal.gps_DistFromLastOiling = 0.0;
}

void pumpProcessLoop() {
	if (ActlVal.PumpStat)
		digitalWrite(PumpP, HIGH);
	else
		digitalWrite(PumpP, LOW);

	if (timerImpulseDuration.ready())
		ActlVal.PumpStat = false;
}

//----------------------------------------------------------------------
//             Функция вызываемая внешним прерыванием
//                 (импульс с датчика скорости)
//----------------------------------------------------------------------
//void ImpScore() {
//	Imp++; //Увеличиваем счетчик импульсов на 1
//}

//----------------------------------------------------------------------
//             Функция замера скорости
// Рассчитываем ActualValuesVO.Speed-текущая скорость и ActualValuesVO.Odo -  пробег в метрах до следующего импульса смазки
//----------------------------------------------------------------------
//void Odometer() {     //Расчитываем скорость
//	if (Stgs.MODE == 1) {
//		cur_ms = millis();       //Присвоили текущее время
//		mSpeed = cur_ms - prev_ms; //Вычислили разницу текущего с предыдущим (prev_ms)
//
//		// TODO: Надо поменять, не котролить тупо время - а вызывать функцию при обновлении данных в GPS!!!
//
//		if (mSpeed >= IntrS) { //Если прошло достаточно времени, то вычисляем дистанцию
//			prev_ms = cur_ms; //Переменной предыдущего значения времени присваевается текущее
//
//			//ActualValuesVO.Speed = SettingsVO.MSignalsOn1;
//			ActlVal.Speed = ((prevSpeed * 2 + float(Stgs.MSignalsOn1)) / 3); // усредненное значение от предыдущего.
//			prevSpeed = ActlVal.Speed;
//
//			if (ActlVal.Speed != 0) { //Если количество импульсов не равно 0
//
//			}
//
//			ActlVal.Dist = float(mSpeed) / 1000 / 3600 * ActlVal.Speed * 1000
//					* 1000; // пробег за контрольное время в мм
//			ActlVal.Odo += ActlVal.Dist; // / 1000;     //считаем общий пробег в метрах, добавляем последний пробег к имеющемуся значению (в мм)
//
//			ActlVal.Dist = 0;               //обнуляем пробег за интервал
//			mSpeed = 0;              //обнуляем интервал времени замера скорости
//		}
//	}
//}

//----------------------------------------------------------------------
//             Функция смазки
//----------------------------------------------------------------------
void oilingCalc() {             //Функция управления смазкой в зависимости от режима
	switch (Stgs.MODE) {
	case 1: //Режим "По пробегу" - Автоматическая смазка по пробегу
		if (MODEprev != Stgs.MODE) {
			MODEprev = Stgs.MODE;
			Web_SendSettingsData();
			ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
			Logger_printad("Oiler", "MODE=1..");
		}
		//ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку по таймеру
		ActlVal.TimeLeft = 0; //запоминаем остаток времени до смазки

		if (Stgs.MODE_MILAGE == 10 || Stgs.MODE_MILAGE == 20) { // смазку проводим только если текущий режим "авто" или "дождь"
		// Определение периода смазки - OdoInt
			if (ActlVal.gps_speed > Stgs.MRoadSpeed) { 		//Если скорость выше городской
				if (ActlVal.gps_speed > 150) { 				//Если скорость больше 150 км/ч - отключаем смазку
					setPumpOff();
					OdoInt = 100000; 					//задаем периодичность 100км (защита от ошибок)
				}
				else { 									//Если скорость выше городской (едем по трассе)
					OdoInt = Stgs.MIntervalRoad; //задаем периодичность смазки для трассы
					if (Stgs.MODE_MILAGE == 20)	// Если режим дождя
						OdoInt = OdoInt / ((100 + Stgs.MCoefRain) / 100); //Если активирован режим дождя, то увеличиваем переодичность уменьшая OdoInt на заданный %
				}
			}
			else { //Если скорость городская
				OdoInt = Stgs.MIntervalTown; //задаем периодичность смазки для города
				if (Stgs.MODE_MILAGE == 20)
					OdoInt = OdoInt / ((100 + Stgs.MCoefRain) / 100); //Если активировани режим дождя, то увеличиваем переодичность уменьшая OdoInt на заданный %
			}
			//запускаем функцию управления насосом
			if (ActlVal.gps_speed > Stgs.MMinSpeed 	//Если скорость больше минимальной скорости смазки
			&& ActlVal.gps_DistFromLastOiling > OdoInt) {		// сравниваем растояние и запускаем импульс смазки
				setPumpImp();
				ActlVal.PUMP_count++;
			}
			else { //Если скорость меньше минимальной скорости смазки
				setPumpOff();
				OdoInt = 100000; //задаем периодичность 100км (защита от ошибок)
			}
			break;
		}
		if (Stgs.MODE_MILAGE == 30) {            //Если выключили
			setPumpOff();
			OdoInt = 100000;     //задаем периодичность 100км (защита от ошибок)
		}
		break;

	case 2:	// MODE_TIMER //Режим "По таймеру"  Принудительная смазка по интервалу времени
		if (MODEprev != Stgs.MODE) {
			MODEprev = Stgs.MODE;
			timerBetweenImpulses.reset();
			Web_SendSettingsData();
			ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
			Logger_printad("Oiler", "MODE=2..");
		}

		timerImpulseDuration.setPeriod(Stgs.DurationImp); // Устанавливаем длительность импульса включения (из настроек)

		if (Stgs.MODE_TIMER == 40) {
			ActlVal.TimeLeft = 0; //запоминаем остаток времени до смазки

			setPumpOff();
		}
		else {
			switch (Stgs.MODE_TIMER) { //В зависимости от выбранного интервала текущего режима:
			case 10:
				ActlVal.TimeLeft = Stgs.TIntervalTown - timerBetweenImpulses.getTimeLeft() / 1000; //запоминаем остаток времени до смазки
				timerBetweenImpulses.setPeriod(Stgs.TIntervalTown * 1000);
				break;
			case 20:
				ActlVal.TimeLeft = Stgs.TIntervalRoad - timerBetweenImpulses.getTimeLeft() / 1000; //запоминаем остаток времени до смазки
				timerBetweenImpulses.setPeriod(Stgs.TIntervalRoad * 1000);
				break;
			case 30:
				ActlVal.TimeLeft = Stgs.TIntervalRain - timerBetweenImpulses.getTimeLeft() / 1000; //запоминаем остаток времени до смазки
				timerBetweenImpulses.setPeriod(Stgs.TIntervalRain * 1000);
				break;
			case 40:
				break;
			default:
				break;
			}

			if (timerBetweenImpulses.ready()) { //проверяем не прошел ли нужный интервал, если прошел то начинаем новый импульс
				Pprev_ms = Pcur_ms;          // сохраняем время начала импульса
				setPumpImp();
				ActlVal.PUMP_count++;
				Logger_printadln("Main", "ActlVal.PumpStat = " + ActlVal.PumpStat ? "true" : "false");
			}
			else {                        //если импульс еще не окончился, то
				if (timerImpulseDuration.ready()) { //проверяем, достаточное ли время отработал насос
					Logger_printadln("Main", "ActlVal.PumpStat = " + ActlVal.PumpStat ? "true" : "false");
					setPumpOff(); //Переводим статус насоса в "выключен"
				}
			}
		}
		break;

	case 3: //Режим "PUMP"  Принудительная подача заданного количества импульсов
		if (MODEprev != Stgs.MODE) {
			ActlVal.PUMP_count = Stgs.PNumberImp;
			timerBetweenImpulses.reset();
			MODEprev = Stgs.MODE;
			Web_SendSettingsData();
			Logger_printad("Oiler", "MODE=3.. PUMP_count=");
			Logger_printadln("Main", String(ActlVal.PUMP_count));
		}

		ActlVal.TimeLeft = timerBetweenImpulses.getTimeLeft() / 1000; //запоминаем остаток времени до смазки
		timerImpulseDuration.setPeriod(Stgs.DurationImp); // Устанавливаем длительность импульса включения (из настроек)
		timerBetweenImpulses.setPeriod(Stgs.PImpInterval);

		if (ActlVal.PUMP_count > 0) {
			if (timerBetweenImpulses.ready()) { //проверяем не прошел ли нужный интервал, если прошел то начинаем новый импульс
				setPumpImp();
				Logger_printadln("Main", "ActlVal.PumpStat = " + ActlVal.PumpStat ? "true" : "false");
			}
			else { 						//если импульс еще не окончился, то
				if (timerImpulseDuration.ready()) { //проверяем, достаточное ли время отработал насос
					if (ActlVal.PumpStat == true) {
						setPumpOff();

						ActlVal.PUMP_count--;

						Logger_printadln("Main", "ActlVal.PumpStat = " + ActlVal.PumpStat ? "true" : "false");
						Logger_printad("Oiler", "MODE=3.. PUMP_count=");
						Logger_printadln("Main", String(ActlVal.PUMP_count));
					}
				}
			}
		}
		else {
			// Когда пркоачка закончилась - переводим в предыдущий режим? или выключаем?
			Logger_printad("Oiler", "\nMODE=3.. PUMP_count=0. So pump finished. Set MODE = OFF");
			Stgs.MODE = 4;
		}
		break;

	case 4: //Режим "OFF" - Выключение всех режимов смазкиF
		if (MODEprev != Stgs.MODE) {
			MODEprev = Stgs.MODE;
			Web_SendSettingsData();
			Logger_printad("Oiler", "MODE=4..");
		}
//		Logger_printad("Oiler", "MODE=4..");
		ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
		ActlVal.TimeLeft = 0; //запоминаем остаток времени до смазки
		setPumpOff();
		break;
	}
	MODEprev = Stgs.MODE;
}

//----------------------------------------------------------------------
//             Функция индикации текущего режима работы
//----------------------------------------------------------------------
//для индикации используется RGB светодиод с общим анодом (общий "+")
//для замены на диод с общим катодом, заменить все LOW на HIGH и наоборот HIGH на LOW.
void indicatorLoop() {
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

}

//----------------------------------------------------------------------
//             Функция индикации текущего режима работы
//----------------------------------------------------------------------
void powerIndLoop() {
	if (PowerIndCnt < Stgs.MODE)
		timeoff = 150; //
	if (Stgs.MODE == 4)
		ActlVal.PowerIndStat = true;

	if (ActlVal.PowerIndStat) {
		timerPowerInd.setPeriod(300);
		digitalWrite(PowerInd, LOW);    //включаем
		if (timerPowerInd.ready()) {
			ActlVal.PowerIndStat = false;
			timerPowerInd.reset();
		}
	}
	else {
		digitalWrite(PowerInd, HIGH);    //включаем
		timerPowerInd.setPeriod(timeoff);
		if (timerPowerInd.ready()) { //(интервал мигания)
			ActlVal.PowerIndStat = true;
			timerPowerInd.reset();

			PowerIndCnt++;
		}

		if (PowerIndCnt == Stgs.MODE)
			timeoff = 1500;
		if (PowerIndCnt > Stgs.MODE)
			PowerIndCnt = 1;
	}
}

//----------------------------------------------------------------------
//             Функция обрабатывающая команды нажатий кнопки
//----------------------------------------------------------------------
void buttonHandler() {
	char inChar;
	if (bouncer1.update()) //если произошло событие, кнопка Режим изменила свое состояние
	{
		if (bouncer1.read() == 0)
			pressed_moment1 = millis(); //Если кнопка Режим нажата,запоминаем время нажатия
		else //Если кнопка была отпущена
		{
			if ((millis() - pressed_moment1) < pressed_long) // если кнопка была нажата кратковременно - меняем режим:
			{
				switch (Stgs.MODE) { //Переключаем интервал текущего режима
				case 1:             //1-MILAGE
					Stgs.MODE_MILAGE = Stgs.MODE_MILAGE + 10; //переключаем текущий интервал режима MILAGE
					events.send(Stgs.getSettingsJSON_String().c_str(), "new_settings", millis());
					if (Stgs.MODE_MILAGE > 30)
						Stgs.MODE_MILAGE = 10; //перебор по кругу

					ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
					break;
				case 2:             //2-TIMER
					Stgs.MODE_TIMER = Stgs.MODE_TIMER + 10; //переключаем текущий интервал режима TIMER
					events.send(Stgs.getSettingsJSON_String().c_str(), "new_settings", millis());
					if (Stgs.MODE_TIMER > 40)
						Stgs.MODE_TIMER = 10; //перебор по кругу

					ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
					break;
				case 3:             //3-PUMP
					if (ActlVal.PUMP_count > 0) {
						ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
						digitalWrite(PumpP, LOW);  // выключаем насос
						ActlVal.PumpStat = false; //Переводим статус насоса в "выключен"
					}
					else
						ActlVal.PUMP_count = Stgs.PNumberImp; //запускаем цикл прокачки
					break;
				case 4:      //4-OFF в режиме OFF ни как не реагируем на нажатие
					ActlVal.PUMP_count = 0; //Обнуляем оставшееся количество импульсов прокачки, останавливая прокачку
					break;
				default:
					break;
				}
			}
			else //удержание
			{
				//ДЕЙСТВИЕ НА УДЕРЖАНИЕ КНОПКИ
				Stgs.MODE = Stgs.MODE + 1; //переключаем  режим
				events.send(Stgs.getSettingsJSON_String().c_str(), "new_settings", millis());
				if (Stgs.MODE > 4)
					Stgs.MODE = 1; //перебор режимов по кругу
			}
		}
	}
}

// WebServer additional handlers

//----------------------------------------------------------------------
//             Initialize LittleFS
//----------------------------------------------------------------------
void initFS() {
#ifdef DEBUG
	Logger_printadln("Main",
			LittleFS.begin() ?
					"LittleFS mounted successfully" :
					"An error has occurred while mounting LittleFS!");
#else
	LittleFS.begin();
#endif
}

//Do not add code below this line
#endif /* _Template8266_H_ */
