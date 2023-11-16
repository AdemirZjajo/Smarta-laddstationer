#include <string>

int xcor // Nodens x-koordinat
int ycor; // Nodens y-koordinat
const string zon; // Kan anta värden laddstation 1-3 samt ett default värde, kanske "0" eller bara "default"

// En lista med objekt av laddsationerna som i sin tur innehåller sitt namn (eg. ls_1) och sina paths till andra laddstationer


// Nodens initieringsprocess börjar här
/* Destinationsgenerering och uträkning av miniladdning görs en gång innan
state machinen för att låta noden bestämma vilket dess första state ska bli. */




int main() {
  string state = "transit"; // Starttillståndet ska bestämmas utifrån  
  switch (state) {
  case "transit":
    break;
  case "queue":
    break;
  case "charge":
    break;
  }
  return 0;
}


// Transit state
//