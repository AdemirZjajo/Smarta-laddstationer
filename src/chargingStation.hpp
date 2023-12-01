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

  vector<int> route_CS1 = {9, 18, 9}; // CS1 --> CS2 | CS1 --> CS3 | CS1 --> CS4
  vector<int> route_CS2 = {9, 18, 9}; // CS2 --> CS3 | CS2 --> CS4 | CS2 --> CS1
  vector<int> route_CS3 = {9, 18, 9}; // CS3 --> CS4 | CS3 --> CS1 | CS3 --> CS2
  vector<int> route_CS4 = {9, 18, 9}; // CS4 --> CS1 | CS4 --> CS2 | CS4 --> CS3

  ChargingStation();
  ChargingStation(int id, int xcor, int ycor, string zone);
};