#include "node.hpp"
#include "communication.hpp"
#include "display.hpp"
#include "mission.hpp"
#include "priorityAlgorithm.hpp"
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

Node node(0);          // Type casta integern för nod id till en float för att kunna användas i en 2d vektor i noden
State state = TRANSIT; // Starttillståndet
String CurrentZon;
using namespace std;

// Metod som hämtar den kontinuerligt uppdaterade kölistan från kommunikationen, samt sorterar den
void updateQueue()
{
    // Uppdaterar nodens kölista
    node.queueVector = getComQueueVector();

    // Sorterar listan
    sort(node.queueVector.begin(),
         node.queueVector.end(),
         [](const vector<float> &a, const vector<float> &b)
         {
             return a[1] > b[1];
         });
}

// Returnerar true om den enda noden som finns i listan är sig själv, annars false; det finns även andra noder i listan
bool isAlone()
{
    bool isAlone = false;
    if (node.queueVector.size() == 1)
    {
        if (node.queueVector[0][0] = node.node_id)
        {
            isAlone = true;
        }
        else
            isAlone = false;
    }
    return isAlone;
    // Otestad ny metod som bör vara mer säker än den övre -Simon
    /*
        bool isAlone = true;

        // Loopar genom queueVector och letar efter andra noder...
        for (const auto &vect : node.queueVector)
        {
            // ...om den hittar någon annan nod sätts isAlone till false; det är andra i listan
            if (!vect.empty() && vect[0] != node.node_id)
            {
                isAlone = false;
            }
        }

        return isAlone;
        */
}

void setup()
{
    cout << "SETUP START" << endl;
    initCOM();
    setupDIS();
    node.node_id = getID();
    cout << "Nod-" << node.node_id << " har spawnat på: " << node.current_CS.zone << endl;
    cout << "Noden har " << node.battery_charge << "% batteri" << endl;
    cout << "Nodens första destination är: " << node.current_mission.missionDestination.zone << endl;
    cout << "Noden har " << node.current_mission.last << " ton i last. Kylvara? " << boolalpha << node.current_mission.kylvara << endl;
    cout << "Noden har " << node.battery_consumption << " batteriförbrukning" << endl;

    /* HÄR DEFINIERAS DISPLAYEN för första gången med TRANSIT A och B sida
    Display display();
    //A-SIDA
    display.setID(node.node_id);
    display.setBat(node.battery_charge)
    display.position(node.xcor, node.ycor);
    display.destination(node.current_mission.missionDestination);
    //B-SIDA
    display.load(node.current_mission.last);
    display.loadType(node.current_mission.kylvara);
    */
}

