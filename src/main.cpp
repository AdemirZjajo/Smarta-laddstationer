#include "node.hpp"
#include "communication.hpp"

enum State
{
    TRANSIT,
    QUEUE,
    CHARGE
};

using namespace std;

int main()
{
    Node node(getID());
    State state = TRANSIT; // Starttillståndet ska bestämmas utifrån

    switch (state)
    {
    case TRANSIT:

        // generera nytt uppdrag med last, typ av last, och förbrukning
       // node.generateMission(node.currentMission.missionOrigin, node.currentMission.missionDestination);

        // beräkna minimumladdning baserat på uppdraget
        // antalet element i aktuell rutt * förbrukning = minimumladdning
       // node.calcMinCharge();

        // jämföra batterinivå med minimumladdning
        // om batterinivån är högre än minimumladdningen börjar noden förflytta sig till sin uppdragsdestination
        // stanna kvar i transit och börja förflytting
        // om batterinivån är lägre än minimumladdningen --> byta tillstånd till queueu
        // One second, one procent subtracted to battery
        break;
    case QUEUE:
        // if no one, charge
        // if at least one, communicate and prioritize queue
        break;
    case CHARGE:
        // in case of an emergency, quit charge
        // One second, one procent added to battery
        break;
    }
    return 0;
}

/* Transit state
Minus en procent batterinivå i slutet
*/