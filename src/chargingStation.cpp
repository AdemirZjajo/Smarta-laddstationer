#include "chargingStation.hpp"
#include <vector>

ChargingStation::ChargingStation(){};

ChargingStation::ChargingStation(int idCS, int xcorCS, int ycorCS, string zoneCS)
{
  id = idCS;
  xcor = xcorCS;
  ycor = ycorCS;
  zone = zoneCS;
}