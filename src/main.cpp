#include "node.hpp"
#include "communication.hpp"
#include "display.hpp"
#include "mission.hpp"
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

Node node(0); // Type casta integern för nod id till en float för att kunna användas i en 2d vektor i noden
State state = TRANSIT;
bool activeMission = false; // Starttillståndet

using namespace std;

void setup()
{
    // cout << "SETUP START" << endl;
    initCOM();
    setupDIS();

    /* HÄR DEFINIERAS DISPLAYEN för första gången med TRANSIT A och B sida
    Display display();
    //A-SIDA
    display.setID(node.nod_id);
    display.setBat(node.battery_charge);
    display.position(node.xcor, node.ycor);
    display.destination(node.current_mission.missionDestination);
    //B-SIDA
    display.load(node.current_mission.last);
    display.loadType(node.current_mission.kylvara);
    */
}

void loop()
{
    if (node.nod_id == 0)
    {
        node.nod_id = getID();
    }

    displayClear();
    setID(node.nod_id);
    setBat(node.battery_charge);
    setWeight(node.current_mission.last);
    setLoadType(node.current_mission.kylvara);

    // displayLooping(node.nod_id);
    updateCommunication();

    switch (state)
    {
    case TRANSIT:
        cout << "NOD i Transit" << endl;
        if (node.battery_charge >= node.min_charge) // Jämföra batterinivå med minimumladdning, för att stanna kvar i transit
        {
            cout << "Nodens Batteri är tillräckligt för resan" << endl;

            // FÖRLYTTNINGS LOOP
            // Iteration med tidsfördröjning
            int steps = 10; // node.calcStepsNeeded(node.current_mission);
            for (int i = 1; i < steps; i++)
            {
                // Chilla 1 sekund
                // iden är att ett steg tar 1 sec att gå (så vi pausar tråden och tror att de kommer funka)
                this_thread::sleep_for(chrono::seconds(1));

                // Optional: Display iteration number
                cout << "Nod Förlyttning " << i + 1 << " klar." << endl;
                node.battery_charge -= node.battery_consumption; // vi minskar beteri % för att simulera att vi rör oss framåt
                // UPPDATERA BATTERI STATUS-FUNKTION TILL OLED
                displayClear();
                setID(node.nod_id);
                setBat(node.battery_charge);
                setWeight(node.current_mission.last);
                setLoadType(node.current_mission.kylvara);
            }
            node.xcor = node.current_mission.missionDestination.xcor;
            node.ycor = node.current_mission.missionDestination.ycor;
            // display.position(node.xcor,node.ycor);

            // Nod framme
            if ((node.xcor == node.current_mission.missionDestination.xcor) && (node.ycor == node.current_mission.missionDestination.ycor))
            {
                cout << "NOD Framme På Destination" << endl;
                cout << "NOD Får Nytt Uppdrag" << endl;
                node.current_mission = node.generateMission(node.current_mission.missionOrigin); // Generera nytt uppdrag med last, typ av last, och förbrukning
                node.battery_consumption = node.calcBatConsumption(node.current_mission);        // Beräkna batteriförbrukning baserat på upppdrag
                node.min_charge = 10;
                // node.min_charge = node.calcMinCharge(node.battery_consumption,node.calcStepsNeeded(node.current_mission));   // Beräkna minimumladdning baserat på uppdraget
                //  display.destination(node.current_mission.missionDestination);
            }
        }
        else
        { // Om batterinivån är lägre än minimumladdningen --> byta tillstånd till queue
            cout << "i ELSE" << endl;
            state = QUEUE;
            break;
        }
        break;

    case QUEUE:
        cout << " NOD i Queue" << endl;
        // changeCS(node.current_mission.missionOrigin.zon);

        // HÄR RÄKNAS KÖPOÄNG UT

        // display.clearArea();
        // display.queuePoints(node.queue_point);

        /// HÄR DEFINERAS LADDSTATIONENS SPECIFIKA KÖLISTA(INSERT + SORT OSV...)

        /*if (false) // Om den egna noden inte redan finns i sin egna lista:
         {
             node.vect.insert(node.vect.end(), {static_cast<float>(node.nod_id), node.queue_point}); // Lägger in nodens egna id och köpoäng i vektorn
         }

         sort(node.vect.begin(),
              node.vect.end(),
              [](const vector<float> &a, const vector<float> &b)
              {
                  return a[1] > b[1];
              });
              */
        state = CHARGE; // TEMP skickar bara en till CHARGE direkt
        // OM: ingen annan nod är vid laddstationen; alltså att man inte är med i något meshnät --> byt tillstånd till CHARGE och börja ladda mot 100% (eftersom det inte finns någon annan i kö)
        // ANNARS OM: det finns någon annan i meshnätet, kommunicera med dem och skicka prioriteringspoäng för att bestämma vem som ska börja ladda --> den som ska börja byter tillstånd till CHARGE
        break;
    case CHARGE:
        cout << " NOD i Charge" << endl;

        // Om man kommer in i detta tillstånd ska man omedelbart börja ladda

        // In case of an emergency, quit charge
        // One second, one procent added to battery

        // display.clearArea();
        // display.loading();

        // OM: man är ensam på laddstationen laddar man mot 100%
        if (node.battery_charge < 100)
        {
            // Chilla 1 sekund
            this_thread::sleep_for(chrono::milliseconds(200));
            if (node.battery_charge >= 99)
            {
                node.battery_charge = 100;
            }
            else
            {
                node.battery_charge++;
            }

            cout << "NOD LADDAR... +1%" << endl;
            // UPPDATERA STATUS-FUNKTION TILL OLED
            // display.setBat(node.battery_charge);
        }

        // ANNARS OM: man inte var ensam, men har högst priopoäng, laddar man mot sin minimumladdning
        else if (node.battery_charge <= node.min_charge)
        {
            cout << "I KÖ FÖR ATT LADDA (ELSE IF)" << endl;
            // Chilla 1 sekund
            // this_thread::sleep_for(chrono::seconds(1));

            node.battery_charge++;
            // UPPDATERA STATUS-FUNKTION TILL OLED
            // display.setBat(node.battery_charge);
            cout << "Nod laddar ++batteri :) "
                 << " klar." << endl;
        }
        /*
                    // ANNARS OM: man laddar för nuvarande men någon annan har MYCKET högre priopoäng, eller av annan anledning får slänga ut dig --> Byt tillstånd till QUEUE
                    else if (true)
                    {
                        state = QUEUE;
                    }*/

        // ANNARS: nu har vi tillräckligt med laddning för att slutföra uppdraget --> Byt tillstånd till TRANSIT
        else
        {
            // TA BORT SIG SJÄLV FRÅN VEKTORLISTAN
            // Iterera genom vektorn av vektorer och sök efter den vektor som innehåller nodens egna nod_id
            // Använd sedan vector.erase(...) för att ta bort vektorn

            // UPPDATERA STATUS-FUNKTION TILL OLED
            // display.clearArea();
            // display.destination(node.current_mission.missionDestination);

            state = TRANSIT;
        }

        break;
    }
}