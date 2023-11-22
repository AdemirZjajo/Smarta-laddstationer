#include <string>
<<<<<<< Updated upstream


using namespace std;

class ChargingStation {
  private: // Accesstyp
    string name;
=======
#include <vector>

using namespace std;

class ChargingStation
{
public:
  int id;
  int xcor;
  int ycor;
  string zon;

  vector<int> route_CS1, route_CS2, route_CS3, route_CS4;

  ChargingStation();
  ChargingStation(int id, int xcor, int ycor, string zon);
>>>>>>> Stashed changes
};