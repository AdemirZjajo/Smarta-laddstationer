#include <string>
//randomize function in this class and show on display
//Prio
//State
//Communication
//Display

/*using namespace std;

class Nod {
  public: // Accesstyp
    int batterinivå;
    float batteriförbrukning; // Hur många procentenheter batteri som noden kräver för att förflytta sig ett steg i x-led eller y-led
                              // Uppdateras dynamiskt under uppdragsgivande

    int xcor; // Nodens x-koordinat
    int ycor; // Nodens y-koordinat
    const string zon; // Kan anta värden laddstation 1-3 samt ett default värde, kanske "0" eller bara "default"

    // En lista med objekt av laddsationerna som i sin tur innehåller sitt namn (eg. ls_1) och sina paths till andra laddstationer
    Laddstation1 LS1;
    Laddstation2 LS2;
    Laddstation3 LS3;
    Laddstation4 LS4;
    list<int> laddstationer{LS1, LS2, LS3, LS4};



    // Nodens initieringsprocess börjar här
    /* Destinationsgenerering och uträkning av miniladdning görs en gång innan
    state machinen för att låta noden bestämma vilket dess första state ska bli. 


};

int main() {
  string state = "transit"; // Starttillståndet ska bestämmas utifrån  
  switch (state) {
  case "transit":
    // generera nytt uppdrag med last, typ av last, och förbrukning
    // beräkna minimumladdning baserat på uppdraget 
        // antalet element i aktuell rutt * förbrukning = minimumladdning
    // jämföra batterinivå med minimumladdning
    // om batterinivån är högre än minimumladdningen börjar noden förflytta sig till sin uppdragsdestination
    // stanna kvar i transit och börja förflytting
    // om batterinivån är lägre än minimumladdningen --> byta tillstånd till queueu
    //One second, one procent subtracted to battery
    break;
  case "queue":
  // if no one, charge
  // if at least one, communicate and prioritize queue
    break;
  case "charge":
  //in case of an emergency, quit charge
  //One second, one procent added to battery
    break;
  }
  return 0;
}*/



/* Transit state
Minus en procent batterinivå i slutet 
*/