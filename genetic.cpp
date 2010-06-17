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

vector<Solution*> genPeople(int tam, vector<int> &rlength,
                            vector<int> &lpiece,
                            vector<int> &dpiece) {
  vector<Solution*> solutionSet;
  Solution initial = Solution(rlength, lpiece, dpiece);
  for(int i = 0; i < tam; i++) {
    solutionSet.push_back(randomSol(initial, lpiece, 
                                    rlength));
  }
  return solutionSet;
}

// Cruce de un punto. Genera dos soluciones 
// infactible.
pair<Solution,Solution> Cross(Solution* mother, Solution* father) {
  // Se generan dos hijos
  vector<vector<int> > acgs;
  vector<vector<int> > bcgs;
  // Punto de cruce
  int point;
  while(true) {
    point = (int) round(rand()) % mother->size;
    if (point == 0) continue;
    if (point >= mother->size - 1) continue;
    break;
  }
  vector<vector<int> >::iterator it;
  vector<int>::iterator vit;
  vector<int> aleftover;
  vector<int> aused_rolls;
  vector<int> arollType;
  vector<int> bleftover;
  vector<int> bused_rolls;
  vector<int> brollType;
  // Primer Hijo. Construcción de genes
  for(it = mother->cgs.begin(); it < mother->cgs.begin() + point; it++) {
    acgs.push_back(*it);
  }
  for(it = father->cgs.begin() + point; it != father->cgs.end(); it++) {
    acgs.push_back(*it);
  }
  // Primer Hijo copia de arreglo leftover;
  for(vit = mother->leftover.begin(); vit < mother->leftover.begin() + point; vit++) {
    aleftover.push_back(*vit);
  }
  for(vit = father->leftover.begin() + point; vit != father->leftover.end(); vit++) {
    aleftover.push_back(*vit);
  }
  // Primer Hijo. Copia de arreglo used_rolls
  for(vit = mother->used_rolls.begin(); vit < mother->used_rolls.begin() + point; vit++) {
    aused_rolls.push_back(*vit);
  }
  for(vit = father->used_rolls.begin() + point; vit != father->used_rolls.end(); vit++) {
    aused_rolls.push_back(*vit);
  }
  // Primer Hijo. Copia de arreglo rollType
  for(vit = mother->rollType.begin(); vit < mother->rollType.begin() + point; vit++) {
    arollType.push_back(*vit);
  }
  for(vit = father->rollType.begin() + point; vit != father->rollType.end(); vit++) {
    arollType.push_back(*vit);
  }
  // Segundo hijo. Construcción de genes
  for(it = mother->cgs.begin() + point; it != mother->cgs.end(); it++) {
    bcgs.push_back(*it);
  }
  for(it = father->cgs.begin(); it < father->cgs.begin() + point; it++) {
    bcgs.push_back(*it);
  }
  // Segundo hijo. Copia de arreglo leftover
  for(vit = mother->leftover.begin() + point; vit != mother->leftover.end(); vit++) {
    bleftover.push_back(*vit);
  }
  for(vit = father->leftover.begin(); vit < father->leftover.begin() + point; vit++) {
    bleftover.push_back(*vit);
  }
  // Segundo hijo. Copia de arreglo used_rolls
  for(vit = mother->used_rolls.begin() + point; vit != mother->used_rolls.end(); vit++) {
    bused_rolls.push_back(*vit);
  }
  for(vit = father->used_rolls.begin(); vit < father->used_rolls.begin() + point; vit++) {
    bused_rolls.push_back(*vit);
  }
  // Segundo hijo. Copia de arreglo rollType
  for(vit = mother->rollType.begin() + point; vit != mother->rollType.end(); vit++) {
    brollType.push_back(*vit);
  }
  for(vit = father->rollType.begin(); vit < father->rollType.begin() + point; vit++) {
    brollType.push_back(*vit);
  }
  pair<Solution,Solution> result;
  Solution a(aleftover, aused_rolls, arollType, acgs, mother->size);
  Solution b(bleftover, bused_rolls, brollType, bcgs, mother->size);
  result.first = a;
  result.second = b;
  
  return result;
  
  
} 
  
