#include "utilities.h"
using namespace std;

/////////////////////////////////////////////////////////////
// Comparación de pares de la forma p(id_pieza, longitud). //
/////////////////////////////////////////////////////////////
bool comparePair(pair<int,int> a, pair<int,int> b) {
  return (a.second > b.second);
}


