#include "node.hpp"
#include "communication.hpp"
#include <iostream>
#include <chrono>
#include <thread>

enum State
{
    TRANSIT,
    QUEUE,
    CHARGE
};

using namespace std;

int missionCounter = 0;

int main()
{
    Node node(getID());
    State state = TRANSIT; // Starttillståndet ska bestämmas utifrån

    switch (state)
    {
    case TRANSIT:

        if (missionCounter > 1)
        {
            node.current_mission = node.generateMission(node.currentMission.missionOrigin); // generera nytt uppdrag med last, typ av last, och förbrukning
            node.battery_consumption = calcBatConsumption(node.current_mission);            // beräkna batteriförbrukning baserat på upppdrag
            node.calcMinCharge(node.battery_consumption, node.current_mission);             // beräkna minimumladdning baserat på uppdraget
        }
        missionCounter++;

        if (node.battery_charge >= node.min_charge) // jämföra batterinivå med minimumladdning, för att stanna kvar i transit
        {
            /* FÖRLYTTNINGS LOOP */
            /* Iteration med tidsfördröjning */
            int steps = node.calcStepsNeeded();
            for (int i = 1; i < steps; i++)
            {
                // Chilla 1 sekund
                this_thread::sleep_for(chrono::seconds(1));

                // Optional: Display iteration number
                cout << "Iteration " << i + 1 << " completed." << endl;
            }

            if ((node.xcor == node.current_mission.missionDestination.xcor) && (node.ycor == node.current_mission.missionDestination.ycor))
            {
                // Nod framme
            }
        }
        else
        { // om batterinivån är lägre än minimumladdningen --> byta tillstånd till queue
            state = QUEUE;
            break;
        }

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