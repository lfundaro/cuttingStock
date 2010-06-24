#include "genetic.h"
using namespace std;

Solution randomSol(Solution &initial, vector<int> &lpiece,
                   vector<int> &rlength) {
  Solution new_solution = Solution(initial);
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
      origin = (int) round(random()) % new_solution.size;
      npieces = new_solution.cgs[origin][origin];
      take = (MOVE_PERCENTAGE*npieces) / 100;
      if (take) { // Hay suficientes piezas para mover
        while (true) {
          destiny = (int) round(random()) % new_solution.size;
          if (destiny != origin) { 
            if (rlength[new_solution.rollType[destiny]] 
                < lpiece[origin]) 
              continue;  // La pieza no cabe en el roll
            space = take*lpiece[origin];
            // Verificación de RO y Diferencia entre piezas
            if (checkConstraints(lpiece,new_solution,
                                 destiny,i)) {
              new_solution.cgs[origin][origin] -= take;
              new_solution.update(origin,lpiece,rlength);
              new_solution.cgs[destiny][origin]  += take;
              new_solution.update(destiny,lpiece,rlength);
              break;
            }
          }
          else continue; // Origen igual a destino
        }
        break; // Se logro hacer perturbación
      }
      else continue; //No hay suficientes piezas para mover
    }
  }
  new_solution.penalty = 0;
  return new_solution;
}

  
vector<Solution> genPeople(int tam, vector<int> &rlength,
                           vector<int> &lpiece,
                           vector<int> &dpiece) {
  vector<Solution> solutionSet;
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
    cout << "here" << endl;
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
  Solution a(aleftover, aused_rolls, arollType, acgs, 0.0, mother->size,0);
  Solution b(bleftover, bused_rolls, brollType, bcgs,0.0, mother->size,0);
  result.first = a;
  result.second = b;
  cout << "no prob" << endl;
  return result;
}

void fixSolution(Solution &son, vector<int> &dpiece, 
                 vector<int> &rlength,vector<int> &lpiece) {
  int M = son.size;
  int penalty = 0;
  vector<int> targetIndex;
  int howMuch;
  int pAmount;
  int diff;
  int candidate;
  pair<int,int> newConfig;
  for(int i = 0; i < M; i++) {
    // Vuelta de reconocimiento
    howMuch = 0;
    pAmount = 0;
    for(int j = 0; j < M; j++) { // Para cada columna
      pAmount = son.cgs[j][i];
      if (pAmount) { // Hay pieza de tipo i
        howMuch += pAmount;
        // Guardo la columna en la que conseguí la pieza
        targetIndex.push_back(j);
      }
    }
    if (howMuch) {
      diff = dpiece[i] - howMuch;
      if (diff) {  // Diferencia es distinta de cero
        penalty += abs(dpiece[i] - howMuch);
        // Si se consigue un candidato cuyas piezas es 
        // menor de lo que se quita, se debe buscar otro.
        if (diff > 0) { // Agrego piezas 
          for(int p = 0; p < diff; p++) 
            addPiece(targetIndex,son,1,rlength, lpiece,i);
        }
        else {   // Quito piezas 
          for(int q = 0; q < abs(diff); q++)
            addPiece(targetIndex,son,-1, rlength, lpiece,i);
        }
      }
    }
    else {     // No se consiguió la pieza en la solución
      int destiny;
      penalty += dpiece[i];
      while (true) {
        cout << "No piece found";
        destiny = (int) round(random()) % son.size;
        if (notEmptyColumn(son.cgs[destiny]) && 
            checkConstraints(lpiece, son, destiny,i)) {
          son.cgs[destiny][i] += dpiece[i];
          newConfig = FFD(rlength[son.rollType[destiny]],
                          lpiece, son.cgs[destiny]);
          son.used_rolls[destiny] = newConfig.first;
          son.leftover[destiny] = newConfig.second;
          break;
        }
      }
    }
    targetIndex.clear();
  }
  son.penalty = penalty;
  son.fitnessEval();
}

int notEmptyColumn(vector<int> column) {
  int decision = 0;
  for(int i = 0; i < column.size(); i++) {
    decision |= column[i];
  }
  return decision;
}

  
bool checkConstraints(vector<int> &lpiece,
                      Solution &sol, int destiny,
                      int pieceType) 
{
  // Verificación de RO y Diferencia entre piezas
  vector<int> tmp = sol.cgs[destiny];
  int minimum;
  int ro_count = 0;
  for(int i = 0; i < tmp.size(); i++) {
    if (tmp[i]) 
      ro_count += 1;
    else { 
      // Esta es la pieza que iría en el cutting group
      if (i == pieceType) 
        ro_count += 1; 
    }
  }
  minimum = MAX_INT;
  // Busco tipos de piezas en el cutting group actual 
  // y si no consigo pieceType se la Agrego.
  vector<int> pieces;
  for(int i = 0; i < tmp.size(); i++) {
    if (tmp[i]) 
      pieces.push_back(i);
    else {
      if (i == pieceType)
        pieces.push_back(i);
    }
  }
  // Para un tipo de pieza i verifico su diferencia 
  // con el resto
  for(int i = 0; i < pieces.size(); i++) {
    for(int j = 0; j < pieces.size(); j++) {
      if (i != j) {
        minimum = min(minimum, abs(lpiece[i] - lpiece[j]));
      }
    }
  }
  if (ro_count <= RO && minimum >= DIFF) 
    return true;   // Cumple los contraints.
  else 
    return false;  // No cumple constraints.
}

// Computa la probabilidad de elección de cada individuo
// en la población
vector<double> roulette(vector<Solution> &people) {
  vector<double> prob;
  double sum;
  for(int i = 0; i < people.size(); i++) {
    sum = 0.0;
    for(int j = 0; j < people.size(); j++) {
      sum += people[j].fitness;
    }
    prob.push_back((people[i].fitness)/sum);
  }
  return prob;
}

