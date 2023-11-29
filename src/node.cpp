#include "node.hpp"

// randomize function in this class and show on display
// Prio
// State
// Communication
// Display

using namespace std;

ChargingStation CS1(1, 0, 0, "LADDSTATION-1"), CS2(2, 0, 9, "LADDSTATION-2"), CS3(3, 9, 0, "LADDSTATION-3"), CS4(4, 9, 9, "LADDSTATION-4");
list<ChargingStation> chargingStations = {CS1, CS2, CS3, CS4};

// Randomly selects a charging station to spawn at
ChargingStation randomCS(ChargingStation CS1, ChargingStation CS2, ChargingStation CS3, ChargingStation CS4)
{
  int randomInt = randomNumber(0, 3, 34);

  // Map the random number to one of the four objects
  switch (randomInt)
  {
  case 0:
    return CS1;
  case 1:
    return CS2;
  case 2:
    return CS3;
  case 3:
    return CS4;
  default:
    return CS1;
  }
};

ChargingStation randomNotCurrentCS(ChargingStation CS1, ChargingStation CS2, ChargingStation CS3, ChargingStation CS4, ChargingStation notThisCS)
{

  ChargingStation chosenCS;
  do
  {
    int randomInt = randomNumber(0, 3, 35);

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
      chosenCS = CS3;
      break;
    case 3:
      chosenCS = CS4;
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
  nod_id = id;

  // En lista med objekt av laddsationerna som i sin tur innehåller sitt namn (eg. ls_1) och sina paths till andra laddstationer

  // NODEN spawnar randomly på en av de fyra Laddstationer
  ChargingStation init_CS = randomCS(CS1, CS2, CS3, CS4);
  current_CS = init_CS;
  xcor = init_CS.xcor; // Nodens initiala x-koordinat
  ycor = init_CS.ycor; // Nodens initiala y-koordinat
  zon = init_CS.zon;   // Nodens initiala zon

  // NODEN får en random batterinivå som utgångsvärde
  battery_charge = randomNumber(1, 100, 36);
  // Hur många procentenheter batteri som noden kräver för att förflytta sig ett steg i x-led eller y-led

  current_mission = generateMission(init_CS); // Uppdateras dynamiskt under uppdragsgivande
  battery_consumption = calcBatConsumption(current_mission);
  // min_charge = calcMinCharge(battery_consumption, current_mission); // Beräkna minimumladdning baserat på uppdraget

  min_charge = 50;
  // calcMinCharge(battery_consumption, calcStepsNeeded(current_mission));
  queue_point = randomNumber(1, 100, 34);
}

// Nodens initieringsprocess börjar här
/* Destinationsgenerering och uträkning av miniladdning görs en gång innan
state machinen för att låta noden bestämma vilket dess första state ska bli. */

Mission Node::generateMission(ChargingStation current_CS)
{
  ChargingStation goal_CS = randomNotCurrentCS(CS1, CS2, CS3, CS4, current_CS);
  Mission mission;
  mission.missionOrigin = current_CS;
  mission.missionDestination = goal_CS;

  return mission;
};

float Node::calcBatConsumption(Mission mission)
{
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
    return 1.3;
  }
}

int Node::calcStepsNeeded(Mission current_mission)
{
  int steps_needed;
  switch (current_mission.missionOrigin.id)
  {
  case 1:
    switch (current_mission.missionDestination.id)
    {
    case 2:
      steps_needed = current_mission.missionOrigin.route_CS1[0];
      break;
    case 3:
      steps_needed = current_mission.missionOrigin.route_CS1[1];
      break;
    case 4:
      steps_needed = current_mission.missionOrigin.route_CS1[2];
      break;
    }
    break;

  case 2:
    switch (current_mission.missionDestination.id)
    {
    case 3:
      steps_needed = current_mission.missionOrigin.route_CS2[0];
      break;
    case 4:
      steps_needed = current_mission.missionOrigin.route_CS2[1];
      break;
    case 1:
      steps_needed = current_mission.missionOrigin.route_CS2[2];
      break;
    }
    break;

  case 3:
    switch (current_mission.missionDestination.id)
    {
    case 4:
      steps_needed = current_mission.missionOrigin.route_CS3[0];
      break;
    case 1:
      steps_needed = current_mission.missionOrigin.route_CS3[1];
      break;
    case 2:
      steps_needed = current_mission.missionOrigin.route_CS3[2];
      break;
    }
    break;

  case 4:
    switch (current_mission.missionDestination.id)
    {
    case 1:
      steps_needed = current_mission.missionOrigin.route_CS4[0];
      break;
    case 2:
      steps_needed = current_mission.missionOrigin.route_CS4[1];
      break;
    case 3:
      steps_needed = current_mission.missionOrigin.route_CS4[2];
      break;
    }
    break;
  }

  return steps_needed;
};

float Node::calcMinCharge(float battery_consumption, int steps_needed)
{
  return steps_needed * battery_consumption;
};

int randomNumber(int from, int to, int seed)
{
  // Seed the random number generator with a value (you can change this value)
  randomSeed(analogRead(seed));

  // Generate a pseudo-random number between 1 and 100
  int randomNumber = random(from, to);

  return randomNumber;
}