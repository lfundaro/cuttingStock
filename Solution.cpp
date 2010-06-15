#include "Solution.h"
using namespace std;

// Construye una solución inicial
Solution::Solution(vector<int> &rlength, 
                   vector<int> &lpiece,
                   vector<int> &dpiece) {
  int M = lpiece.size();
  size = M;
  int i;
  int j;
  int k;
  pair <int,int> target;
  pair <int,int> ffdresult;
  int minimum;
  int left = 0;
  vector<int>::iterator it;
  
  for(i = 0; i < M; i++)
    cgs.push_back(vector<int>(M,0));
  
  for(i = 0; i < M; i++) {
    vector<int> pieceSet(M,0);
    pieceSet[i] = dpiece[i];
    target.first = MAX_INT;
    target.second = -1;
    // Se prueba con todos los tipos de rolls 
    // para ver donde se acomoda mejor la n cantidad 
    // de piezas del tipo i.
    for(j = 0; j < rlength.size(); j++) {
      if (rlength[j] < lpiece[i]) continue;
      ffdresult = FFD(rlength[j], lpiece, pieceSet);
      minimum = min(target.first,ffdresult.first);
      if (target.first != minimum){
        target.first = minimum;
        target.second = j;
        left = ffdresult.second;
      }
    }
    used_rolls.push_back(target.first);
    leftover.push_back(left);
    rollType.push_back(target.second);
    
    cgs[i][i] = dpiece[i];
  }
}

// Constructor por copia
Solution::Solution(const Solution& a, int M) {
  leftover = a.leftover;
  used_rolls = a.used_rolls;
  rollType = a.rollType;
  size = a.size;
  cgs = a.cgs;
}

void Solution::printSolution() {
  int i = 0;
  int totalpieces = 0;
  vector<vector<int> >::iterator it;
  vector<int>::iterator iv;
  for(it = cgs.begin(); it != cgs.end() ; it++) {
    cout << "Tipo " << i << endl;
    for(iv = (*it).begin(); iv != (*it).end(); iv++) {
      if ((*iv) != 0) {
        cout << (*iv) << endl;
        totalpieces += (*iv);
      }
    }
    cout << "===========" << endl;
    i++;
  }
  cout << "Total pieces = " << totalpieces << endl;
}

void Solution::update(int where, vector<int> &lpiece,
                      vector<int> &rlenght) {
  pair<int,int> new_values;
  vector<int> fset = cgs[where];
  new_values = FFD(rlenght[rollType[where]],lpiece, fset);
  used_rolls[where] = new_values.first;
  leftover[where]   = new_values.second;
}


