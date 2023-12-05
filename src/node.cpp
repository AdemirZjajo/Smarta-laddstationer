#include "node.hpp"
#include <iostream>
#include <vector>
#include <esp_adc_cal.h>

// randomize function in this class and show on display
// Prio
// State
// Communication
// Display

using namespace std;

ChargingStation CS1(1, 0, 0, "LADDSTATION_1"), CS2(2, 0, 9, "LADDSTATION_2"), CS3(3, 9, 0, "LADDSTATION_3"), CS4(4, 9, 9, "LADDSTATION_4");
list<ChargingStation> chargingStations = {CS1, CS2, CS3, CS4};

/*
// Returnar ett slumpat tal
int randomNumber(int from, int to)
{
  // Seed the random number generator with a value (you can change this value)
  randomSeed(analogRead(32));

  // Generate a pseudo-random number between 1 and 100
  int randomNumber = random(from, to);

  return randomNumber;
}
*/

int randomNumber(int from, int to)
{
  int randomnumber = random(from, to);
  return randomnumber;
}

// Randomly selects a charging station to spawn at
ChargingStation randomCS(ChargingStation CS1, ChargingStation CS2, ChargingStation CS3, ChargingStation CS4)
{
  int randomInt = randomNumber(0, 3);

  // Map the random number to one of the four objects
  switch (randomInt)
  {
  case 0:
    return CS1;
  case 1:
    return CS2;
  case 2:
    return CS1;
  case 3:
    return CS2;
  default:
    return CS1;
  }
};

// Returnar en slumpvis vald laddstation som inte är den nuvarande laddstationen
ChargingStation randomNotCurrentCS(ChargingStation CS1, ChargingStation CS2, ChargingStation CS3, ChargingStation CS4, ChargingStation notThisCS)
{

  ChargingStation chosenCS;
  do
  {
    int randomInt = randomNumber(0, 3);

    // Map the random number to one of the four objects
    switch (randomInt)
    {
    case 0:
      chosenCS = CS1;
      break;
    case 1:
      chosenCS = CS2;
      break;
    case 2:
      chosenCS = CS1;
      break;
    case 3:
      chosenCS = CS2;
      break;
    default:
      chosenCS = CS1;
      break;
    }
  } while (chosenCS.id == notThisCS.id);
  return chosenCS;
};
Node::Node() {}
Node::Node(int id)
{

  // Nodens ID blir tlldelat :)
  node_id = id;

  // En lista med objekt av laddsationerna som i sin tur innehåller sitt namn (eg. ls_1) och sina paths till andra laddstationer

  // NODEN spawnar randomly på en av de fyra Laddstationer
  ChargingStation init_CS = randomCS(CS1, CS2, CS3, CS4);
  current_CS = CS1;       // init_CS;
  xcor = current_CS.xcor; // Nodens initiala x-koordinat
  ycor = current_CS.ycor; // Nodens initiala y-koordinat
  zone = current_CS.zone; // Nodens initiala zon

  // NODEN får en random batterinivå som utgångsvärde
  battery_charge = randomNumber(1, 100);
  // Hur många procentenheter batteri som noden kräver för att förflytta sig ett steg i x-led eller y-led

  current_mission = generateMission(init_CS); // Uppdateras dynamiskt under uppdragsgivande
  battery_consumption = calcBatConsumption(current_mission);
  min_charge = calcMinCharge(battery_consumption, calcStepsNeeded(current_mission)); // Beräkna minimumladdning baserat på uppdraget
  queue_point = randomNumber(1, 100);
}

// Nodens initieringsprocess börjar här
/* Destinationsgenerering och uträkning av miniladdning görs en gång innan
state machinen för att låta noden bestämma vilket dess första state ska bli. */

// Genererar ett nytt uppdrag för noden, inparametern är den plats som noden är just nu; ofta current_mission.missionDestination eftersom noden vill ha ett nytt uppdrag efter den avslutat det tidigare uppdraget
Mission Node::generateMission(ChargingStation current_CS)
{
  ChargingStation goal_CS = randomNotCurrentCS(CS1, CS2, CS3, CS4, current_CS);
  Mission mission;
  mission.missionOrigin = current_CS;
  mission.missionDestination = goal_CS;

  return mission;
}

// Returnerar batteriförbruking beroende på lasten hos det nuvarande uppdraget
float Node::calcBatConsumption(Mission mission)
{
  // Last varierar mellan 0-10 ton
  switch (mission.last)
  {
  case 0:
    return 0.5;
  case 1:
    return 1;
  case 2:
    return 1.5;
  case 3:
    return 2;
  case 4:
    return 2.5;
  case 5:
    return 3;
  case 6:
    return 3.5;
  case 7:
    return 4;
  case 8:
    return 4.5;
  case 9:
    return 5;
  case 10:
    return 5.5;
  default:
    return 1;
  }
}

// Beräknar antalet steg som noden behöver ta i koordinatsystemet, detta används för att senare beräkna minimumladdning
int Node::calcStepsNeeded(Mission currentMission)
{
  int steps_needed = 0;
  ChargingStation start = currentMission.missionOrigin;
  ChargingStation finish = currentMission.missionDestination;

  switch (start.id)
  {
  case 1:
    switch (finish.id)
    {
    case 2:
      steps_needed = start.route_CS1[0];
      break;
    case 3:
      steps_needed = start.route_CS1[1];
      break;
    case 4:
      steps_needed = start.route_CS1[2];
      break;
    }
    break;

  case 2:
    switch (finish.id)
    {
    case 3:
      steps_needed = start.route_CS2[0];
      break;
    case 4:
      steps_needed = start.route_CS2[1];
      break;
    case 1:
      steps_needed = start.route_CS2[2];
      break;
    }
    break;

  case 3:
    switch (finish.id)
    {
    case 4:
      steps_needed = start.route_CS3[0];
      break;
    case 1:
      steps_needed = start.route_CS3[1];
      break;
    case 2:
      steps_needed = start.route_CS3[2];
      break;
    }
    break;

  case 4:
    switch (finish.id)
    {
    case 1:
      steps_needed = start.route_CS4[0];
      break;
    case 2:
      steps_needed = start.route_CS4[1];
      break;
    case 3:
      steps_needed = start.route_CS4[2];
      break;
    }
    break;
  }
  return steps_needed;
}

// Beräknar minimumladdning
float Node::calcMinCharge(float battery_consumption, int steps_needed)
{
  return steps_needed * battery_consumption;
}
