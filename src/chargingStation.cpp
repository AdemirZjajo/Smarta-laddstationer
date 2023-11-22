#include "chargingStation.hpp"
#include <vector>

using namespace std;

vector<int> route_CS1 = {50, 100, 50}; // CS1 --> CS2 | CS1 --> CS3 | CS1 --> CS4
vector<int> route_CS2 = {50, 100, 50}; // CS2 --> CS3 | CS2 --> CS4 | CS2 --> CS1
vector<int> route_CS3 = {50, 100, 50}; // CS3 --> CS4 | CS3 --> CS1 | CS3 --> CS2
vector<int> route_CS4 = {50, 100, 50}; // CS4 --> CS1 | CS4 --> CS2 | CS4 --> CS3

ChargingStation::ChargingStation(){};

ChargingStation::ChargingStation(int idCS, int xcorCS, int ycorCS, string zonCS)
{
  id = idCS;
  xcor = xcorCS;
  ycor = ycorCS;
  zon = zonCS;
}