void loop()
{
    if (node.node_id == 0)
    {
        getID();
        setID(node.node_id);
    }

    // displayClear();
    setID(getID());
    setBat(node.battery_charge);
    // position(node.xcor, node.ycor);
    setWeight(node.current_mission.last);
    setLoadType(node.current_mission.kylvara);

    // displayLooping(node.node_id);

    updateCommunication(); // Utflyttat från QUEUE

    switch (state)
    {
    case TRANSIT:
        cout << "** NOD är i Transit-state **" << endl; // För debugging
        // OM: Batterinivån är högre än minimumladdning påbörjar noden sitt uppdrag
        if (node.battery_charge >= node.min_charge)
        {
            node.zone = "Transit-zone";
            cout << "Noden ger sig iväg mot sin destination: " << node.current_mission.missionDestination.zone << endl;

            // FÖRLYTTNINGS LOOP
            // Iteration med tidsfördröjning
            // int steps = 10;
            int steps = 10; // node.calcStepsNeeded(node.current_mission);
            for (int i = 1; i < steps; i++)
            {
                // Chilla 1 sekund
                // iden är att ett steg tar 1 sec att gå (så vi pausar tråden och tror att de kommer funka)
                this_thread::sleep_for(chrono::seconds(1));

                // Optional: Display iteration number
                node.battery_charge = (node.battery_charge - node.battery_consumption); // vi minskar batteri % för att simulera att vi rör oss framåt
                cout << "Nod-Förlyttning " << i << "/10. "
                     << "Nodens batteri status är nu: " << node.battery_charge << endl;
                // UPPDATERA BATTERI STATUS-FUNKTION TILL OLED
                displayClear();
                setID(node.node_id);
                setBat(node.battery_charge);
                setWeight(node.current_mission.last);
                setLoadType(node.current_mission.kylvara);
                // position(node.xcor, node.ycor); // Display saker
                // destination(node.current_mission.missionDestination.zone); // Display saker
            }
            // Dessa två rader finns just nu eftersom vi inte har någon riktig förflyttningskod, ska bort när den är skriven. If-satsen nedan bör funka även efter förflyttningskoden är skriven
            node.xcor = node.current_mission.missionDestination.xcor;
            node.ycor = node.current_mission.missionDestination.ycor;
            // position(node.xcor,node.ycor);

            // Nod framme
            if ((node.xcor == node.current_mission.missionDestination.xcor) && (node.ycor == node.current_mission.missionDestination.ycor))
            {
                node.zone = node.current_mission.missionDestination.zone;
                cout << "Noden har nått sin destination"
                     << " X: " << node.xcor << " Y: " << node.ycor << endl;
                node.current_mission = node.generateMission(node.current_mission.missionDestination); // Generera nytt uppdrag, skicka in nuvarande plats
                node.current_CS = node.current_mission.missionOrigin;
                node.battery_consumption = node.calcBatConsumption(node.current_mission); // Beräkna batteriförbrukning baserat på uppdrag
                node.min_charge = 40;
                // node.calcMinCharge(node.battery_consumption, node.calcStepsNeeded(node.current_mission));
                cout << "Noden får nytt uppdrag: " << node.current_mission.missionDestination.zone << " med lasten: " << node.current_mission.last << " ton i last. Kylvara? " << boolalpha << node.current_mission.kylvara << endl;
                cout << "Nodens förbrukning är nu: " << node.battery_consumption << endl;

                // node.min_charge = node.calcMinCharge(node.battery_consumption,node.calcStepsNeeded(node.current_mission));   // Beräkna minimumladdning baserat på uppdraget
                // display.destination(node.current_mission.missionDestination);
            }
        }
        // ANNARS: Noden har inte tillräckligt mycket batteri för att slutföra sitt uppdrag, och måste ladda --> Byt tillstånd till QUEUE
        else
        {
            cout << "Noden behöver ladda batteriet. Eftersom batteristatus är: " << node.battery_charge << " men uppdraget kräver: " << node.min_charge << endl;
            state = QUEUE;
            cout << "** NOD är i QUEUE-state **" << endl;
            break;
        }
        break;

    case QUEUE:
        // Bytar meshinställningar till de som gäller för det nuvarande uppdragets startladdstation
        // Gör det enbart möjligt för noden att kommunicera med de noder som är på samma laddstation
        cout << "TEST: " << node.current_CS.zone << endl;
        changeCS(node.current_CS.zone);

        // updateCommunication(); // Testar att flytta ut denna utanför switchen, känns som att det behövs för att kommunikationen ska fungera korrekt eller?

        // sendQ(node.node_id, node.queue_point); // Varje gång en nod kommer in i QUEUE skickar den sitt ID samt köpoäng till nätverket

        updateQueue();

        /* Ska nog tas bort, men har kvar eftersom jag är osäker -Simon
        for (size_t i = 0; i < 20; i++)
         {
             updateCommunication();
             sendQ(node.node_id, node.queue_point);
             this_thread::sleep_for(chrono::milliseconds(100));
         }
         */

        // Ska inte vara med i slutversionen, men för testning finns denna if-sats kvar
        /*
        if (node.queueVector[0][0] = node.node_id) //(!isAlone())//node.queueVector[0][0] = node.node_id
        {
            cout << "Nod-" << node.node_id << " är först i kön. Dags att börja ladda :)" << endl;
            state = CHARGE;
            cout << "** NOD är i Charge-state **" << endl;
        }
        */

        // OM: Ingen annan nod är vid laddstationen; alltså att man är den enda noden i ens egna kölista --> byt tillstånd till CHARGE och börja ladda mot 100%
        //     Noden vill alltid ladda så högt som möjligt i förebyggande syfte; kanske det är många som ska ladda på nästa laddstation
        //     Dock är det osannolikt att den kommer ladda mot 100% i vårt system eftersom det är många noder på få laddstationer
        if (isAlone())
        {
            state = CHARGE;
        }

        // ANNARS OM: det finns någon annan i meshnätet, kommunicera med dem och skicka prioriteringspoäng för att bestämma vem som ska börja ladda --> den som ska börja byter tillstånd till CHARGE
        else if (!isAlone())
        {
            node.queue_point = calculatePriority(node.battery_charge, node.min_charge); // Beräknar nodens köpoäng  // Avkommenterat 11:30 30/11 -Simon
            cout << "Nodens köpoäng är: " << node.queue_point << endl;                  // För debugging
            sendQ(node.node_id, node.queue_point);                                      // Noden skickar sitt ID samt köpoäng till nätverket så fort den vet att den inte är ensam på laddstationen
            updateQueue();                                                              // Uppdatera kölistan för säkerhets skull, i nästa steg börjar den ladda vilket noden inte ska göra om den inte är 100% säker på att den faktiskt får
            if (node.queueVector[0][0] = node.node_id)                                  // Kollar om noden är först i kön, om den är det får den börja ladda
            {
                cout << "Nod-" << node.node_id << " är först i kön. Dags att börja ladda :)" << endl;
                state = CHARGE;
                cout << "** NOD är i CHARGE-state **" << endl;
            }
        }

        // Skriver ut kölistan för debugging
        cout << "--KÖLISTA--" << endl;
        for (const auto &row : node.queueVector)
        {
            for (const auto &element : row)
            {
                cout << element << ' ';
            }
            cout << '\n';
        }

        // Display saker
        displayClear();
        setID(node.node_id);
        setBat(node.battery_charge);
        position(node.xcor, node.ycor);
        queuePoints(node.queue_point);
        this_thread::sleep_for(chrono::milliseconds(500)); // Finns för att artificiellt slöa ner programmet, annars blir saker oläsbara ibland
        break;

    case CHARGE:
        // Uppdaterar listan för att säkerställa att noden fortfarande är först i kön
        // Nodens egna köpoäng kommer inte förändras under laddning, men det kan komma in andra
        // noder med högre köpoäng som ska kunna "slänga ut" den nuvarande laddande noden
        updateQueue();

        // Display saker
        displayClear();
        setID(node.node_id);
        setBat(node.battery_charge);
        position(node.xcor, node.ycor);
        loading();

        //  OM: man är ensam på laddstationen laddar man mot 100%
        if (isAlone() && node.battery_charge < 100)
        {
            this_thread::sleep_for(chrono::milliseconds(200)); // Slöa ner programmet; det tar ju faktiskt tid att ladda
            if (node.battery_charge >= 99)
            {
                node.battery_charge = 100;
            }
            else
            {
                node.battery_charge++; // Öka batterinivån
            }

            // UPPDATERA STATUS-FUNKTION TILL OLED
            // display.setBat(node.battery_charge);
            cout << "NOD LADDAR... till max_charge " << node.battery_charge << "%" << endl; // För debugging
        }

        // ANNARS OM: man inte är ensam, och har högst priopoäng, laddar man mot sin minimumladdning
        else if (!isAlone() && (node.queueVector[0][0] == node.node_id) && node.battery_charge <= node.min_charge) // Otestat, har lagt till "&& (node.queueVector[0][0] == node.node_id)" -Simon
        {
            // Slöa ner programmet; det tar ju faktiskt tid att ladda
            this_thread::sleep_for(chrono::milliseconds(200));

            node.battery_charge++; // Öka batterinivån

            // UPPDATERA STATUS-FUNKTION TILL OLED
            // display.setBat(node.battery_charge);
            cout << "NOD LADDAR till min_charge... " << node.battery_charge << "%" << endl; // För debugging
        }

        // ANNARS OM: man laddar för nuvarande men någon annan har MYCKET högre priopoäng, eller av annan anledning får slänga ut dig --> Byt tillstånd till QUEUE
        // JUST NU slängs noden ut från laddning så fort någon annan har högre köpoäng
        else if (!isAlone() && (node.queueVector[0][0] != node.node_id) && node.battery_charge <= node.min_charge)
        {
            state = QUEUE;
        }

        // ANNARS: nu har vi tillräckligt med laddning för att slutföra uppdraget --> Byt tillstånd till TRANSIT
        else
        {
            // Skickar ett meddelande till de andra noderna vid laddstationen när man har laddat klart och att man ska tas bort från deras kölistor
            // Därefter raderar noden sin egna kölista
            sendRemove(node.node_id);
            node.queueVector.clear();
            clearComVector();

            printf("DISCONNECTION\n");
            delay(2000);
            disconnect();

            // UPPDATERA STATUS-FUNKTION TILL OLED
            // display.clearArea();
            // display.destination(node.current_mission.missionDestination);
            /*node.queueVector.erase(node.queueVector.begin() + 0);
            cout << "--KÖLISTA--" << endl;
            for (const auto &row : node.queueVector)
            {
                for (const auto &element : row)
                {
                    cout << element << ' ';
                }
                cout << '\n';
            }*/
            state = TRANSIT;
        }

        break;
    }
}

// används ej just nu :)
float getFirstValue(const vector<vector<float>> &queue)
{
    // Check if the vector of vectors is not empty
    if (!queue.empty())
    {
        // Check if the first element is not empty
        if (!queue[0].empty())
        {
            // Return the first value of the first element
            return queue[0][0];
        }
        else
        {
            // Handle the case where the first element is empty
            cerr << "Error: First element is empty" << endl;
        }
    }
    else
    {
        // Handle the case where the vector of vectors is empty
        cerr << "Error: Vector of vectors is empty" << endl;
    }

    // Return a default value or handle the error accordingly
    return 0.0f; // Adjust the default value as needed
}
