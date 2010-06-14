#include "Solution.h"
using namespace std;

// Construye una solución inicial
Solution::Solution(vector<int> &rlength, 
                   vector<int> &lpiece,
                   vector<int> &dpiece) {
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

