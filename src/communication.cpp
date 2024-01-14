//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//

/*
1. Test for intializing the mes
2. Test for connection/changed connection
3. Test for sending/broadcasting
4. test for recieving
5. test for disconnecting
6. test for changing network
*/

//************************************************************
#include "communication.hpp"
#include "message.hpp"
#include "mission.hpp"
#include "node.hpp"
#include <painlessMesh.h>
#include <list>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ArduinoWebsockets.h>

using namespace std;

Scheduler userScheduler; // Scheduler to control tasks
painlessMesh mesh;       // instance for our mesh
list<uint32_t> nodeList; // instance for our list
String MESH_PREFIX = "meshOrigin";
String MESH_PASSWORD = "123456789";
int MESH_PORT = 5555;
int Counter = 0;
int nodeId = 0;
String currentZon;

vector<vector<float>> queueVector;
vector<vector<float>> queueVector_CS1;
vector<vector<float>> queueVector_CS2;
vector<vector<float>> queueVector_CS3;
vector<vector<float>> queueVector_CS4;

struct MessageStruct
{
  String category;
  int nodeID;
  String cs;
  float qp;
  int msgID;
};

vector<float> tempVect;
bool exists;

void sendMessage(); // Prototype so PlatformIO doesn't complain

// Create the task
// Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

// Clear the list
/*void clearComVector()
{
  queueVector.clear();
}*/

// Creating the message and broadcast it here
void sendMessage(Message message)
{
  String msg = String(message.message_category.c_str());
  msg += "-";
  msg += message.sender_nod_id;
  msg += "-";
  msg += String(message.message_zone.c_str());
  msg += "-";
  msg += message.queue_point;
  msg += "-";
  msg += message.message_id;
  mesh.sendBroadcast(msg);
  cout << "Nod-" << message.sender_nod_id << " skickade: " << msg.c_str() << endl;
}

void addSelfToQueue(Message message)
{
  // Lägger till sig själv i vektorn
  tempVect = {static_cast<float>(message.sender_nod_id), static_cast<float>(message.queue_point)};
  exists = false;

  if (message.message_zone == "LADDSTATION_1")
  {
    for (auto it = queueVector_CS1.begin(); it != queueVector_CS1.end(); ++it)
    {
      if ((*it)[0] == tempVect[0])
      {
        exists = true;
        (*it)[0] = tempVect[0];
        (*it)[1] = tempVect[1];
        break;
      }
    }

    if (!exists)
    {
      queueVector_CS1.push_back(tempVect);
      cout << "Nod-" << message.sender_nod_id << " lägger till sig själv i kölistan för: " << message.message_zone << endl;
    }
  }

  else if (message.message_zone == "LADDSTATION_2")
  {
    for (auto it = queueVector_CS2.begin(); it != queueVector_CS2.end(); ++it)
    {
      if ((*it)[0] == tempVect[0])
      {
        exists = true;
        (*it)[0] = tempVect[0];
        (*it)[1] = tempVect[1];
        break;
      }
    }

    if (!exists)
    {
      queueVector_CS2.push_back(tempVect);
      cout << "Nod-" << message.sender_nod_id << " lägger till sig själv i kölistan för: " << message.message_zone << endl;
    }
  }

  else if (message.message_zone == "LADDSTATION_3")
  {
    for (auto it = queueVector_CS3.begin(); it != queueVector_CS3.end(); ++it)
    {
      if ((*it)[0] == tempVect[0])
      {
        exists = true;
        (*it)[0] = tempVect[0];
        (*it)[1] = tempVect[1];
        break;
      }
    }

    if (!exists)
    {
      queueVector_CS3.push_back(tempVect);
      cout << "Nod-" << message.sender_nod_id << " lägger till sig själv i kölistan för: " << message.message_zone << endl;
    }
  }

  else if (message.message_zone == "LADDSTATION_4")
  {
    for (auto it = queueVector_CS4.begin(); it != queueVector_CS4.end(); ++it)
    {
      if ((*it)[0] == tempVect[0])
      {
        exists = true;
        (*it)[0] = tempVect[0];
        (*it)[1] = tempVect[1];
        break;
      }
    }

    if (!exists)
    {
      queueVector_CS4.push_back(tempVect);
      cout << "Nod-" << message.sender_nod_id << " lägger till sig själv i kölistan för: " << message.message_zone << endl;
    }
  }
}

