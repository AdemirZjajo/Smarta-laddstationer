<<<<<<< Updated upstream
#include "chargingStation.hpp"

#include <list>
//randomize function in this class and show on display
//Prio
//State
//Communication
//Display

enum State {
      TRANSIT,
      QUEUE,
      CHARGE
    };

using namespace std;

class Node {
  public: // Accesstyp
    //int batterinivå;
    //float batteriförbrukning; // Hur många procentenheter batteri som noden kräver för att förflytta sig ett steg i x-led eller y-led
                              // Uppdateras dynamiskt under uppdragsgivande

    int xcor; // Nodens x-koordinat
    int ycor; // Nodens y-koordinat
    const string zon; // Kan anta värden laddstation 1-3 samt ett default värde, kanske "0" eller bara "default"

    // En lista med objekt av laddsationerna som i sin tur innehåller sitt namn (eg. ls_1) och sina paths till andra laddstationer
    ChargingStation LS1;
    ChargingStation LS2;
    ChargingStation LS3;
    ChargingStation LS4;
    list<ChargingStation> chargingStations = {LS1, LS2, LS3, LS4};



    // Nodens initieringsprocess börjar här
    /* Destinationsgenerering och uträkning av miniladdning görs en gång innan
    state machinen för att låta noden bestämma vilket dess första state ska bli. */

    

};

int main() {
  State state = TRANSIT; // Starttillståndet ska bestämmas utifrån 

  switch (state) {
  case TRANSIT:
    // generera nytt uppdrag med last, typ av last, och förbrukning
    // beräkna minimumladdning baserat på uppdraget 
        // antalet element i aktuell rutt * förbrukning = minimumladdning
    // jämföra batterinivå med minimumladdning
    // om batterinivån är högre än minimumladdningen börjar noden förflytta sig till sin uppdragsdestination
    // stanna kvar i transit och börja förflytting
    // om batterinivån är lägre än minimumladdningen --> byta tillstånd till queueu
    //One second, one procent subtracted to battery
    break;
  case QUEUE:
  // if no one, charge
  // if at least one, communicate and prioritize queue
    break;
  case CHARGE:
  //in case of an emergency, quit charge
  //One second, one procent added to battery
    break;
  }
  return 0;
}



/* Transit state
Minus en procent batterinivå i slutet 
*/
=======
#include "node.hpp"
// randomize function in this class and show on display
// Prio
// State
// Communication
// Display

using namespace std;

ChargingStation CS1(1, 0, 0, "CS1_ZON"), CS2(2, 0, 49, "CS2_ZON"), CS3(3, 49, 0, "CS3_ZON"), CS4(4, 49, 49, "CS4_ZON");
list<ChargingStation> chargingStations = {CS1, CS2, CS3, CS4};

Node::Node(int id)
{

  // Nodens ID blir tlldelat :)
  nod_id = id;

  // En lista med objekt av laddsationerna som i sin tur innehåller sitt namn (eg. ls_1) och sina paths till andra laddstationer

  // NODEN spawnar randomly på en av de fyra Laddstationer
  ChargingStation init_CS = randomCS(CS1, CS2, CS3, CS4);
  xcor = init_CS.xcor; // Nodens initiala x-koordinat
  ycor = init_CS.ycor; // Nodens initiala y-koordinat
  zon = init_CS.zon;   // Nodens initiala zon

  // NODEN får en random batterinivå som utgångsvärde
  battery_charge = (rand() % 100);
  // Hur många procentenheter batteri som noden kräver för att förflytta sig ett steg i x-led eller y-led

  current_mission = generateMission(init_CS); // Uppdateras dynamiskt under uppdragsgivande
  battery_consumption = calcBatConsumption(current_mission);

  calcMinCharge(battery_consumption, current_mission);
}

// Nodens initieringsprocess börjar här
/* Destinationsgenerering och uträkning av miniladdning görs en gång innan
state machinen för att låta noden bestämma vilket dess första state ska bli. */

Mission generateMission(ChargingStation current_CS)
{
  ChargingStation goal_CS = randomNotCurrentCS(CS1, CS2, CS3, CS4, current_CS);
  Mission mission(current_CS, goal_CS);

  return mission;
};

int calcBatConsumption(Mission mission)
{
  switch (mission.last)
  {
  case 0:
    return 0.7;
  case 1:
    return 0.8;
  case 2:
    return 0.9;
  case 3:
    return 1;
  default:
    return 0.7;
  }
}

int calcMinCharge(int battery_consumption, Mission current_mission)
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
  }

  return steps_needed * battery_consumption;
};

// Randomly selects a charging station to spawn at
ChargingStation randomCS(ChargingStation CS1, ChargingStation CS2, ChargingStation CS3, ChargingStation CS4)
{
  int randomInt = rand() % 4;

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
    int randomInt = rand() % 4;

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
};
>>>>>>> Stashed changes
