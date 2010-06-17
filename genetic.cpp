#include "genetic.h"
using namespace std;

Solution* randomSol(Solution &initial, vector<int> &lpiece,
                    vector<int> &rlength) {
  Solution* new_solution = new Solution(initial);
  int n = NUM_PERTURBATIONS;
  int origin;
  int destiny;
  int npieces;
  int take;
  int space;
  int type;
  int rep=0;
  int ro_count;
  int minimum;
  for(int i = 0; i < n; i++) {
    while (true) {
      origin = (int) round(random()) % new_solution->size;
      npieces = new_solution->cgs[origin][origin];
      take = (MOVE_PERCENTAGE*npieces) / 100;
      if (take) { // Hay suficientes piezas para mover
        while (true) {
          destiny = (int) round(random()) % new_solution->size;
          if (destiny != origin) { 
            if (rlength[new_solution->rollType[destiny]] 
                < lpiece[origin]) 
              continue;
            space = take*lpiece[origin];
            //if (new_solution->leftover[destiny] >= space) {
              // Verificación de RO y Diferencia entre piezas
              vector<int> tmp = new_solution->cgs[destiny];
              ro_count = 0;
              for(int i = 0; i < tmp.size(); i++) {
                if (tmp[i]) 
                  ro_count += 1;
                if (i == destiny) 
                  ro_count += 1;
              }
              minimum = MAX_INT;
              for(int i = 0; i < tmp.size(); i++) {
                for(int j = 0; j < tmp.size(); j++) {
                  if (i != j) {
                    if (tmp[j]) {
                      minimum = min(minimum, abs(lpiece[i] - lpiece[j]));
                    }
                    else {
                      if (destiny == j) {
                        minimum = min(minimum, abs(lpiece[i] - lpiece[destiny]));
                      }
                    }
                  }
                }
              }
              if (ro_count > RO || minimum < DIFF) break; 
              new_solution->cgs[origin][origin] -= take;
              new_solution->update(origin,lpiece,rlength);
              new_solution->cgs[destiny][origin]  += take;
              new_solution->update(destiny,lpiece,rlength);
              break;
              //            }
            // else continue; // No hay espacio en destino 
            //                // Se intenta con otro destino
          }
        }
        break; // Se logro hacer perturbación
      }
      else continue; //No hay suficientes piezas para mover
    }
  }
  return new_solution;
}
  