void eraseSelfFromQueue(Message message)
{

  for (auto it = queueVector_CS1.begin(); it != queueVector_CS1.end(); ++it)
  {
    if ((*it)[0] == static_cast<float>(message.sender_nod_id))
    {
      queueVector_CS1.erase(it);
      cout << "Nod-" << message.sender_nod_id << " tog bort sig själv från kölistan för: " << message.message_zone << endl;
      break;
    }
  }

  for (auto it = queueVector_CS2.begin(); it != queueVector_CS2.end(); ++it)
  {
    if ((*it)[0] == static_cast<float>(message.sender_nod_id))
    {
      queueVector_CS2.erase(it);
      cout << "Nod-" << message.sender_nod_id << " tog bort sig själv från kölistan för: " << message.message_zone << endl;
      break;
    }
  }

  for (auto it = queueVector_CS3.begin(); it != queueVector_CS3.end(); ++it)
  {
    if ((*it)[0] == static_cast<float>(message.sender_nod_id))
    {
      queueVector_CS3.erase(it);
      cout << "Nod-" << message.sender_nod_id << " tog bort sig själv från kölistan för: " << message.message_zone << endl;
      break;
    }
  }

  for (auto it = queueVector_CS4.begin(); it != queueVector_CS4.end(); ++it)
  {
    if ((*it)[0] == static_cast<float>(message.sender_nod_id))
    {
      queueVector_CS4.erase(it);
      cout << "Nod-" << message.sender_nod_id << " tog bort sig själv från kölistan för: " << message.message_zone << endl;
      break;
    }
  }
}

void sortQueue()
{

  if (!(queueVector_CS1.empty()))
  {
    sort(queueVector_CS1.begin(),
         queueVector_CS1.end(),
         [](const vector<float> &a, const vector<float> &b)
         {
           return a[1] > b[1];
         });
  }
  if (!(queueVector_CS2.empty()))
  {
    sort(queueVector_CS2.begin(),
         queueVector_CS2.end(),
         [](const vector<float> &a, const vector<float> &b)
         {
           return a[1] > b[1];
         });
  }
  if (!(queueVector_CS3.empty()))
  {
    sort(queueVector_CS3.begin(),
         queueVector_CS3.end(),
         [](const vector<float> &a, const vector<float> &b)
         {
           return a[1] > b[1];
         });
  }
  if (!(queueVector_CS4.empty()))
  {
    sort(queueVector_CS4.begin(),
         queueVector_CS4.end(),
         [](const vector<float> &a, const vector<float> &b)
         {
           return a[1] > b[1];
         });
  }
}

MessageStruct parseString(const String &input)
{
  MessageStruct result;

  // Find the positions of "-" in the input string
  int pos1 = input.indexOf('-');
  int pos2 = input.indexOf('-', pos1 + 1);
  int pos3 = input.indexOf('-', pos2 + 1);
  int pos4 = input.indexOf('-', pos3 + 1);
  int pos5 = input.indexOf('-', pos4 + 1);

  String category;
  int nodeID;
  String cs;
  float qp;
  int msgID;

  // Extract substrings based on "-" positions
  result.category = input.substring(0, pos1);
  result.nodeID = input.substring(pos1 + 1, pos2).toInt();
  result.cs = input.substring(pos2 + 1, pos3);
  result.qp = input.substring(pos3 + 1, pos4).toFloat();
  result.msgID = input.substring(pos4 + 1, pos5).toInt();

  return result;
}

// Itererar genom meshnätets anslutna noder lista, om en nod inte finns här men finns i köistan tas den bort
/*void removeMissingNodes()
{
  for (const auto &node : nodeList)
  {
    if (!(mesh.isConnected(node)))
    {
      printf("%u is disconnected\n", node % 1000);

      for (auto it = queueVector.begin(); it != queueVector.end(); ++it)
      {
        if ((*it)[0] == static_cast<float>(node % 1000))
        {
          queueVector.erase(it);
          cout << "Vector removed successfully." << endl;
          break;
        }
        else
        {
          cout << "ERROR: Tried to remove but couldn't" << endl;
        }
      }

      nodeList.remove(node);
    }
  }
}*/

// returns the node id, mainly used in display
int getID()
{
  return mesh.getNodeId() % 1000;
}

