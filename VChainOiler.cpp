//#define DEBUG 1               // ���� ���������� ���������

// Do not remove the include below
#include "VChainOiler.h"

#include "libraries/SettingsVOoiler.h"

//----------------------------------------------------------------------
//             ������� �������� ��� ���������
//----------------------------------------------------------------------
void setup() {
#ifdef DEBUG
	Serial.begin(115200);                 // ������������� �����
//	Serial.begin(4800);                 // ������������� �����
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

	// ��������� ����� ��������� ����������
	//pinMode(DR, OUTPUT);                 // ��������� ������ ������ ���� ����������� ������ R ���������� � ����� ������
	//pinMode(DG, OUTPUT);                 // ��������� ������ ������ ���� ����������� ������ G ���������� � ����� ������
	//pinMode(DB, OUTPUT);                 // ��������� ������ ������ ���� ����������� ������ B ���������� � ����� ������

	// ��������� ����� ������ ����������
	//  pinMode(BUTTON_PIN_1 , INPUT);      // ������ �� ���� BUTTON_PIN_11289657
	//  digitalWrite(BUTTON_PIN_1 , HIGH);  // ���������� ���������� ������������� ��������  � ���� ������
	// ������� ������������� �������� �� �����, ����� ������ �� ������ �����.
	//  bouncer1 .attach(BUTTON_PIN_1);     // ������������� ������
	//  bouncer1 .interval(5);              // ������������� �������� stable interval = 5 ��

	// ��������� ���� ������� ������� ������
	//  pinMode(ImpP, INPUT);               // ��������� ������ ������ ���� ������� �������� (ImpP) � ����� �����
	//  digitalWrite(ImpP , HIGH);          // ���������� ���������� ������������� ��������, � �������� ������� ����� �����.

	//attachInterrupt(0, ImpScore, RISING);  //������ ������� ImpScore() ��� �������� ����������,
	//�������������� ��� ����� ������� � �������� �� ������ �� ����� 2 (����� 0)
	//��� ����������� �������� � ���� 3, ����� ���������� 0 - �������� �� 1

	// �������� ���� ���������� �������
	pinMode(PumpP, OUTPUT);   // ��������� ������ ������ ���� ���������� �������
	//digitalWrite(PumpP, LOW);
	pinMode(PowerInd, OUTPUT); // ��������� ������ ������ ���� ���������� �������

	Logger_printad("Main", "Setup() finished.\n");
}

//----------------------------------------------------------------------
//
//             ������� ��������� �����
//
//----------------------------------------------------------------------
void loop() {
	Logger_printad("Main", "Main Loop() start.\n");
	buttonHandler();              //����� ������� �������������� ������� �������
//	Odometer();  //� ������ MILAGE ���������� ������� ������ ������� ��������

	dnsServerLoop();				// ��������� DNS ��������
	WebEvents();                    // ��������� ������� ��� ���-�������
	GPSMainLoop();					// ���������� ������ GPS

	//��� �����:
//	ActlVal.gps_DistFromLastOiling += 0.4;
//	ActlVal.gps_speed = 24.5;
	oilingCalc();                   // ����� ������� ������

	indicatorLoop();                    //����� ������� ���������� �������� ������
	powerIndLoop();					//����� ������� ���������� �������� ������
	pumpProcessLoop();				// ������� ���������� ���������� ������.
	//Logger_printad("Main", "Main Loop() finish.\n");


}
