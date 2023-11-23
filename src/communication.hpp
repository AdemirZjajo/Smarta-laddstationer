#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <stdint.h>



void sendMessage();

int getID();

void receivedCallback( uint32_t from, string &msg );

void newConnectionCallback(uint32_t nodeId);

void changedConnectionCallback();

void setupCommunication();

void loopCommunication();

#endif