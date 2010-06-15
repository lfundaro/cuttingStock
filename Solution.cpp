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

// Solution::~Solution() {
//   free_vector(cgs);
// }

Solution::Solution(const Solution& a, int M) {
  leftover = a.leftover;
  used_rolls = a.used_rolls;
  rollType = a.rollType;
  size = a.size;
  cgs = a.cgs;
  // for(int i = 0; i < size; i++) {
  //   cgs.push_back(new vector<int>(M,0)); 
  // }
  // for(int i = 0; i < size; i++) {
  //   //    for(int j = 0; j < M; j++) {
  //   cgs[i] = a.cgs[i];
  //     // }
  // }
}


