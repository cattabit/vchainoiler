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
#include "./DNSServer.h"

// WiFi Network parameters
const char *ssid = "VOilWiFi";
const char *password = "89268302305";

const byte DNS_PORT = 53;          // Capture DNS requests on port 53

IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

DNSServer dnsServer;              // Create the DNS object

//----------------------------------------------------------------------
//             Initialize WiFi
//----------------------------------------------------------------------
void initWiFi() {
#ifdef DEBUG
	Logger_printad("WiFi", "Setting soft-AP configuration ... ");
	Logger_println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

	Logger_printad("WiFi", "Setting soft-AP ... ");
	Logger_println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

	Logger_printad("WiFi", "Soft-AP IP address = ");
	//WiFi.softAPIP();
	Logger_println(WiFi.softAPIP().toString());
#else
	WiFi.softAPConfig(local_IP, gateway, subnet);
	WiFi.softAP(ssid, password);

	// if DNSServer is started with "*" for domain name, it will reply with
	// provided IP to all DNS request
	dnsServer.start(DNS_PORT, "*", local_IP);
#endif
}

void dnsServerLoop() {
	dnsServer.processNextRequest();
}

#endif /* WIFI_H_ */
