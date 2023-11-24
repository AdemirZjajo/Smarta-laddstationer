#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <stdint.h>


using namespace std;

class COM
{
public:
    COM();

    void sendMessage();

    void changeCS(string zoneCode);

    int getID();

    // void receivedCallback( uint32_t from, STD::string &msg );

    void newConnectionCallback(uint32