#include "utilities.h"
using namespace std;


// Comparación de pares de la forma p(id_pieza, longitud).
bool comparePair(pair<int,int> a, pair<int,int> b) {
  return (a.second > b.second);
}

// Mide la calidad de un cutting group contra
// otro. Devuelve un booleano True si el cutting 
// group A es mejor que el B, Falso sino..
bool group_quality(vector<int> leftover, 
                   vector<int> used_rolls, 
                   pair<int, vector<int> > A, 
                   pair<int, vector<int> > B) {
  // Se mide criterio por número de rolls.
  if (used_rolls[A.first] < used_rolls[B.first])
    return true;
  else 
    if (used_rolls[A.first] == used_rolls[B.first]) {
      // Probar criterio de leftOver
      if (leftover[A.first] <= leftover[B.first])
        return true;
    }
    else  // Número de rolls en A es mayor que B
          // o leftover en A es más que en B
      return false;
}
