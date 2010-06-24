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
      if (target.first != minimum) {
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
  size = M;
  double sum = 0.0;
  for(int i = 0; i < leftover.size(); i++) 
    sum += leftover[i];
  fitness = (LEFTOVER * sum) / 100;
  penalty = 0;
}

Solution::~Solution() {}

// Constructor por copia
Solution::Solution(const Solution& a, int M) {
  leftover = a.leftover;
  used_rolls = a.used_rolls;
  rollType = a.rollType;
  size = a.size;
  cgs = a.cgs;
  fitness = a.fitness;
  penalty = a.penalty;
}

Solution::Solution(vector<int> left, 
                   vector<int> urolls,
                   vector<int> rType, 
                   vector<vector<int> > p, double fitn,
                   int sz, int penal) {
  leftover = left;
  used_rolls = urolls;
  rollType = rType;
  cgs = p;
  size = sz;
  fitness = fitn;
  penalty = penal;
}

Solution::Solution() {};


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
  int totLeft = 0;
  vector<int>::iterator itl;
  for(itl = leftover.begin(); itl != leftover.end(); itl++)
    totLeft += *itl;
  cout << "Total pieces = " << totalpieces << endl;
  cout << "Total leftover = " << totLeft << endl;
  cout << "Penalty = " << penalty << endl;
  cout << "Fitness = " << fitness << endl;
}

void Solution::update(int where, vector<int> &lpiece,
                      vector<int> &rlength) {
  pair<int,int> new_values;
  vector<int> fset = cgs[where];
  new_values = FFD(rlength[rollType[where]],lpiece, fset);
  used_rolls[where] = new_values.first;
  leftover[where]   = new_values.second;
}

void Solution::fitnessEval() {
  int sum = 0;
  for(int i = 0; i < leftover.size(); i++) 
    sum += leftover[i];
  double x = (PENALTY * penalty) / 100;
  double y = (LEFTOVER * sum) / 100;
  fitness = x + y;
}

// void Solution::printSolution() {
//   vector<vector<int> >::iterator it;
//   vector<int>::iterator eit;
//   int i = 0;
//   for(it = cgs.begin(); it != cgs.end(); it++) {
//     cout << "C.Group" << endl;
//     cout << i << endl;
//     for(eit = (*it).begin(); eit != (*it).end(); eit++) {
//       if (*eit != 0) 
//         cout << *eit << endl;
//     }
//     i++;
//     cout << "============" << endl;
//   }
// }