// Se reemplaza al peor de la población
void replace(vector<Solution> &people,
             Solution &son) {
  pair<double,int> maximum;
  maximum.first = MIN_DOUBLE;
  double control = MIN_DOUBLE;
  for(int i = 0; i < people.size(); i++) {
    control = max(control,people[i].fitness);
    if (control != maximum.first) {
      maximum.first = control;
      maximum.second = i;
    }
  }
  //delete people[maximum.second];
  //  vector<Solution*>::iterator it;
  //it = people.begin();
  people.erase(people.begin() + maximum.second);
  //people[maximum.second] = son;
  people.insert(people.begin() + maximum.second, son);
  return;
}

Solution get_best(vector<Solution> &people) {
  double control = MAX_INT;
  pair<double,int> minimum;
  minimum.first = MAX_INT;
  for(int i = 0; i < people.size(); i++) {
    control = min(control,people[i].fitness);
    if (control != minimum.first) {
      minimum.first = control;
      minimum.second = i;
    }
  }
  return people[minimum.second];
}

Solution geneticAlgorithm(int tam, vector<int> &rlength,
                          vector<int> &dpiece,
                          vector<int> &lpiece,
                          int genNum) {
  Solution bestFound; 
  // Generación de la población inicial
  vector<Solution> people = genPeople(tam, rlength, lpiece,
                                      dpiece);
  // Se calcula fitness de cada una de las soluciones 
  // aleatorias iniciales
  for(int i = 0; i < people.size(); i++) 
    people[i].fitnessEval();
  vector<double> prob; // Vector de probabilidades
  pair<Solution,Solution> parents;
  pair<Solution,Solution> children;
  int z = 0;
  int toMutate;
  double crossFactor;
  double mutationFactor;
  while (z < genNum) {
    bestFound = get_best(people);
    prob = roulette(people);
    parents = getParents(people, prob);
    crossFactor = (double) random() / RAND_MAX; 
    if (crossFactor >= CROSS_PROB) {
      cout << "bye" << endl;
      children = Cross(&parents.first, &parents.second);
      // Arreglamos las soluciones
      fixSolution(children.first, dpiece, rlength,lpiece);
      fixSolution(children.second, dpiece, rlength,lpiece);
      children.first.printSolution();
      children.second.printSolution();
      int a;
      cin >> a;
      cout << "after fix" << endl;
      mutationFactor = (double) random() / RAND_MAX;
      if (mutationFactor <= MUTATION_FACT) {
        cout << "mutati" << endl;
        toMutate = (int) random() % 2;
        if (toMutate) 
          mutate(children.second, rlength, lpiece);
        else
          mutate(children.first, rlength, lpiece);
      }
      // Remplazo en la población
      replace(people, children.first);
      replace(people, children.second);
    }
    z++;
    cout << "hello" << endl;
  }
  return bestFound;
}

void mutate(Solution &child, vector<int> &rlength,
            vector<int> &lpiece) {
  int origin;
  int destiny;
  int piece;
  int space;
  int M = child.size;
  while (true) {  // Seleccionamos origen
    origin = (int) round(random()) % M;
    cout << "mutati origin " << endl;
    piece = (int) round(random()) % M;
    // Se puede tomar pieza
    if (child.cgs[origin][piece]) {
      destiny = (int) round(random()) % M;
      cout << "mutati destiny " << endl;
      if (destiny != origin && 
          rlength[child.rollType[destiny]] >= lpiece[piece]) {
        space = lpiece[piece];
        if (checkConstraints(lpiece, child, destiny,origin)) {
          // Se puede hacer movimiento de pieza
          child.cgs[origin][piece]--;
          child.update(origin,lpiece,rlength);
          child.cgs[destiny][piece]++;
          child.update(destiny, lpiece, rlength);
          // Actualización de fitness
          child.fitnessEval();
          break;
        }
        else continue;
      }
      else continue;
    }
    else continue;
  }
  return;
}

pair<Solution,Solution> getParents(vector<Solution> &people, vector<double> &prob) {
  double chance = random() % people.size();
  vector<pair<int,double> > probSelect;
  for(int i = 0; i < prob.size(); i++) 
    probSelect.push_back(make_pair(i, prob[i]));
  sort(probSelect.begin(), probSelect.end(), comparePairDouble);
  pair<Solution,Solution> result = make_pair(people[probSelect[0].first], people[probSelect[1].first]);
  return result;
}

void addPiece(vector<int> & targetIndex, Solution &son,
              int udiff, vector<int> &rlength, 
              vector<int> &lpiece, int pieceType) {
  int candidate;
  pair<int,int> newConfig;
  while (true) { 
    candidate = (int) round(random()) % targetIndex.size();
    cout << "candidateFIX " << endl;
    if (son.cgs[targetIndex[candidate]][pieceType] + udiff >= 0) {
      son.cgs[targetIndex[candidate]][pieceType] += udiff;
      // Se elimina el candidato si la columna se queda 
      // sin piezas del tipo
      if (!son.cgs[targetIndex[candidate]][pieceType]) 
        targetIndex.erase(targetIndex.begin() + candidate);
      // Se ejecuta FFD con nuevo estado de piezas 
      int index = targetIndex[candidate];
      newConfig = FFD(rlength[son.rollType[index]],lpiece,son.cgs[index]);
      son.used_rolls[index] = newConfig.first;
      son.leftover[index] = newConfig.second;
      break;
    }
    else {
      // Se elimina el índice que falló 
      targetIndex.erase(targetIndex.begin() + candidate);
      continue;
    }
  }
  return;
}




  



