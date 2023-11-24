//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//

/*
1. TEst for intializing the mes
2. Test for connection/changed connection
3. Test for sending/broadcasting
4. test for recieving
5. test for disconnecting
6. test for changing network
*/

//************************************************************
#include "communication.hpp"
#include <painlessMesh.h>
#include <list>
#include <iostream>

using namespace std;

Scheduler userScheduler;      // Scheduler to control tasks
painlessMesh mesh;            // instance for our mesh
std::list<uint32_t> nodeList; // instance for our list
String MESH_PREFIX = "meshOrigin";
String MESH_PASSWORD = "1234";
int MESH_PORT = 5555;
int Counter = 0;
int nodeId = 0;
// User stub





void sendMessage(); // Prototype so PlatformIO doesn't complain

// Create the task
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

// Creating the message and broadcast it here
void sendMessage()
{
  String msg = "Hello from node ";
  nodeId = mesh.getNodeId() % 1000;
  msg += nodeId;
  mesh.sendBroadcast(msg);
  taskSendMessage.setInterval(random(TASK_SECOND * 3, TASK_SECOND * 5));
  Serial.printf("Sent %s\n", msg.c_str());
}



COM::COM()
{
  initCOM();
}

void changeCS(string zoneCode)
{
  if (zoneCode == "CS1_ZON")
  {
    MESH_PREFIX = "station1";
    String MESH_PASSWORD = "station1";
  }
  else if (zoneCode == "CS2_ZON")
  {
    MESH_PREFIX = "station2";
    String MESH_PASSWORD = "station2";
  }
  else if (zoneCode == "CS3_ZON")
  {
    MESH_PREFIX = "station3";
    String MESH_PASSWORD = "station3";
  }
  else if (zoneCode == "CS4_ZON")
  {
    MESH_PREFIX = "station4";
    String MESH_PASSWORD = "station4";
  }
}

// returns the node id, mainly used in display
int getID()
{
  return nodeId;
}

// This notifies the ESP whe a message is recieved
void receivedCallback(uint32_t from, String &msg)
{
  Serial.printf("Received  %s\n", msg.c_str());
  if (Counter < 5)
  {
    Counter = Counter + 1;
    printf("Counter : %d\n", Counter);
  }
  else if (Counter == 5)
  {
    MESH_PREFIX = "laddstiation2";
    String MESH_PASSWORD = "1234";
    printf("Changed MESH perfix \n");
    Counter += 1;
  }
}

void newConnectionCallback(uint32_t nodeId)
{

  nodeList.push_back(nodeId);
  Serial.printf("-->New Connection, nodeId = %u\n", nodeId);
  for (const auto &node : nodeList)
  {
    printf("node list :\n");
    std::cout << node << std::endl;
  }
}

void changedConnectionCallback()
{
  Serial.printf("Changed connections\n");
  for (const auto &node : nodeList)
  {
    if (mesh.isConnected(node))
    {
    }
    else
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
  if (Counter == 6)
  {
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    printf("MESH UPDATED \n");
    Counter += 1;
  }
}

void initCOM()
{
  Serial.begin(115200);

  // ... other initialization code ...

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  userScheduler.addTask(taskSendMessage);
  delay(3000);
  taskSendMessage.enable(); // Enable continuous message sending task
}