// This notifies the ESP when a message is recieved
void receivedCallback(uint32_t from, String &msg)
{
  cout << "Meddelande från Nod-" << from % 1000 << " --> " << msg.c_str() << endl;
  MessageStruct callbackStruct = parseString(msg);

  cout << "Kategori: " << callbackStruct.category.c_str();
  cout << ", Nod-ID: " << callbackStruct.nodeID;
  cout << ", Laddstation: " << callbackStruct.cs.c_str();
  cout << " Köpoäng: " << callbackStruct.qp;
  cout << " Message-ID: " << callbackStruct.msgID << endl;

  if (callbackStruct.category == "AddToQueue")
  {
    tempVect = {static_cast<float>(callbackStruct.nodeID), callbackStruct.qp};

    exists = false;

    if (callbackStruct.cs == "LADDSTATION_1")
    {
      for (auto it = queueVector_CS1.begin(); it != queueVector_CS1.end(); ++it)
      {
        if ((*it)[0] == tempVect[0])
        {
          exists = true;
          (*it)[0] = tempVect[0];
          (*it)[1] = tempVect[1];
        }
      }

      if (!exists)
      {
        queueVector_CS1.push_back(tempVect);
        cout << "Nod-" << from % 1000 << " läggs till i kölista för: " << callbackStruct.cs.c_str() << endl;
      }
    }

    else if (callbackStruct.cs == "LADDSTATION_2")
    {
      for (auto it = queueVector_CS2.begin(); it != queueVector_CS2.end(); ++it)
      {
        if ((*it)[0] == tempVect[0])
        {
          exists = true;
          (*it)[0] = tempVect[0];
          (*it)[1] = tempVect[1];
        }
      }

      if (!exists)
      {
        queueVector_CS2.push_back(tempVect);
        cout << "Nod-" << from % 1000 << " läggs till i kölista för: " << callbackStruct.cs.c_str() << endl;
      }
    }

    else if (callbackStruct.cs == "LADDSTATION_3")
    {
      for (auto it = queueVector_CS3.begin(); it != queueVector_CS3.end(); ++it)
      {
        if ((*it)[0] == tempVect[0])
        {
          exists = true;
          (*it)[0] = tempVect[0];
          (*it)[1] = tempVect[1];
        }
      }

      if (!exists)
      {
        queueVector_CS3.push_back(tempVect);
        cout << "Nod-" << from % 1000 << " läggs till i kölista för: " << callbackStruct.cs.c_str() << endl;
      }
    }
    else if (callbackStruct.cs == "LADDSTATION_4")
    {
      for (auto it = queueVector_CS4.begin(); it != queueVector_CS4.end(); ++it)
      {
        if ((*it)[0] == tempVect[0])
        {
          exists = true;
          (*it)[0] = tempVect[0];
          (*it)[1] = tempVect[1];
        }
      }

      if (!exists)
      {
        queueVector_CS4.push_back(tempVect);
        cout << "Nod-" << from % 1000 << " läggs till i kölista för: " << callbackStruct.cs.c_str() << endl;
      }
    }
  }

  else if (callbackStruct.category == "RemoveFromQueue")
  {
    if (callbackStruct.cs == "LADDSTATION_1")
    {
      for (auto it = queueVector_CS1.begin(); it != queueVector_CS1.end(); ++it)
      {
        if ((*it)[0] == callbackStruct.nodeID)
        {
          queueVector_CS1.erase(it);
          cout << "Nod-" << callbackStruct.nodeID << " blev borttagen från kölista för: " << callbackStruct.cs.c_str() << endl;
          break;
        }
      }
    }

    else if (callbackStruct.cs == "LADDSTATION_2")
    {
      for (auto it = queueVector_CS2.begin(); it != queueVector_CS2.end(); ++it)
      {
        if ((*it)[0] == callbackStruct.nodeID)
        {
          queueVector_CS2.erase(it);
          cout << "Nod-" << callbackStruct.nodeID << " blev borttagen från kölista för: " << callbackStruct.cs.c_str() << endl;
          break;
        }
      }
    }

    if (callbackStruct.cs == "LADDSTATION_3")
    {
      for (auto it = queueVector_CS3.begin(); it != queueVector_CS3.end(); ++it)
      {
        if ((*it)[0] == callbackStruct.nodeID)
        {
          queueVector_CS3.erase(it);
          cout << "Nod-" << callbackStruct.nodeID << " blev borttagen från kölista för: " << callbackStruct.cs.c_str() << endl;
          break;
        }
      }
    }

    if (callbackStruct.cs == "LADDSTATION_4")
    {
      for (auto it = queueVector_CS4.begin(); it != queueVector_CS4.end(); ++it)
      {
        if ((*it)[0] == callbackStruct.nodeID)
        {
          queueVector_CS4.erase(it);
          cout << "Nod-" << callbackStruct.nodeID << " blev borttagen från kölista för: " << callbackStruct.cs.c_str() << endl;
          break;
        }
      }
    }
  }

  sortQueue();
}

