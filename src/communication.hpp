#pragma once
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <stdint.h>
#include <string>
#include <painlessMesh.h>

using namespace std;

void initCOM();

void sendMessage();

void sendQ(int id, float points);
void receive();

void changeCS(string zoneCode);

int getID();

void receivedCallback(uint32_t from, String &msg);

void newConnectionCallback(uint32_t nodeId);

void changedConnectionCallback();
void updateCommunication();

#endif