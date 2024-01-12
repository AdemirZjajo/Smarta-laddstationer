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
vector<vector<float>> vector_transit;

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
void clearComVector()
{
  queueVector.clear();
}

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

  for (auto it = queueVector.begin(); it != queueVector.end(); ++it)
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
    queueVector.push_back(tempVect);
    cout << "Nod-" << message.sender_nod_id << " lägger till sig själv i kölistan för: " << message.message_zone << endl;
  }
}

void eraseSelfFromQueue(Message message)
{

  for (auto it = queueVector.begin(); it != queueVector.end(); ++it)
  {
    if ((*it)[0] == static_cast<float>(message.sender_nod_id))
    {
      queueVector.erase(it);
      cout << "Nod-" << message.sender_nod_id << " tog bort sig själv från kölistan för: " << message.message_zone << endl;
      break;
    }
  }
}

void sortQueue()
{

  if (!(queueVector.empty()))
  {
    sort(queueVector.begin(),
         queueVector.end(),
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
void removeMissingNodes()
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
}

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
    for (auto it = queueVector.begin(); it != queueVector.end(); ++it)
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
      queueVector.push_back(tempVect);
      cout << "Nod-" << from % 1000 << " läggs till i kölista för: " << callbackStruct.cs.c_str() << endl;
    }
  }

  else if (callbackStruct.category == "RemoveFromQueue")
  {
    for (auto it = queueVector.begin(); it != queueVector.end(); ++it)
    {
      if ((*it)[0] == callbackStruct.nodeID)
      {
        queueVector.erase(it);
        cout << "Nod-" << callbackStruct.nodeID << " blev borttagen från kölista för: " << callbackStruct.cs.c_str() << endl;
        break;
      }
    }
  }

  sortQueue();
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

void updateCommunication()
{
  // it will run the user scheduler as well
  mesh.update();
}

vector<vector<float>> getComQueueVector()
{
  return queueVector;
}

void printQueueVector()
{
  /*cout << "--KÖLISTA--" << endl;
  for (const auto &row : queueVector)
  {
    for (const auto &element : row)print
    {
      cout << element << ' ';
    }
    cout << '\n';
  }*/

  cout << "------------KÖLISTA------------" << endl;

  // Get the maximum size among all vectors
  size_t maxSize = max({queueVector_CS1.size(), queueVector_CS2.size(), queueVector_CS3.size(),
                        queueVector_CS4.size(), vector_transit.size()});

  // Iterate over the vectors concurrently
  for (size_t i = 0; i < maxSize; ++i)
  {
    // Print elements from queueVector_CS1
    if (i < queueVector.size())
    {
      for (const auto &element : queueVector[i])
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

    // Print elements from vector_transit
    if (i < vector_transit.size())
    {
      for (const auto &element : vector_transit[i])
      {
        cout << element << ' ';
      }
    }
    cout << '\t'; // Separate columns

    cout << '\n'; // Move to the next row
  }
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

void disconnect()
{
  printf("***NODE DISCONNECTED***\n");
  mesh.stop();
}

void sendStatus(Message message)
{

  // cout "TEST STATUS" << endl;

  // Server address
  const char *server_ip = "127.0.0.1"; // Replace with your server IP
  const int server_port = 12345;       // Replace with your server port

  // Create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    cerr << "Error creating socket\n";
    return;
  }

  // Set up server address
  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(server_port);
  inet_pton(AF_INET, server_ip, &serverAddr.sin_addr);

  // Connect to the server
  if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
  {
    cerr << "Error connecting to the server\n";
    close(sockfd);
    return;
  }

  /*string state = message.sender.state;
  string nodeID = to_string(message.sender.node_id);
  string battery = to_string(message.sender.battery_charge);
  string batteryCon = to_string(message.sender.battery_consumption);
  string minCharge = to_string(message.sender.min_charge);
  string queuePoint = to_string(message.sender.queue_point);
  string zone = message.sender.zone;
  string x = to_string(message.sender.xcor);
  string y = to_string(message.sender.ycor);
  string orgin = message.sender.current_mission.missionOrigin.zone;
  string dest = message.sender.current_mission.missionDestination.zone;
  string freight = to_string(message.sender.current_mission.last);
  string coldFreight = message.sender.current_mission.kylvara ? "true" : "false";

  // Creating a single string with commas between values
  string serializedMessage = nodeID + "," + battery + "," + batteryCon + "," +
                             minCharge + "," + queuePoint + "," + zone + "," +
                             x + "," + y + "," + orgin + "," + dest + "," +
                             freight + "," + coldFreight;


  // Output the serialized message
  cout << serializedMessage << endl;*/

  string serializedMessage = "Hejhej";

  // Send the message
  if (send(sockfd, serializedMessage.c_str(), serializedMessage.size(), 0) == -1)
  {
    cerr << "Error sending message\n";
  }

  // Close the socket
  close(sockfd);
}

/*ON THE SERVER SIDE MAYBE??

import React, { useEffect, useState } from 'react';

const WebSocketExample = () => {
  const [messages, setMessages] = useState([]);

  useEffect(() => {
    // Replace 'ws://your-server-ip:your-port' with your WebSocket server's address
    const socket = new WebSocket('ws://127.0.0.1:12345');

    // Handle connection open
    socket.onopen = () => {
      console.log('WebSocket connected');
    };

    // Handle incoming messages
    socket.onmessage = (event) => {
      const message = JSON.parse(event.data);
      setMessages((prevMessages) => [...prevMessages, message]);
      console.log('Received message:', message);
    };

    // Handle connection close
    socket.onclose = () => {
      console.log('WebSocket closed');
    };

    // Clean up the WebSocket connection when the component unmounts
    return () => {
      socket.close();
    };
  }, []);

  return (
    <div>
      <h2>Received Messages:</h2>
      <ul>
        {messages.map((message, index) => (
          <li key={index}>{JSON.stringify(message)}</li>
        ))}
      </ul>
    </div>
  );
};

export default WebSocketExample;
 */