#pragma once
#include <string>
#include <vector>

using namespace std;

struct Coordinate
{
  int x;
  int y;
};

class ChargingStation
{

public:
  int id;
  int xcor;
  int ycor;
  string zone;

  ChargingStation();
  ChargingStation(int id, int xcor, int ycor, string zone);

  private:
  // Routes from CS1
  vector<Coordinate> CS1_CS2 = {{1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9}, {6, 9}, {7, 9}, {8, 9}, {9, 9}};
  vector<Coordinate> CS1_CS3 = {{0, 8}, {0, 7}, {0, 6}, {0, 5}, {0, 4}, {0, 3}, {0, 2}, {0, 1}, {0, 0}};
  vector<Coordinate> CS1_CS4 = {{1, 9}, {1, 8}, {2, 8}, {2, 7}, {3, 7}, {3, 6}, {4, 6}, {4, 5}, {5, 5}, {5, 4}, {6, 4}, {6, 3}, {7, 3}, {7, 2}, {8, 2}, {8, 1}, {9, 1}, {9, 0}};

  // Routes from CS2
  vector<Coordinate> CS2_CS1 = {{8, 9}, {7, 9}, {6, 9}, {5, 9}, {4, 9}, {3, 9}, {2, 9}, {1, 9}, {0, 9}};
  vector<Coordinate> CS2_CS4 = {{9, 8}, {9, 7}, {9, 6}, {9, 5}, {9, 4}, {9, 3}, {9, 2}, {9, 1}, {9, 0}};
  vector<Coordinate> CS2_CS3 = {{9, 8}, {8, 8}, {8, 7}, {7, 7}, {7, 6}, {6, 6}, {6, 5}, {5, 5}, {5, 4}, {4, 4}, {4, 3}, {3, 3}, {3, 2}, {2, 2}, {2, 1}, {1, 1}, {1, 0}, {0, 0}};

  // Routes from CS3
  vector<Coordinate> CS3_CS1 = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}};
  vector<Coordinate> CS3_CS4 = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}};
  vector<Coordinate> CS3_CS2 = {{0, 1}, {1, 1}, {1, 2}, {2, 2}, {2, 3}, {3, 3}, {3, 4}, {4, 4}, {4, 5}, {5, 5}, {5, 6}, {6, 6}, {6, 7}, {7, 7}, {7, 8}, {8, 8}, {8, 9}, {9, 9}};

  // Routes from CS4
  vector<Coordinate> CS4_CS3 = {{8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}};
  vector<Coordinate> CS4_CS2 = {{9, 1}, {9, 2}, {9, 3}, {9, 4}, {9, 5}, {9, 6}, {9, 7}, {9, 8}, {9, 9}};
  vector<Coordinate> CS4_CS1 = {{8, 0}, {8, 1}, {7, 1}, {7, 2}, {6, 2}, {6, 3}, {5, 3}, {5, 4}, {4, 4}, {4, 5}, {3, 5}, {3, 6}, {2, 6}, {2, 7}, {1, 7}, {1, 8}, {0, 8}, {0, 9}};

  public:
  // CS1 coords: 0, 9 (x, y)
  // Input: 0 is to CS2, 1 is to CS3, 2 is to CS4
  vector<vector<Coordinate>> routes_CS1 = {CS1_CS2, CS1_CS3, CS1_CS4};
  // CS2 coords: 9, 9 (x, y)
  // Input: 0 is to CS1, 1 is to CS4, 2 is to CS3
  vector<vector<Coordinate>> routes_CS2 = {CS2_CS1, CS2_CS4, CS2_CS3};
  // CS3 coords: 0, 0 (x, y)
  // Input: 0 is to CS1, 1 is to CS4, 2 is to CS2
  vector<vector<Coordinate>> routes_CS3 = {CS3_CS1, CS3_CS4, CS3_CS2};
  // CS4 coords: 9, 0 (x, y)
  // Input: 0 is to CS3, 1 is to CS2, 2 is to CS1
  vector<vector<Coordinate>> routes_CS4 = {CS4_CS3, CS4_CS2, CS4_CS1};
};
