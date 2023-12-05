#include "message.hpp"

Message::Message(){};

Message::Message(int code, string cat, int msg_id, int sender_id, string zone)
{
    message_code = code;
    message_category = cat;
    message_id = msg_id;
    sender_nod_id = sender_id;
    message_zone = zone;
};

Message::Message(int code, string cat, int msg_id, int sender_id, float queuepoint, string zone)
{
    message_code = code;
    message_category = cat;
    message_id = msg_id;
    sender_nod_id = sender_id;
    queue_point = queuepoint;
    message_zone = zone;
};