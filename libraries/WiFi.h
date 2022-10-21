/*
 * WiFi.h
 *
 *  Created on: 3 июл. 2022 г.
 *      Author: vector
 */

#ifndef WIFI_H_
#define WIFI_H_

//#define DEBUG 1               // Флаг отладочных сообщений

#pragma once
#include <ESP8266WiFi.h>
#include <IPAddress.h>
#include "ActualValuesVOiler.h"

// Replace with your network credentials
const char *ssid = "VOilWiFi";
const char *password = "89268302305";

IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//----------------------------------------------------------------------
//             Initialize WiFi
//----------------------------------------------------------------------
void initWiFi() {
#ifdef DEBUG
	Serial.print("Setting soft-AP configuration ... ");
	Serial.println(
			WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

	Serial.print("Setting soft-AP ... ");
	Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
	//WiFi.softAP(ssid);
	//WiFi.softAP(ssid, password, channel, hidden, max_connection)

	Serial.print("Soft-AP IP address = ");
	Serial.println(WiFi.softAPIP());
#else
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);
  WiFi.softAPIP();
#endif
}

#endif /* WIFI_H_ */
