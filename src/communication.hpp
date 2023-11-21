#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <stdint.h>


void sendMessage();

void receivedCallback( uint32_t from, String &msg );

void newConnectionCallback(uint32_t nodeId);

void changedConnectionCallback();


#endif