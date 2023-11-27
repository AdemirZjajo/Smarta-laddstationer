#include "mission.hpp"

/*
 * Detta är uppdragsobjekten, för varje nytt slumpat uppdrag skapas ett nytt unikt objekt
 */
/*
Mission::Mission()
{
}*/
Mission::Mission(/*ChargingStation initCS, ChargingStation goalCS*/)
{
    last = rand() % 11;
    kylvara = rand() % 2;
    //missionOrigin = initCS;
    //missionDestination = goalCS;
}

// metod som genererar ett uppdrag till noden
// last: exempelvis 1-5 ton
// förbrukning: baserat på last