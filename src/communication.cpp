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
#include <string>
#include <vector>
#include <tuple>

using namespace std;

Scheduler userScheduler; // Scheduler to control tasks
painlessMesh mesh;       // instance for our mesh
list<uint32_t> nodeList; // instance for our list
String MESH_PREFIX = "meshOrigin";
String MESH_PASSWORD = "123456789";
int MESH_PORT = 5555;
int Counter = 0;
int nodeId = 0;

tuple<int, int, float> queueTuple;

vector<vector<float>> queueVector;

vector<float> tempVect;
bool exists;

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
  // mesh.sendBroadcast(msg);
  // taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 3));
  Serial.printf("Sent %s\n", msg.c_str());
}

void sendQ(int id, float points)
{
  // mesh.getNodeId()%1000;
  String qPoints = "0";
  qPoints += "-";
  qPoints += id;
  qPoints += "-";
  qPoints += points;
  mesh.sendBroadcast(qPoints);
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 3));
  // cout << qPoints.c_str() << endl;
  /*
  String qPoints = String(id) + "-" + String(points);
  mesh.sendBroadcast(qPoints);
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 3));
  Serial.printf("Sent %s\n", qPoints.c_str());
  */

  // Lägger till sig själv i vektorn
  tempVect = {static_cast<float>(id), points};

  exists = false;
  for (const auto &vec : queueVector)
  {
    if (vec[0] == tempVect[0])
    {
      exists = true;
      break;
    }
  }

  if (!exists)
  {
    queueVector.push_back(tempVect);
  }
}

void sendRemove(int id)
{
  String tempStr = "1";
  tempStr += "-";
  tempStr += id;
  tempStr += "-";
  tempStr += "";
  mesh.sendBroadcast(tempStr);
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 3));
}

void changeCS(string zoneCode)
{
  if (zoneCode == "CS1_ZON")
  {
    MESH_PREFIX = "station1";
    String MESH_PASSWORD = "station1";
    MESH_PORT = 1111;
  }
  else if (zoneCode == "CS2_ZON")
  {
    MESH_PREFIX = "station2";
    String MESH_PASSWORD = "station2";
    MESH_PORT = 2222;
  }
  else if (zoneCode == "CS3_ZON")
  {
    MESH_PREFIX = "station3";
    String MESH_PASSWORD = "station3";
    MESH_PORT = 3333;
  }
  else if (zoneCode == "CS4_ZON")
  {
    MESH_PREFIX = "station4";
    String MESH_PASSWORD = "station4";
    MESH_PORT = 4444;
  }
}

// returns the node id, mainly used in display
int getID()
{
  return mesh.getNodeId() % 1000;
}

tuple<int, int, float> splitString(const string &input)
{
  // Find the positions of the "-" characters
  size_t pos1 = input.find('-');
  size_t pos2 = input.find('-', pos1 + 1);

  // Check if both "-" characters are found
  if (pos1 != string::npos && pos2 != string::npos)
  {
    // Extract the substrings between the "-" characters
    string firstPart = input.substr(0, pos1);
    string secondPart = input.substr(pos1 + 1, pos2 - pos1 - 1);
    string thirdPart = input.substr(pos2 + 1);

    // Convert the substrings to integer and float values
    int firstValue = stoi(firstPart);
    int secondValue = stoi(secondPart);
    float thirdValue = stof(thirdPart);

    // Return a tuple of integer and float values
    return make_tuple(firstValue, secondValue, thirdValue);
  }
  else
  {
    // Handle the case where one or both "-" characters are not found
    // You might want to throw an exception or handle it differently based on your needs
    cerr << "Error: One or both '-' characters not found in the input string" << endl;
    return make_tuple(0, 0, 0.0f); // Return default values or handle the error accordingly
  }
}

// This notifies the ESP when a message is recieved
void receivedCallback(uint32_t from, String &msg)
{
  // cout << msg.c_str() << endl;
  string stringMsg = msg.c_str();
  tuple<int, int, float> result = splitString(stringMsg);

  queueTuple = result;

  switch (get<0>(queueTuple))
  {
  case 0: // Lägga in annan i vektorn
    tempVect = {static_cast<float>(get<1>(queueTuple)), get<2>(queueTuple)};

    exists = false;
    for (const auto &vec : queueVector)
    {
      if (vec[0] == tempVect[0])
      {
        exists = true;
        break;
      }
    }

    if (!exists)
    {
      queueVector.push_back(tempVect);
    }
  
    break;

  case 1: // Ta bort annan från vektorn
    for (auto it = queueVector.begin(); it != queueVector.end(); ++it) {
        if ((*it)[0] == static_cast<float>(std::get<1>(queueTuple))) {
            queueVector.erase(it);
            cout << "Vector removed successfully." << endl;
            break;  // Exit the loop after erasing the element
        } else {
            cout << "ERROR: Tried to remove but couldn't" << endl;
        }
 }
 }
}
  /*
    for (const auto &vec : queueVector)
    {
      if (vec[0][0] == static_cast<float>(get<1>(queueTuple)))
      {
        queueVector.erase(queueVector[queueVector.begin]);

        break;
      }
      else
      {
        cout << "ERROR TRIED TO REMOVE BUT COULDNT" << endl;
      }
    }
    break;
*/

 

void newConnectionCallback(uint32_t nodeId)
{

  nodeList.push_back(nodeId);
  Serial.printf("-->New Connection, nodeId = %u\n", nodeId);
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

  /* if (Counter == 6)
   {
     mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
     printf("MESH UPDATED \n");
     Counter += 1;
   }*/
}

vector<vector<float>> getComQueueVector()
{
  return queueVector;
}

void initCOM()
{
  Serial.begin(115200);

  // ... other initialization code ...

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  //userScheduler.addTask(taskSendMessage);
  // delay(3000);
  //taskSendMessage.enable(); // Enable continuous message sending task
  mesh.getNodeId() % 1000;
}