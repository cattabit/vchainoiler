#define DEBUG 1               // ���� ���������� ���������

// Do not remove the include below
#include "VChainOiler.h"

#include "libraries/SettingsVOoiler.h"

//----------------------------------------------------------------------
//             ������� �������� ��� ���������
//----------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);                 // ������������� �����
//	Serial.begin(4800);                 // ������������� �����
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

#ifdef DEBUG
  Serial.printf("\nSetup() finished.\n");
#endif
}

//----------------------------------------------------------------------
//
//             ������� ��������� �����
//
//----------------------------------------------------------------------
void loop()
{
	serialHandler();              //����� ������� �������������� ������� �������
//	Speedometer();  //� ������ MILAGE ���������� ������� ������ ������� ��������
	Oiling();                       //����� ������� ������
	Indicator();                    //����� ������� ���������� �������� ������
	WebEvents();                    // ��������� ������� ��� ���-�������
	GPSMainLoop();
//  delay(5);                      //����� 10��
}
