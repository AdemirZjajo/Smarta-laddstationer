#include "node.hpp"
#include "communication.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

enum State
{
    TRANSIT,
    QUEUE,
    CHARGE
};

using namespace std;

bool activeMission = false;
State state = TRANSIT;                  // Starttillståndet
Node node(static_cast<float>(getID())); // Type casta integern för nod id till en float för att kunna användas i en 2d vektor i noden

int main()
{
    switch (state)
    {
    case TRANSIT:

        if (node.battery_charge >= node.min_charge) // Jämföra batterinivå med minimumladdning, för att stanna kvar i transit
        {
            /* FÖRLYTTNINGS LOOP */
            /* Iteration med tidsfördröjning */
            int steps = node.calcStepsNeeded(node.current_mission);
            for (int i = 0; i < steps; i++)
            {
                // Chilla 1 sekund
                this_thread::sleep_for(chrono::seconds(1));

                // Optional: Display iteration number
                cout << "Iteration " << i + 1 << " completed." << endl;
                node.battery_charge -= node.battery_consumption;
                /* UPPDATERA STATUS-FUNKTION TILL OLED */
            }
            node.xcor = node.current_mission.missionDestination.xcor;
            node.ycor = node.current_mission.missionDestination.ycor;

            // Nod framme
            if ((node.xcor == node.current_mission.missionDestination.xcor) && (node.ycor == node.current_mission.missionDestination.ycor))
            {
                node.current_mission = node.generateMission(node.current_mission.missionOrigin);      // Generera nytt uppdrag med last, typ av last, och förbrukning
                node.battery_consumption = node.calcBatConsumption(node.current_mission);             // Beräkna batteriförbrukning baserat på upppdrag
                node.min_charge = node.calcMinCharge(node.battery_consumption, node.current_mission); // Beräkna minimumladdning baserat på uppdraget
            }
        }
        else
        { // Om batterinivån är lägre än minimumladdningen --> byta tillstånd till queue
            state = QUEUE;
            break;
        }
        break;

    case QUEUE:

        /* HÄR RÄKNAS KÖPOÄNG UT */
        /* HÄR DEFINERAS LADDSTATIONENS SPECIFIKA KÖLISTA(INSERT + SORT OSV...) */

        if (true) // Om den egna noden inte redan finns i sin egna lista:
        {
            node.vect.insert(node.vect.end(), {node.nod_id, node.queue_point}); // Lägger in nodens egna id och köpoäng i vektorn
        }

        sort(node.vect.begin(),
             node.vect.end(),
             [](const vector<float> &a, const vector<float> &b)
             {
                 return a[1] > b[1];
             });

        // OM: ingen annan nod är vid laddstationen; alltså att man inte är med i något meshnät --> byt tillstånd till CHARGE och börja ladda mot 100% (eftersom det inte finns någon annan i kö)
        // ANNARS OM: det finns någon annan i meshnätet, kommunicera med dem och skicka prioriteringspoäng för att bestämma vem som ska börja ladda --> den som ska börja byter tillstånd till CHARGE
        break;
    case CHARGE:
        // Om man kommer in i detta tillstånd ska man omedelbart börja ladda

        // In case of an emergency, quit charge
        // One second, one procent added to battery

        // OM: man är ensam på laddstationen laddar man mot 100%
        if (/* OM MAN ÄR ENSAM PÅ LADDSATIONEN OCH... */ node.battery_charge <= 100)
        {
            // Chilla 1 sekund
            this_thread::sleep_for(chrono::seconds(1));

            node.battery_charge++;
            /* UPPDATERA STATUS-FUNKTION TILL OLED */
        }

        // ANNARS OM: man inte var ensam, men har högst priopoäng, laddar man mot sin minimumladdning
        else if (/* OM MAN INTE ÄR ENSAM PÅ LADDSATIONEN OCH... */ node.battery_charge <= node.min_charge)
        {
            // Chilla 1 sekund
            this_thread::sleep_for(chrono::seconds(1));

            node.battery_charge++;
            /* UPPDATERA STATUS-FUNKTION TILL OLED */
        }

        // ANNARS OM: man laddar för nuvarande men någon annan har MYCKET högre priopoäng, eller av annan anledning får slänga ut dig --> Byt tillstånd till QUEUE
        else if (true)
        {
            state = QUEUE;
        }

        // ANNARS: nu har vi tillräckligt med laddning för att slutföra uppdraget --> Byt tillstånd till TRANSIT
        else
        {
            // TA BORT SIG SJÄLV FRÅN VEKTORLISTAN
            // Iterera genom vektorn av vektorer och sök efter den vektor som innehåller nodens egna nod_id
            // Använd sedan vector.erase(...) för att ta bort vektorn

            /* UPPDATERA STATUS-FUNKTION TILL OLED */
            state = TRANSIT;
        }

        break;
    }
    return 0;
}

/* Transit state
Minus en procent batterinivå i slutet
*/