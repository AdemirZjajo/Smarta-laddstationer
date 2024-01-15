#include "node.hpp"
#include "communication.hpp"
#include "display.hpp"
#include "mission.hpp"
#include "message.hpp"
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

Node node(0);
State state = TRANSIT; // Starttillståndet
Message message;
string statusMessage;
using namespace std;

// Metod som:
// 1. Rensar gamla nodlister i meshnätet
// 2. Uppdaterar kommunikationen
// 3. Hämtar den kontinuerligt uppdaterade kölistan från kommunikationen, samt sorterar den
/*void updateQueue() // ANVÄNDS EJ!
{
    // removeMissingNodes();
    // updateCommunication();

    // Uppdaterar nodens kölista

    // Sorterar listan
    sort(getComQueueVector().begin(),
         getComQueueVector().end(),
         [](const vector<float> &a, const vector<float> &b)
         {
             return a[1] > b[1];
         });

    // Skriver ut kölistan för debugging
    cout << "--KÖLISTA--" << endl;
    for (const auto &row : getComQueueVector())
    {
        for (const auto &element : row)
        {
            cout << element << ' ';
        }
        cout << '\n';
    }
}*/

// Returnerar true om den enda noden som finns i listan är sig själv, annars false; det finns även andra noder i listan
bool isAlone()
{
    bool isAlone = false;

    if (node.zone == "LADDSTATION_1")
    {
        if (getComQueueVectorCS1().size() == 1)
        {

            if (getComQueueVectorCS1()[0][0] == node.node_id)
            {
                isAlone = true;
            }
            else
                isAlone = false;
        }
        return isAlone;
    }

    else if (node.zone == "LADDSTATION_2")
    {
        if (getComQueueVectorCS2().size() == 1)
        {

            if (getComQueueVectorCS2()[0][0] == node.node_id)
            {
                isAlone = true;
            }
            else
                isAlone = false;
        }
        return isAlone;
    }

    else if (node.zone == "LADDSTATION_3")
    {
        if (getComQueueVectorCS3().size() == 1)
        {

            if (getComQueueVectorCS3()[0][0] == node.node_id)
            {
                isAlone = true;
            }
            else
                isAlone = false;
        }
        return isAlone;
    }

    else if (node.zone == "LADDSTATION_4")
    {
        if (getComQueueVectorCS4().size() == 1)
        {

            if (getComQueueVectorCS4()[0][0] == node.node_id)
            {
                isAlone = true;
            }
            else
                isAlone = false;
        }
        return isAlone;
    }

    else
    {
        cout << "isAlone() kunde inte matcha nodens zon" << endl;
        return isAlone;
    }
}

bool isFirst()
{
    bool isFirst = false;

    if (node.zone == "LADDSTATION_1")
    {
        if (getComQueueVectorCS1()[0][0] == node.node_id)
        {
            isFirst = true;
            return isFirst;
        }
        return isFirst;
    }

    else if (node.zone == "LADDSTATION_2")
    {

        if (getComQueueVectorCS2()[0][0] == node.node_id)
        {
            isFirst = true;
            return isFirst;
        }
        return isFirst;
    }

    else if (node.zone == "LADDSTATION_3")
    {

        if (getComQueueVectorCS3()[0][0] == node.node_id)
        {
            isFirst = true;
            return isFirst;
        }
        return isFirst;
    }

    else if (node.zone == "LADDSTATION_4")
    {

        if (getComQueueVectorCS4()[0][0] == node.node_id)
        {
            isFirst = true;
            return isFirst;
        }
        return isFirst;
    }

    else
    {
        cout << "isFirst() kunde inte matcha nodens zon" << endl;
        return isAlone;
    }
}

string makeStatusString(Node node)
{

    string state = node.state;
    string nodeID = to_string(node.node_id);
    string battery = to_string(node.battery_charge);
    string batteryCon = to_string(node.battery_consumption);
    string minCharge = to_string(node.min_charge);
    string queuePoint = to_string(node.queue_point);
    string zone = node.zone;
    string x = to_string(node.xcor);
    string y = to_string(node.ycor);
    string orgin = node.current_mission.missionOrigin.zone;
    string dest = node.current_mission.missionDestination.zone;
    string freight = to_string(node.current_mission.last);
    string coldFreight = node.current_mission.kylvara ? "true" : "false";

    // Creating a single string with commas between values
    string statusMsg = nodeID + "," + battery + "," + batteryCon + "," +
                       minCharge + "," + queuePoint + "," + zone + "," +
                       x + "," + y + "," + orgin + "," + dest + "," +
                       freight + "," + coldFreight;

    return statusMsg;
}

