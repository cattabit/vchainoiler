#define DEBUG 1               // Флаг отладочных сообщений

// Do not remove the include below
#include "VChainOiler.h"

#include "libraries/SettingsVOoiler.h"

//----------------------------------------------------------------------
//             Функция настроек при включении
//----------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);                 // инициализация порта
//	Serial.begin(4800);                 // инициализация порта
	delay(10);
	Serial.setTimeout(100);
#ifdef DEBUG
  Serial.println("\n");
  Serial.println("//----------------------------------------------------------------------");
  Serial.println("//             VOiler, 2022.");
  Serial.println("//           program starting..");
  Serial.println("//----------------------------------------------------------------------");
#endif

#ifdef DEBUG
  Serial.printf("\nInitialize File System..\n");
#endif
	initFS();

#ifdef DEBUG
  Serial.printf("\nLoad saved settings..\n");
#endif
	Stgs.LoadSettings();             //Initialize saved settings values
//  SettingsVO.MODEprev = 0;

#ifdef DEBUG
  Serial.printf("\nInitialize WiFi..\n");
#endif
	initWiFi();

#ifdef DEBUG
  Serial.printf("\nInitialize Web Server..\n");
#endif
	initWebServer();

#ifdef DEBUG
  Serial.printf("\nInitialize GPS..\n");
#endif
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

#ifdef DEBUG
  Serial.printf("\nSetup() finished.\n");
#endif
}

//----------------------------------------------------------------------
//
//             Функция основного цикла
//
//----------------------------------------------------------------------
void loop()
{
	serialHandler();              //вызов функции обрабатывающей команды нажатий
//	Speedometer();  //В режиме MILAGE вызывается функция замера текущей скорости
	Oiling();                       //Вызов функции смазки
	Indicator();                    //Вызов функции индикатора текущего режима
	WebEvents();                    // Обработка событий для веб-сервера
	GPSMainLoop();
//  delay(5);                      //Спать 10мс
}
