/*
 * WebServer.h
 *
 *  Created on: 3 июл. 2022 г.
 *      Author: vector
 */

#ifndef WEBSERVER_H_
#define WEBSERVER_H_

//#define DEBUG 1               // Флаг отладочных сообщений

#pragma once
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "SettingsVOoiler.h"
#include "ActualValuesVOiler.h"

#define WebServEventUpdTmr 500 // Период вызова обновления Веб-сервера
#define WebServEventReconnect 2000 // Период вызова обновления Веб-сервера

const char *PARAM_MESSAGE = "message";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Timer variables
Timer timerWebSrvEvents_readings(WebServEventUpdTmr);

void Web_SendSettingsData() {
	events.send(Stgs.getSettingsJSON_String().c_str(), "new_settings",
			millis());

}

void notFound(AsyncWebServerRequest *request) {
#ifdef DEBUG
	Serial.printf("NOT_FOUND: ");
	if (request->method() == HTTP_GET)
		Serial.printf("GET");
	else if (request->method() == HTTP_POST)
		Serial.printf("POST");
	else if (request->method() == HTTP_DELETE)
		Serial.printf("DELETE");
	else if (request->method() == HTTP_PUT)
		Serial.printf("PUT");
	else if (request->method() == HTTP_PATCH)
		Serial.printf("PATCH");
	else if (request->method() == HTTP_HEAD)
		Serial.printf("HEAD");
	else if (request->method() == HTTP_OPTIONS)
		Serial.printf("OPTIONS");
	else
		Serial.printf("UNKNOWN");
	Serial.printf(" http://%s%s\n", request->host().c_str(),
			request->url().c_str());

	if (request->contentLength()) {
		Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
		Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
	}

	int headers = request->headers();
	int i;
	for (i = 0; i < headers; i++) {
		AsyncWebHeader *h = request->getHeader(i);
		Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(),
				h->value().c_str());
	}

	int params = request->params();
	for (i = 0; i < params; i++) {
		AsyncWebParameter *p = request->getParam(i);
		if (p->isFile()) {
			Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(),
					p->value().c_str(), p->size());
		} else if (p->isPost()) {
			Serial.printf("_POST[%s]: %s\n", p->name().c_str(),
					p->value().c_str());
		} else {
			Serial.printf("_GET[%s]: %s\n", p->name().c_str(),
					p->value().c_str());
		}
	}
#endif
	request->send(404, "text/plain", "Not found");
}

//----------------------------------------------------------------------
//             Initialize WebServer
//----------------------------------------------------------------------
void initWebServer() {
	Logger_printadln("WebServer", "initWebServer() started.");

	// Web Server Root URL
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(LittleFS, "/index.html", "text/html");
	});

	server.serveStatic("/", LittleFS, "/");

	// Request for latest values of dinamic parameters
	// Запрос последних значений динамических параметров
	server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request) {
		String json = ActlVal.getSensorReadingsJSON_String();
		request->send(200, "application/json", json);
#ifdef DEBUG
		Logger_printadln("WebServer", "/readings HTTP_GET: " + String(json));
//		Serial.printf("/readings HTTP_GET: ");
//		Serial.println(String(json));
#endif
		json = String();
	});

	// Request for current settings values
	// Запрос текущих значений настроек
	server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		String json = Stgs.getSettingsJSON_String();
		request->send(200, "application/json", json);
#ifdef DEBUG
		Logger_printadln("WebServer", "/settings HTTP_GET: " + String(json));
//		Serial.printf("/settings HTTP_GET: ");
//		Serial.println(String(json));
#endif
		json = String();
	});

	// Send a GET request to <IP>/get?message=<message>
	server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
		String message;
		if (request->hasParam(PARAM_MESSAGE)) {
			message = request->getParam(PARAM_MESSAGE)->value();
		} else {
			message = "No message sent";
		}
#ifdef DEBUG
		Logger_printadln("WebServer", "/get HTTP_GET: " + message);
//		Serial.printf("/get HTTP_GET: ");
//		Serial.println(message);
#endif
		request->send(200, "text/plain", "Hello, GET: " + message);
	});

	// Send a POST request to <IP>/post with a form field message set to <message>
	server.on("/postSettings", HTTP_POST, [](AsyncWebServerRequest *request) {
		/*//List all collected headers
		 int headers = request->headers();
		 int i;
		 for (i = 0; i < headers; i++) {
		 AsyncWebHeader* h = request->getHeader(i);
		 Serial.printf("HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
		 }*/

		String message;
		int param_cnt = request->params();
		for (int i = 0; i < param_cnt; i++) {
			AsyncWebParameter *p = request->getParam(i);
			message += Stgs.setSettingsValue(p->name().c_str(), p->value().c_str());
			message += ", ";
		}
		/*if (request->hasParam(PARAM_MESSAGE, true)) {
		 message = request->getParam(PARAM_MESSAGE, true)->value();
		 } else {
		 message = "No message sent";
		 }*/
#ifdef DEBUG
		Logger_printadln("WebServer", "/postSettings HTTP_POST: " + message);
//		Serial.printf("/postSettings HTTP_POST: ");
//		Serial.println(message);
#endif

		/*//List all parameters
		 int params = request->params();
		 Serial.printf("Params count: ");
		 Serial.println(params);
		 for (int i = 0; i < params; i++) {
		 AsyncWebParameter* p = request->getParam(i);
		 Serial.printf("Param: ", p->name().c_str());
		 Serial.printf("[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
		 if (p->isFile()) { //p->isPost() is also true
		 Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
		 } else if (p->isPost()) {
		 Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
		 } else {
		 Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
		 }
		 }*/

		request->send(200, "text/plain", "Hello, POST: " + message);
		Stgs.SaveSettings();
		Web_SendSettingsData();
	});

	//
	server.on("/settings_reset_req", HTTP_GET,
			[](AsyncWebServerRequest *request) {
				Stgs.ResetSettings();
				Stgs.reboot();
				request->send(200, "text/plain", String("Settings reseted."));
			});

	//
	server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", String(ESP.getFreeHeap()));
	});

	// для остальных запросов отвечаем 404
	server.onNotFound(notFound);

	// Настройка событий
	events.onConnect(
			[](AsyncEventSourceClient *client) {
#ifdef DEBUG
				if (client->lastId()) {
					Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
				}
#endif
				// send event with message "hello!", id current millis
				// and set reconnect delay to 1 second
				client->send("hello!", NULL, millis(), WebServEventReconnect);
				Web_SendSettingsData();
			});
	server.addHandler(&events);

	// Start server
	server.begin();

	Logger_printadln("WebServer", "initWebServer() finished.");
}

//----------------------------------------------------------------------
//             Формирование событий для веб-сервера
//----------------------------------------------------------------------
void WebEvents() {
	if (timerWebSrvEvents_readings.ready()) {
		// Send Events to the client with the Readings
		events.send(ActlVal.getSensorReadingsJSON_String().c_str(),
				"new_readings", millis());
	}
}

#endif /* WEBSERVER_H_ */
