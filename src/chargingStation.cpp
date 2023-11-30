#include "chargingStation.hpp"
#include <vector>

using namespace std;

const vector<int> route_CS1 = {9, 18, 9}; // CS1 --> CS2 | CS1 --> CS3 | CS1 --> CS4
const vector<int> route_CS2 = {9, 18, 9}; // CS2 --> CS3 | CS2 --> CS4 | CS2 --> CS1
const vector<int> route_CS3 = {9, 18, 9}; // CS3 --> CS4 | CS3 --> CS1 | CS3 --> CS2
const vector<int> route_CS4 = {9, 18, 9}; // CS4 --> CS1 | CS4 --> CS2 | CS4 --> CS3

ChargingStation::ChargingStation(){};

ChargingStation::ChargingStation(int idCS, int xcorCS, int ycorCS, string zoneCS)
{
  id = idCS;
  xcor = xcorCS;
  ycor = ycorCS;
  zone = zoneCS;
}