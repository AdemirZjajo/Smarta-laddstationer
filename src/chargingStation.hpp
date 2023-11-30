#pragma once
#include <string>
#include <vector>

using namespace std;

class ChargingStation
{
public:
  int id;
  int xcor;
  int ycor;
  string zone;

  static const vector<int> route_CS1, route_CS2, route_CS3, route_CS4;

  ChargingStation();
  ChargingStation(int id, int xcor, int ycor, string zone);
};