void setup()
{
    cout << "SETUP START..." << endl;
    initCOM();
    setupDIS();
    node.node_id = getID();
    cout << "Nod-" << node.node_id << " har spawnat på: " << node.current_CS.zone << endl;
    cout << "Noden har " << node.battery_charge << "% batteri" << endl;
    cout << "Nodens första destination är: " << node.current_mission.missionDestination.zone << endl;
    cout << "Noden har " << node.current_mission.last << " ton i last. Kylvara? " << boolalpha << node.current_mission.kylvara << endl;
    cout << "Minimumladdning: " << node.min_charge << "%, uträkning: " << node.calcStepsNeeded(node.current_mission) << " * " << node.battery_consumption << endl;

    // HÄR DEFINIERAS DISPLAYEN för första gången med TRANSIT A och B sida
    setID(getID());
    setBat(node.battery_charge);
    setWeight(node.current_mission.last);
    setLoadType(node.current_mission.kylvara);
    randomSeed(analogRead(A0));
    cout << "SETUP FINISHED!" << endl;
}

void loop()
{
    updateCommunication();

    switch (state)
    {
    case TRANSIT:
        cout << "** NOD är i TRANSIT-state **" << endl;
        //  OM: Batterinivån är högre än minimumladdning påbörjar noden sitt uppdrag
        if (node.battery_charge >= node.min_charge)
        {
            node.zone = "Transit-zone";
            cout << "Noden ger sig iväg mot sin destination: " << node.current_mission.missionDestination.zone << endl;
            vector<Coordinate> route = node.chooseRoute(node.current_mission);
            for (const Coordinate &destination : route)
            {
                updateCommunication();
                this_thread::sleep_for(chrono::milliseconds(1000));
                node.battery_charge = (node.battery_charge - node.battery_consumption); // vi minskar batteri % för att simulera att vi rör oss framåt
                node.xcor = destination.x;
                node.ycor = destination.y;
                cout << "Nod-Förlyttning: x: " << destination.x << ", y: " << destination.y << " | Batterinivå: " << node.battery_charge << "%" << endl;
                // UPPDATERA BATTERI STATUS-FUNKTION TILL OLED

                setID(node.node_id);
                setBat(node.battery_charge);
                position(node.xcor, node.ycor);
                setLoadType(node.current_mission.kylvara);

                statusMessage = makeStatusString(node);
                sendStatus(statusMessage);
            }

            // Nod framme
            if ((node.xcor == node.current_mission.missionDestination.xcor) && (node.ycor == node.current_mission.missionDestination.ycor))
            {
                node.zone = node.current_mission.missionDestination.zone; // Sätt zonen till den nya startplatsen, alltså det förra uppdragets destination
                cout << "Noden har nått sin destination, X: " << node.xcor << " Y: " << node.ycor << endl;

                node.current_mission = node.generateMission(node.current_mission.missionDestination); // Generera nytt uppdrag, skicka in nuvarande plats
                node.current_CS = node.current_mission.missionOrigin;
                node.battery_consumption = node.calcBatConsumption(node.current_mission);                                   // Beräkna batteriförbrukning baserat på uppdrag
                node.min_charge = node.calcMinCharge(node.battery_consumption, node.calcStepsNeeded(node.current_mission)); // Beräkna minimumladdning
                cout << "Noden får nytt uppdrag: " << node.current_mission.missionDestination.zone << " med lasten: " << node.current_mission.last << " ton i last. Kylvara? " << boolalpha << node.current_mission.kylvara << endl;
                cout << "Minimumladdning: " << node.min_charge << "%, uträkning: " << node.calcStepsNeeded(node.current_mission) << " * " << node.battery_consumption << endl;
                node.queue_point = calculatePriority(node.battery_charge, node.min_charge, node.current_mission); // randomNumber(1, 100); //  // Beräknar nodens köpoäng
                cout << "Köpoäng: " << node.queue_point << " | Bat: " << node.battery_charge << " | Min: " << node.min_charge << " | Last: " << node.current_mission.last << " | Kylvara: " << node.current_mission.kylvara << endl;
            }
        }
        // ANNARS: Noden har inte tillräckligt mycket batteri för att slutföra sitt uppdrag, och måste ladda --> Byt tillstånd till QUEUE
        else
        {
            cout << "Noden behöver ladda batteriet. Eftersom batteristatus är: " << node.battery_charge << "% men uppdraget kräver: " << node.min_charge << "%" << endl;
            message = Message(1, "AddToQueue", ++node.messageID, node.node_id, node.queue_point, node.zone);
            sendMessage(message);
            addSelfToQueue(message);
            sortQueue();
            state = QUEUE;
            node.state = "queue";
            cout << "** NOD är i QUEUE-state **" << endl;
            break;
        }
        break;

    case QUEUE:

        statusMessage = makeStatusString(node);
        sendStatus(statusMessage);
        cout << "Nod-" << node.node_id << " köar för att få ladda..." << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));

        updateCommunication();
        printQueueVector();

        // OM: Ingen annan nod är vid laddstationen; alltså att man är den enda noden i ens egna kölista --> byt tillstånd till CHARGE och börja ladda mot 100%
        //     Noden vill alltid ladda så högt som möjligt i förebyggande syfte; kanske det är många som ska ladda på nästa laddstation
        //     Dock är det osannolikt att den kommer ladda mot 100% i vårt system eftersom det är många noder på få laddstationer
        if (isAlone())
        {
            state = CHARGE;
            node.state = "charge";
        }

        // ANNARS OM: det finns någon annan i meshnätet, kommunicera med dem och skicka prioriteringspoäng för att bestämma vem som ska börja ladda --> den som ska börja byter tillstånd till CHARGE
        else if (!isAlone())
        {

            if (isFirst()) // Kollar om noden är först i kön, om den är det får den börja ladda
            {
                cout << "Din tur att ladda Nod-" << node.node_id << " du är först i kön!" << endl;
                state = CHARGE;
                node.state = "charge";
                cout << "** NOD är i CHARGE-state **" << endl;
            }
        }

        setID(node.node_id);
        setBat(node.battery_charge);
        location(node.current_CS.id);
        queuePoints(node.queue_point);

        break;

    case CHARGE:

        statusMessage = makeStatusString(node);
        sendStatus(statusMessage);
        //  Uppdaterar listan för att säkerställa att noden fortfarande är först i kön
        //  Nodens egna köpoäng kommer inte förändras under laddning, men det kan komma in andra
        //  noder med högre köpoäng som ska kunna "slänga ut" den nuvarande laddande noden

        setID(node.node_id);
        setBat(node.battery_charge);
        location(node.current_CS.id);
        loading();

        updateCommunication();
        sortQueue();
        // printQueueVector();

        //  OM: man är ensam på laddstationen laddar man mot 100%
        if (isAlone() && node.battery_charge < 100)
        {
            this_thread::sleep_for(chrono::milliseconds(100)); // Slöa ner programmet; det tar ju faktiskt tid att ladda
            if (node.battery_charge >= 99)
            {
                node.battery_charge = 100;
            }
            else
            {
                node.battery_charge++; // Öka batterinivån
            }

            cout << "NOD LADDAR till max_charge... " << node.battery_charge << "% på LS: " << node.current_CS.id << endl; // För debugging
        }

        // ANNARS OM: man inte är ensam, och har högst priopoäng, laddar man mot sin minimumladdning
        else if (!isAlone() && isFirst() && node.battery_charge <= node.min_charge) // Otestat, har lagt till "&& (getComQueueVector()[0][0] == node.node_id)" -Simon
        {
            //  Slöa ner programmet; det tar ju faktiskt tid att ladda
            this_thread::sleep_for(chrono::milliseconds(200));

            node.battery_charge++; // Öka batterinivån
            updateCommunication();
            cout << "NOD LADDAR till min_charge... " << node.battery_charge << "% på LS: " << node.current_CS.id << endl;
        }

        // ANNARS OM: man laddar för nuvarande men någon annan har MYCKET högre priopoäng, eller av annan anledning får slänga ut dig --> Byt tillstånd till QUEUE
        // JUST NU slängs noden ut från laddning så fort någon annan har högre köpoäng

        else if (!isAlone() && !isFirst() && node.battery_charge <= node.min_charge)
        {
            cout << "Noden har blivit utslängd av en annan med högre köpoäng." << endl; // För debugging
            state = QUEUE;
            node.state = "queue";
        }

        // ANNARS: nu har vi tillräckligt med laddning för att slutföra uppdraget --> Byt tillstånd till TRANSIT
        else
        {
            cout << "Nod-" << node.node_id << " har laddat klart och är nu redo för sitt uppdrag!" << endl;
            // Skickar ett meddelande till de andra noderna vid laddstationen när man har laddat klart och att man ska tas bort från deras kölistor
            // Därefter raderar noden sin egna kölista
            message = Message(1, "RemoveFromQueue", ++node.messageID, node.node_id, node.queue_point, node.zone);
            sendMessage(message);
            eraseSelfFromQueue(message);
            sortQueue();
            printQueueVector();

            state = TRANSIT;
            node.state = "transit";
        }

        break;
    }
}