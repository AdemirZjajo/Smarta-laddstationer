#ifndef MISSION_H
#define MISSION_H

#include "chargingStation.hpp"

/*
 * Detta är uppdragsobjekten, för varje nytt slumpat uppdrag skapas ett nytt unikt objekt
 */
class Mission
{
public:
  int last;     // Nodens last för den nuvarande uppdrag
  bool kylvara; // Detta är lasttypen. true om det är kylvara, eller false om inte är det
  ChargingStation missionOrigin;
  ChargingStation missionDestination;

  Mission();
  Mission(ChargingStation initCS, ChargingStation goalCS);
};

// metod som genererar ett uppdrag till noden
// last: exempelvis 1-5 ton
// förbrukning: baserat på last

#endif