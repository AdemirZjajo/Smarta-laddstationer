#include "mission.hpp"
#include <Arduino.h>

/*
 * Detta är uppdragsobjekten, för varje nytt slumpat uppdrag skapas ett nytt unikt objekt
 */
/*
Mission::Mission()
{
}*/
using namespace std;

Mission::Mission(/*ChargingStation initCS, ChargingStation goalCS*/)
{
    // Seed the random number generator with a value (you can change this value)
    randomSeed(analogRead(32));
    last = random() % 11;
    kylvara = rand() % 2;
    // missionOrigin = initCS;
    // missionDestination = goalCS;
}

// metod som genererar ett uppdrag till noden
// last: exempelvis 1-5 ton
// förbrukning: baserat på last