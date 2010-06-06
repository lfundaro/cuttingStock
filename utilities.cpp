#include "utilities.h"
#include "FFD.h"
using namespace std;


// Comparación de pares de la forma p(id_pieza, longitud).
bool comparePair(pair<int,int> a, pair<int,int> b) {
  return (a.second > b.second);
}

// Mide la calidad de un cutting group contra
// otro. Devuelve un booleano True si el cutting 
// group A es mejor que el B, Falso sino..
bool group_quality(vector<int> &leftover, 
                   vector<int> &used_rolls, 
                   pair<int, vector<int> > &A, 
                   pair<int, vector<int> > &B) {
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

// Generador de solución inicial.
// Consiste en generar M cutting groups 
// donde M es la cantidad de tipos de piezas.
// En cada grupo se coloca un tipo de pieza sin 
// importar la cantidad de rolls que estas ocupen
vector<vector<int>*> genInitSol(vector<int> &rlenght, 
                                vector<int> &lpiece,
                                vector<int> &dpiece,
                                vector<int> &leftover,
                                vector<int> &used_rolls) {
  int ntpieces = lpiece.size();
  vector<vector<int>*> cgroups;
  int i;
  int j;
  int k;
  int target;
  pair <int, vector<vector<int>* > > ffdresult;
  vector<pair <int, vector<vector<int>* > > > partial;
  vector<int>::iterator it;
  for(i = 0; i < ntpieces; i++) 
    cgroups.push_back(new vector<int>(ntpieces,0));

  for(i = 0; i < ntpieces; i++) {
    vector<int> pieceSet(ntpieces,0);
    pieceSet[i] = dpiece[i];
    target = MAX_INT;
    for(j = 0; j < rlenght.size(); j++) {
      partial.push_back(FFD(rlenght[j], lpiece, pieceSet));
      target = min(target, partial[j].first);
    }
    used_rolls[i] = ;
    leftover[i] = leftOver(ffdresult.second, rlenght[i],
                           lpiece);
    
    for(j = 0; j < ffdresult.first; j++) {
      for(k = 0; k < ffdresult.second[j]->size(); k++) {
        if (ffdresult.second[j]->at(k) != 0) {
          cgroups[i]->at(k) = ffdresult.second[j]->at(k);
          break;
        }
      }
    }
    free_vector(ffdresult.second);
  }
  return cgroups;
}

// Función que Calcula mínimo
inline int min(int a, int b) {
  if (a < b) 
    return a;
  else
    return b;
}

