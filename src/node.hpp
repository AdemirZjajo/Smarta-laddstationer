#ifndef NODE_H
#define NODE_H

#include "chargingStation.hpp"
#include "mission.hpp"
#include <list>
// randomize function in this class and show on display
// Prio
// State
// Communication
// Display

using namespace std;

class Node
{
public:
  int nod_id;
  int battery_charge;
  int battery_consumption;    // Hur många procentenheter batteri som noden kräver för att förflytta sig ett steg i x-led eller y-led
  Mission current_mission;    // Uppdateras dynamiskt under uppdragsgivande
  ChargingStation current_CS; // Laddstationen som noden är vid just nu
  int xcor;                   // Nodens x-koordinat
  int ycor;                   // Nodens y-koordinat
  string zon;                 // Kan anta värden laddstation 1-3 samt ett default värde, kanske "0" eller bara "default"

  // En lista med objekt av laddsationerna som i sin tur innehåller sitt namn (eg. ls_1) och sina paths till andra laddstationer
  ChargingStation LS1, LS2, LS3, LS4;
  list<ChargingStation> chargingStations = {LS1, LS2, LS3, LS4};

  // Nodens initieringsprocess börjar här
  /* Destinationsgenerering och uträkning av miniladdning görs en gång innan
  state machinen för att låta noden bestämma vilket dess första state ska bli. */

  Node();
  Node(int name);

  Mission generateMission(ChargingStation currentCS);

  int calcBatConsumption(Mission mission);

  void calcMinCharge(int battery_consumption, Mission mission);
};

#endif