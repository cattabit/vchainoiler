//#define DEBUG 1               // Флаг отладочных сообщений

// Do not remove the include below
#include "VChainOiler.h"

#include "libraries/SettingsVOoiler.h"

//----------------------------------------------------------------------
//             Функция настроек при включении
//----------------------------------------------------------------------
void setup() {
#ifdef DEBUG
	Serial.begin(115200);                 // инициализация порта
//	Serial.begin(4800);                 // инициализация порта
	//delay(500);
	//Serial.setTimeout(200);
#endif

	delay (2000);
	Logger_printadln("Main", "");
	Logger_printadln("Main", "//----------------------------------------------------------------------");
	Logger_printadln("Main", "//             VOiler, 2022.");
	Logger_printadln("Main", "//           program starting..");
	Logger_printadln("Main", "//----------------------------------------------------------------------");

	Logger_printadln("Main", "Initialize File System..");
	initFS();

	Logger_printadln("Main", "Load saved settings..");
	Stgs.LoadSettings();             //Initialize saved settings values
//  SettingsVO.MODEprev = 0;


	Logger_printadln("Main", "Initialize WiFi..");
	initWiFi();

	Logger_printadln("Main", "Initialize Web Server..");
	initWebServer();

	Logger_printadln("Main", "Initialize GPS..");
	initGPS();

	// Настройка пинов светового индикатора
	//pinMode(DR, OUTPUT);                 // Установка режима работы пина подключения вывода R светодиода в режим выхода
	//pinMode(DG, OUTPUT);                 // Установка режима работы пина подключения вывода G светодиода в режим выхода
	//pinMode(DB, OUTPUT);                 // Установка режима работы пина подключения вывода B светодиода в режим выхода

	// Настройка пинов кнопки управления
	//  pinMode(BUTTON_PIN_1 , INPUT);      // кнопка на пине BUTTON_PIN_11289657
	//  digitalWrite(BUTTON_PIN_1 , HIGH);  // подключаем встроенный подтягивающий резистор  В этом случае
	// внешний подтягивающий резистор не нужен, через кнопку мы подаем землю.
	//  bouncer1 .attach(BUTTON_PIN_1);     // устанавливаем кнопку
	//  bouncer1 .interval(5);              // устанавливаем параметр stable interval = 5 мс

	// Настройка пина датчика оборота колеса
	//  pinMode(ImpP, INPUT);               // Установка режима работы пина датчика скорости (ImpP) в режим входа
	//  digitalWrite(ImpP , HIGH);          // подключаем встроенный подтягивающий резистор, в качестве ипульса будет земля.

	//attachInterrupt(0, ImpScore, RISING);  //Задаем функцию ImpScore() для внешнего прерывания,
	//срабатывающего при смене сигнала с высокого на низкое на входе 2 (номер 0)
	//при подключении сигналов к пину 3, номер прерывания 0 - заменить на 1

	// Нстройка пина управления насосом
	pinMode(PumpP, OUTPUT);   // Установка режима работы пина управления насосом
	//digitalWrite(PumpP, LOW);
	pinMode(PowerInd, OUTPUT); // Установка режима работы пина управления насосом

	Logger_printad("Main", "Setup() finished.\n");
}

//----------------------------------------------------------------------
//
//             Функция основного цикла
//
//----------------------------------------------------------------------
void loop() {
	Logger_printad("Main", "Main Loop() start.\n");
	buttonHandler();              //вызов функции обрабатывающей команды нажатий
//	Odometer();  //В режиме MILAGE вызывается функция замера текущей скорости

	dnsServerLoop();				// Обработка DNS запросов
	WebEvents();                    // Обработка событий для веб-сервера
	GPSMainLoop();					// Обновление данных GPS

	//Для теста:
//	ActlVal.gps_DistFromLastOiling += 0.4;
//	ActlVal.gps_speed = 24.5;
	oilingCalc();                   // Вызов функции смазки

	indicatorLoop();                    //Вызов функции индикатора текущего режима
	powerIndLoop();					//Вызов функции индикатора текущего режима
	pumpProcessLoop();				// Функция управления импульсами насоса.
	//Logger_printad("Main", "Main Loop() finish.\n");


}
