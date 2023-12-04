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
String MESH_PREFIX = "meshOrigi";
String MESH_PASSWORD = "12345678";
int MESH_PORT = 5556;
int Counter = 0;
int nodeId = 0;
String currentZon;
tuple<int, int, float> queueTuple;

vector<vector<float>> queueVector;

vector<float> tempVect;
bool exists;

void sendMessage(); // Prototype so PlatformIO doesn't complain

// Create the task
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

// Clear the list
void clearComVector()
{
  queueVector.clear();
}

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

void sendQ(int id, float points)
{
  if (points == 9999)
  {
    String qPoints = "0";
    qPoints += "-";
    qPoints += id;
    qPoints += "-";
    qPoints += points;

    mesh.sendBroadcast(qPoints);
  }
  else
  {
    String qPoints = "0";
    qPoints += "-";
    qPoints += id;
    qPoints += "-";
    qPoints += points;
    mesh.sendBroadcast(qPoints);

    // Lägger till sig själv i vektorn
    tempVect = {static_cast<float>(id), static_cast<float>(points)};
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
    }
  }
}

void sendRemove(int id)
{
  cout << "Början av sendRemove" << endl;
  String tempStr = "1";
  tempStr += "-";
  tempStr += id;
  tempStr += "-";
  tempStr += "Guar";
  mesh.sendBroadcast(tempStr);
  // taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 3));
}

void changeCS(string zoneCode)
{
  printf("changing Zone \n");
  if (zoneCode == "LADDSTATION-1")
  {
    MESH_PREFIX = "station1";
    // String MESH_PASSWORD = "station1";
    MESH_PORT = 1111;
    mesh.update();
    printf("changed LS  to LS1\n", zoneCode);
    // delay(2000);
  }
  else if (zoneCode == "LADDSTATION-2")
  {
    MESH_PREFIX = "station2";
    // String MESH_PASSWORD = "station2";
    MESH_PORT = 2222;
    mesh.update();
    printf("changed LS to LS2\n", zoneCode);
    // delay(2000);
  }
  else if (zoneCode == "LADDSTATION-3")
  {
    MESH_PREFIX = "station3";
    // String MESH_PASSWORD = "station3";
    MESH_PORT = 3333;
    mesh.update();
    printf("changed LS from to LS3\n", zoneCode);
    // delay(2000);
  }
  else if (zoneCode == "LADDSTATION-4")
  {
    MESH_PREFIX = "station4";
    // String MESH_PASSWORD = "station4";
    MESH_PORT = 4444;
    mesh.update();
    printf("changed LS to LS4\n", zoneCode);
    // delay(2000);
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
  cout << "NOD:" << from << " " << msg.c_str() << endl;
  string stringMsg = msg.c_str();
  tuple<int, int, float> queueTuple = splitString(stringMsg);

  switch (get<0>(queueTuple))
  {
  case 0: // Lägga in annan i vektorn
    if (get<2>(queueTuple) == 9999)
    {
      // cout << "Start of 9999." << endl;
      for (auto it = queueVector.begin(); it != queueVector.end(); ++it)
      {
        if ((*it)[0] == get<1>(queueTuple))
        {
          queueVector.erase(it);
          cout << (*it)[0] << " Vector removed successfully." << endl;
          break;
        }
        else
        {
          cout << (*it)[0] << " ERROR: Tried to remove but couldn't" << endl;
        }
      }
    }
    else if (get<2>(queueTuple) == 9998)
    {
      cout << from << " in transit" << endl;
    }

    else
    {
      // cout << "case 0: lägg till någon i kölistan." << endl;
      tempVect = {static_cast<float>(get<1>(queueTuple)), get<2>(queueTuple)};

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
      }
    }
    break;

  case 1: // Ta bort annan från vektorn
    cout << "Början av case 1; ta bort från listan" << endl;

    for (auto it = queueVector.begin(); it != queueVector.end(); ++it)
    {
      if ((*it)[0] == get<1>(queueTuple))
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
    break;
  default:
    cout << "case Default" << endl;
    break;
  }
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
      sendQ(node % 1000, 9999);
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