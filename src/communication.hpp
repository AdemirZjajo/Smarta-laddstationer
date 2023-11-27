#pragma once
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <stdint.h>
#include <string>

using namespace std;

void initCOM();

void sendMessage();

void changeCS(string zoneCode);

int getID();

// void receivedCallback( uint32_t from, STD::string &msg );

void newConnectionCallback(uint32_t nodeId);

void changedConnectionCallback();

#endif