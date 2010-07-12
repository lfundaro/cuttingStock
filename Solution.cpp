#include "Solution.h"
using namespace std;

// Construye una solución inicial. 
// La solución inicial consiste en colocar todas las 
// piezas del tipo i en el grupo de corte i utilizando 
// el tipo de rollo que minimice el desperdicio.
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

  // Cálculo de variety
  variety = calcVariety(cgs);
  // Cálculo de diversidad
  diversity = calcDiversity(rlength.size(), cgs, rollType,
                            used_rolls);
  label = false;
}

// Constructor por copia
Solution::Solution(const Solution& a) {
  leftover = a.leftover;
  used_rolls = a.used_rolls;
  rollType = a.rollType;
  size = a.size;
  cgs = a.cgs;
  fitness = a.fitness;
  penalty = a.penalty;
  variety = a.variety;
  diversity = a.diversity;
  label = a.label;
}

Solution::Solution(vector<int> left, 
                   vector<int> urolls,
                   vector<int> rType, 
                   vector<vector<int> > p, double fitn,
                   int sz, int penal, vector<int> variet,
                   vector<vector<int> > divers,
                   bool labl) {
  leftover = left;
  used_rolls = urolls;
  rollType = rType;
  cgs = p;
  size = sz;
  fitness = fitn;
  penalty = penal;
  variety = variet;
  diversity = divers;
  label = labl;
}

Solution::~Solution() {};

Solution::Solution() {};


void Solution::printSolution() {
  int i = 0;
  int totalpieces = 0;
  vector<vector<int> >::iterator it;
  vector<int>::iterator iv;
  for(it = cgs.begin(); it != cgs.end() ; it++) {
    cout << "Cutting group " << i << endl;
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

// Actualización de la solución. Cuando se quita o se pone 
// una pieza de un grupo de corte se deben actualizar 
// la cantidad de desperdicio y rollos que se usó 
// para ese grupo de corte.
void Solution::update(int where, vector<int> &lpiece,
                      vector<int> &rlength) {
  pair<int,int> new_values;
  vector<int> fset = cgs[where];
  new_values = FFD(rlength[rollType[where]],lpiece, fset);
  used_rolls[where] = new_values.first;
  leftover[where]   = new_values.second;
}

// Función que dada una solución calcula su Fitness.
// El fitness se calcula en base a la variable penalty 
// la cual indica cuantas veces hubo que arrelar la 
// solución para hacerla válida despues de un cruce. 
// También se calcula en base a la sumatoria de leftover.
// De cada cifra se toma un porcentage. Como queremos 
// minimizar el desperdicio entonces tomamos el mayor 
// porcentage de desperdicio y junto con el porcentage 
// de penalty se suman ambas cifras y se obtiene el atributo
// fitness.
void Solution::fitnessEval() {
  int sum = 0;
  for(int i = 0; i < leftover.size(); i++) 
    sum += leftover[i];
  double x = (PENALTY * penalty) / 100;
  double y = (LEFTOVER * sum) / 100;
  fitness = x + y;
}

void Solution::printAsPaper(vector<int> &rlength,
                            vector<int> &lpiece) {
  printCG(cgs);
  int sum;
  int totalLO = 0;
  for(int i = 0; i < cgs.size(); i++) {
    totalLO += leftover[i];
  }
  

  vector<int> pieceSet;
  int totalRollLength = 0;
  int minLG;
  int minRollType;
  for(int i = 0; i < cgs.size(); i++) {
    if (notEmptyColumn(cgs[i])) {
      pieceSet = cgs[i];
      
      int* bestCut;
      bestCut = bestCutting(pieceSet, rlength, lpiece);
      
        minLG = bestCut[1];
        minRollType = bestCut[2];
        
        delete [] bestCut;
        
        totalRollLength += minLG*rlength[minRollType];
    }
  }
  
  cout << "Trim loss = " << (100.0*(double)totalLO)/(double)totalRollLength << endl;
  int totLeft = 0;
  vector<int>::iterator itl;
  for(itl = leftover.begin(); itl != leftover.end(); itl++)
    totLeft += *itl;
  cout << "Total leftover = " << totLeft << endl;
  cout << "Penalty = " << penalty << endl;
  cout << "Fitness = " << fitness << endl;
}


bool Solution::checkRolls(vector<int> &rlength,
                          vector<int> &lpiece) {
  for(int i = 0; i < cgs.size(); i++) {
    for(int j = 0; j < cgs.size(); j++) {
      if (cgs[i][j]) {
        if (rlength[rollType[i]] < lpiece[j]) {
          cout << "error: pieza " << j << " en " <<
            "CG " << i <<  endl;
          cout << "Rolltype = " << rlength[rollType[i]] 
               << endl;
          cout << "PieceSize = " << lpiece[j] << endl;
          return true;
        }
      }
    }
  }
    return false;
}

