#include "utilities.h"
#include "FFD.h"
using namespace std;


// Comparación de pares de la forma p(id_pieza, longitud).
bool comparePair(pair<int,int> a, pair<int,int> b) {
  return (a.second > b.second);
}

// Dado un movimiento evaluado por la función 
// shiftNeighbourhood, esta función decide si el
// movimiento permite minizar la cantidad de 
// desperdicios generados.
bool group_quality(int * info, vector<int> &leftover,
                   vector<int> &used_rolls) {
  if (info[4] < leftover[info[1]])
    return true;
  else {
    // Se compara si leftovers son iguales
    if (info[4] == leftover[info[1]]) {
      // Se compara por numero de rolls usados
      if (info[6] < used_rolls[info[1]])
        return true;
      else {
        if (info[6] == used_rolls[info[1]]) {
          // Se comparan los orígenes
          if (info[5] < used_rolls[info[0]])
            return true;
          else 
            return false;
        }
        return false;
      }
    }
    else 
      return false;
  }
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
                                vector<int> &used_rolls,
                                vector<int> &rollType) {
  int M = lpiece.size();
  vector<vector<int>*> cgroups; // Solucion
  int i;
  int j;
  int k;
  pair <int,int> target;
  pair <int,int> ffdresult;
  int minimum;
  int left = 0;
  vector<int>::iterator it;

  for(i = 0; i < M; i++)
    cgroups.push_back(new vector<int>(M,0));

  for(i = 0; i < M; i++) {
    vector<int> pieceSet(M,0);
    pieceSet[i] = dpiece[i];
    target.first = MAX_INT;
    target.second = -1;
    // Se prueba con todos los tipos de rolls 
    // para ver donde se acomoda mejor la n cantidad 
    // de piezas del tipo i.
    for(j = 0; j < rlenght.size(); j++) {
      ffdresult = FFD(rlenght[j], lpiece, pieceSet);
      minimum = min(target.first,ffdresult.first);
      if (target.first != minimum){
        target.first = minimum;
        target.second = j;
        left = ffdresult.second;
      }
    }
    used_rolls[i] = target.first;
    leftover[i] = left;
    rollType[i] = target.second;
    
    //cgroups[i] = &pieceSet;
    //cout << dpiece[i] <<"\n";
    cgroups[i]->at(i) = dpiece[i];
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

// Swap de posiciones en vector pieceSet


