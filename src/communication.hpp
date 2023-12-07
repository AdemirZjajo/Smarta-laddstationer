#pragma once
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "message.hpp"
#include <stdint.h>
#include <string>
#include <painlessMesh.h>
#include <vector>

using namespace std;

void initCOM();

void sendMessage(Message message);

void addSelfToQueue(Message message);

void eraseSelfFromQueue(Message message);

void sortQueue();

int getID();

void receivedCallback(uint32_t from, String &msg);

void newConnectionCallback(uint32_t nodeId);

void changedConnectionCallback();

void updateCommunication();

void clearComVector();

vector<vector<float>> getComQueueVector();

void disconnect();

void removeMissingNodes();

void printQueueVector();

#endif