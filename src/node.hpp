#pragma once
#ifndef NODE_H
#define NODE_H

#include "chargingStation.hpp"
#include "mission.hpp"
#include <list>
#include <Arduino.h>
// Randomize function in this class and show on display
// Prio
// State
// Communication
// Display

using namespace std;

class Node
{
public:
  int node_id;
  float battery_charge;
  float battery_consumption;  // Hur många procentenheter batteri som noden kräver för att förflytta sig ett steg i x-led eller y-led
  float min_charge;           // Minimum laddning för aktivt upprag
  Mission current_mission;    // Uppdateras dynamiskt under uppdragsgivande
  ChargingStation current_CS; // Laddstationen som noden är vid just nu
  int xcor;                   // Nodens x-koordinat
  int ycor;                   // Nodens y-koordinat
  string zone;                // Kan anta värden laddstation 1-3 samt ett default värde, kanske "0" eller bara "default"
  float queue_point;
  int messageID = 0;

  vector<vector<float>> queueVector{};

  vector<int> route_CS1;
  vector<int> route_CS2;
  vector<int> route_CS3;
  vector<int> route_CS4;

  // En lista med objekt av laddsationerna som i sin tur innehåller sitt namn (eg. ls_1) och sina paths till andra laddstationer
  ChargingStation LS1, LS2, LS3, LS4;
  list<ChargingStation> chargingStations = {LS1, LS2, LS3, LS4};

  // Nodens initieringsprocess börjar här
  /* Destinationsgenerering och uträkning av miniladdning görs en gång innan
  state machinen för att låta noden bestämma vilket dess första state ska bli. */

  Node();
  Node(int nodeID);

  Mission generateMission(ChargingStation currentCS);

  float calcBatConsumption(Mission mission);

  // Returnar antalet steg som noden behöver ta för att ta sig hela vägen fram till sin destination
  int calcStepsNeeded(Mission current_mission);

  float calcMinCharge(float battery_consumption, int steps_needed);
};

int randomNumber(int from, int to);

#endif