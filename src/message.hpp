#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

using namespace std;

class Message
{
public:
    int message_code;
    string message_category;
    int message_id;
    int sender_nod_id;
    float queue_point;
    string message_zone;

    Message();
    Message(int code, string cat, int msg_id, int sender_id, string zone);
    Message(int code, string cat, int msg_id, int sender_id, float queuepoint, string zone);
};

#endif