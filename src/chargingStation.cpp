#include <string>

class Laddstation1 {
  public: // Accesstyp
};

int main() {
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
}

// rutter från denna specifika laddstation till de tre andra, dessa kan skapas som listor eller
// arrayer av koordinater för hur noden ska iterera sin förflyttning
//hej