void updateCommunication()
{
  // it will run the user scheduler as well
  mesh.update();
}

vector<vector<float>> getComQueueVector()
{
  return queueVector;
}

vector<vector<float>> getComQueueVectorCS1()
{
  return queueVector_CS1;
}

vector<vector<float>> getComQueueVectorCS2()
{
  return queueVector_CS2;
}

vector<vector<float>> getComQueueVectorCS3()
{
  return queueVector_CS3;
}

vector<vector<float>> getComQueueVectorCS4()
{
  return queueVector_CS4;
}

void printQueueVector()
{

  cout << "-----------KÖLISTA-----------" << endl;
  cout << "-LS1-----LS2-----LS3-----LS4-" << endl;
  // Get the maximum size among all vectors
  size_t maxSize = max({queueVector_CS1.size(), queueVector_CS2.size(), queueVector_CS3.size(),
                        queueVector_CS4.size()});

  // Iterate over the vectors concurrently
  for (size_t i = 0; i < maxSize; ++i)
  {
    // Print elements from queueVector_CS1
    if (i < queueVector_CS1.size())
    {
      for (const auto &element : queueVector_CS1[i])
      {
        cout << element << ' ';
      }
    }
    cout << '\t'; // Separate columns

    // Print elements from queueVector_CS2
    if (i < queueVector_CS2.size())
    {
      for (const auto &element : queueVector_CS2[i])
      {
        cout << element << ' ';
      }
    }
    cout << '\t'; // Separate columns

    // Print elements from queueVector_CS3
    if (i < queueVector_CS3.size())
    {
      for (const auto &element : queueVector_CS3[i])
      {
        cout << element << ' ';
      }
    }
    cout << '\t'; // Separate columns

    // Print elements from queueVector_CS4
    if (i < queueVector_CS4.size())
    {
      for (const auto &element : queueVector_CS4[i])
      {
        cout << element << ' ';
      }
    }
    cout << '\t'; // Separate columns

    cout << '\n'; // Move to the next row
  }
  cout << "-----------------------------" << endl;
}

// void painlessMesh::init(String ssid, String password, uint16_t port = 5555, WiFiMode_t connectMode = WIFI_AP_STA, _auth_mode authmode = AUTH_WPA2_PSK, uint8_t channel = 1, phy_mode_t phymode = PHY_MODE_11G, uint8_t maxtpw = 82, uint8_t hidden = 0, uint8_t maxconn = 4)

void initCOM()
{
  Serial.begin(115200);

  // ... other initialization code ...

  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  // userScheduler.addTask(taskSendMessage);
  //  delay(3000);
  // taskSendMessage.enable(); // Enable continuous message sending task
  mesh.getNodeId() % 1000;
}

void newConnectionCallback(uint32_t nodeId)
{

  nodeList.push_back(nodeId);
  Serial.printf("-->New Connection, nodeId = %u\n", nodeId % 1000);
  for (const auto &node : nodeList)
  {
    printf("node list :\n");
    cout << node << endl;
  }
}

void changedConnectionCallback()
{
  Serial.printf("Changed connections\n");
  for (const auto &node : nodeList)
  {
    if (!(mesh.isConnected(node)))
    {
      printf("%u is disconnected\n", node);
      nodeList.remove(node);
    }
  }
}

void disconnect()
{
  printf("***NODE DISCONNECTED***\n");
  mesh.stop();
}

WebsocketsClient websocketsClient;

/*void sendStatus(string statusMessage)
{
  cout << "TEST STATUS: " << statusMessage << endl;

  if (websocketsClient.connect("ws://your_server_ip:your_server_port"))
  {
    websocketsClient.send(statusMessage.c_str());
  }
  else
  {
    cerr << "Error connecting to WebSocket server\n";
  }
}
*/