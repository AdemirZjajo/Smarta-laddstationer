<<<<<<< Updated upstream


class ChargingStation {
  public: // Accesstyp
};
/*int main() {
  string state = ; // vilken laddstaionsrutt vi ska välja, beroende på vilket uppdrag noden har när den kallar på funktionen
  switch (state) {
  case "till_LS_2":
    return // returna listan för denna rutt
    break;
  case "till_LS_3":
    break;
  case "till_LS_4":
    break;
  }
  return 0;
}*/
=======
#include "chargingStation.hpp"
#include <vector>

using namespace std;
>>>>>>> Stashed changes

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