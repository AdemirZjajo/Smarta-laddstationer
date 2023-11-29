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

using namespace std;

Scheduler userScheduler;      // Scheduler to control tasks
painlessMesh mesh;            // instance for our mesh
std::list<uint32_t> nodeList; // instance for our list
String MESH_PREFIX = "meshOrigin";
String MESH_PASSWORD = "123456789";
int MESH_PORT = 5555;
int Counter = 0;
int nodeId = 0;

pair<int, float> nodePair;

vector<vector<float>> queueVector;

vector<float> tempVect;
vector<float> tempVect2;
pair<int, float> tempNodeMsg;
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
  mesh.sendBroadcast(msg);
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 3));
  Serial.printf("Sent %s\n", msg.c_str());
}

void sendQ(int id, float points)
{
  // mesh.getNodeId()%1000;
  String qPoints = "";
  qPoints += id;
  qPoints += "-";
  qPoints += points;
  mesh.sendBroadcast(qPoints);
  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 3));
  // Serial.printf("Sent %s\n", qPoints.c_str());
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
  
  // Sorterar vektorn
  sort(queueVector.begin(),
       queueVector.end(),
       [](const vector<float> &a, const vector<float> &b)
       {
         return a[1] > b[1];
       });
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

pair<int, float> splitString(const string &input)
{
  // Find the position of the "-" character
  size_t pos = input.find('-');

  // Check if the "-" character is found
  if (pos != string::npos)
  {
    // Extract the substrings before and after the "-"
    string firstPart = input.substr(0, pos);
    string secondPart = input.substr(pos + 1);

    // Convert the substrings to double values
    int firstValue = stod(firstPart);
    float secondValue = stod(secondPart);

    // Return a pair of double values
    return make_pair(firstValue, secondValue);
  }
  else
  {
    // Handle the case where "-" is not found
    // You might want to throw an exception or handle it differently based on your needs
    cerr << "Error: '-' not found in the input string" << endl;
    return make_pair(0.0, 0.0); // Return default values or handle the error accordingly
  }
}

// This notifies the ESP when a message is recieved
void receivedCallback(uint32_t from, String &msg)
{
  // cout << msg.c_str() << endl;
  string stringMsg = msg.c_str();
  pair<int, float> result = splitString(stringMsg);

  // Print the results
  // cout << "First value: " << result.first << endl;
  // cout << "Second value: " << result.second << endl;

  // Serial.printf("Received  %s\n", msg.c_str());

  // Serial.printf("Received  %s\n", q.c_str());

  nodePair = result;

  /* if (Counter < 5)
   {
     Counter = Counter + 1;
     printf("Counter : %d\n", Counter);
   }
   else if (Counter == 5)
   {
     MESH_PREFIX = "laddstiation2";
     printf("Changed MESH perfix \n");
     Counter += 1;
   }*/

  // Lägger till andra i vektorn
  tempVect2 = {static_cast<float>(nodePair.first), nodePair.second};

  exists = false;
  for (const auto &vec : queueVector)
  {
    if (vec[0] == tempVect2[0])
    {
      exists = true;
      break;
    }
  }

  if (!exists)
  {
    queueVector.push_back(tempVect2);
  }

  // Sorterar vektorn
  sort(queueVector.begin(),
       queueVector.end(),
       [](const vector<float> &a, const vector<float> &b)
       {
         return a[1] > b[1];
       });
}

pair<int, float> getNodePair()
{
  // cout << "Pair: (" << nodePair.first << ", " << nodePair.second << ")" << endl;
  return nodePair;
};

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

  userScheduler.addTask(taskSendMessage);
  delay(3000);
  taskSendMessage.enable(); // Enable continuous message sending task
  mesh.getNodeId() % 1000;
}