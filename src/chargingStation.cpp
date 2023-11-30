#include "chargingStation.hpp"
#include <vector>

using namespace std;

vector<int> route_CS1 = {10, 20, 10}; // CS1 --> CS2 | CS1 --> CS3 | CS1 --> CS4
vector<int> route_CS2 = {10, 20, 10}; // CS2 --> CS3 | CS2 --> CS4 | CS2 --> CS1
vector<int> route_CS3 = {10, 20, 10}; // CS3 --> CS4 | CS3 --> CS1 | CS3 --> CS2
vector<int> route_CS4 = {10, 20, 10}; // CS4 --> CS1 | CS4 --> CS2 | CS4 --> CS3

ChargingStation::ChargingStation(){};

ChargingStation::ChargingStation(int idCS, int xcorCS, int ycorCS, string zoneCS)
{
  id = idCS;
  xcor = xcorCS;
  ycor = ycorCS;
  zone = zoneCS;
}