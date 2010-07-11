#include "genetic.h"
using namespace std;

// Función que genera una solución aleatoria. 
// Parte de una solución inicial válida y realiza 
// perturbaciones que se traducen en mover una pieza 
// de un grupo de corte a otro siempre y cuando se respeten 
// las restricciones del problema.
// La cantidad de permutaciones se puede controlar con la 
// constante NUM_PERTURBATIONS que se encuentra en el 
// encabezado de este archivo. Igualmente, el porcentage 
// de piezas que se mueven de un grupo a otro se puede
// cambiar con la constante MOVE_PERCENTAGE
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
                                 destiny,origin,rlength)) {
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
  new_solution.diversity = calcDiversity(rlength.size(),
					 new_solution.cgs,
					 new_solution.rollType,
					 new_solution.used_rolls);
  new_solution.variety = calcVariety(new_solution.cgs);
  return new_solution;
}

// Genera la población inicial del algoritmo genético.
// Utiliza repetidamente la función randomSol para 
// generar un individuo en la población que esté 
// perturbado aleatoriamente.
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

// Cruce de un punto. Este cruce puede generar soluciones
// factibles y no factibles. La práctica demostró que la 
// mayoría de las soluciones son no factibles por lo tanto 
// deben ser arregladas con la función fixSolution.
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
  vector<vector<int> > diversity;
  vector<int> variety;
  Solution a(aleftover, aused_rolls, arollType, acgs, 0.0, mother->size,0,variety,diversity);
  Solution b(bleftover, bused_rolls, brollType, bcgs,0.0, mother->size,0,variety, diversity);
  result.first = a;
  result.second = b;
  return result;
}

// Arregla una solución que se genera del cruce entre 
// otras dos soluciones. Por cada tipo de pieza demandada 
// se hace una vuelta de reconocimiento para ver 
// si hay un déficit o superávit. Según sea el caso 
// se quitan o se ponen piezas para así arreglar la 
// solución.
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
        destiny = (int) round(random()) % son.size;
        if (notEmptyColumn(son.cgs[destiny]) && 
            checkConstraints(lpiece, son, destiny,i,rlength)) {
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


// Chequea que se cumplan las restricciones de 
// Open Stacks y mínima diferencia de longitud 
// entre piezas que se cortan.
bool checkConstraints(vector<int> &lpiece,
                      Solution &sol, int destiny,
                      int pieceType, vector<int> &rlength) 
{
  // Verificación de RO y Diferencia entre piezas
  vector<int> tmp = sol.cgs[destiny];
  int minimum;
  int ro_count = 0;
  for(int i = 0; i < tmp.size(); i++) {
    if (tmp[i]) 
      ro_count++;
    else { 
      // Esta es la pieza que iría en el cutting group
      if (i == pieceType) 
        ro_count++;
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
  if (ro_count <= RO && minimum >= DIFF && 
      rlength[sol.rollType[destiny]] >= lpiece[pieceType]) 
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

// Se reemplaza al peor de la población.
// El peor de la población se mide en base 
// al valor de fitness. Mientras mayor se dicho 
// valor significa que la solución desperdicia 
// mucho el espacio por lo tanto se elimina de 
// la población.
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
  people.erase(people.begin() + maximum.second);
  people.insert(people.begin() + maximum.second, son);
  return;
}

// Toma el mejor individuo de la población
// en base a su valor de fitness. El mejor 
// será aquel que tenga el mínimo fitness 
// de entre todos.
Solution get_best(vector<Solution> &people) {
  double control = MAX_DOUBLE;
  pair<double,int> minimum;
  minimum.first = MAX_DOUBLE;
  for(int i = 0; i < people.size(); i++) {
    control = min(control,people[i].fitness);
    if (control != minimum.first) {
      minimum.first = control;
      minimum.second = i;
    }
  }
  return people[minimum.second];
}

// Algoritmo genético que genera una población inicial
// aleatoria, realiza cruces de un punto, tiene un 
// criterio de selección de ruleta y su estrategia 
// de remplazo es estacionaria reemplazando al peor.
Solution geneticAlgorithm(int tam, vector<int> &rlength,
                          vector<int> &dpiece,
                          vector<int> &lpiece,
                          int genNum) {
  Solution optimum = Solution(rlength, lpiece, dpiece);
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
    prob = roulette(people);
    parents = getParents(people, prob);
    crossFactor = (double) random() / RAND_MAX; 
    if (crossFactor >= CROSS_PROB) {
      children = Cross(&parents.first, &parents.second);
      // Arreglamos las soluciones
      fixSolution(children.first, dpiece, rlength,lpiece);
      fixSolution(children.second, dpiece, rlength,lpiece);
      mutationFactor = (double) random() / RAND_MAX;
      if (mutationFactor <= MUTATION_FACT) {
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
    bestFound = get_best(people);
    optimum = opt(optimum,bestFound);
    z++;
  }
  return optimum;
}

// Función que toma la mejor solución entre dos soluciones.
Solution opt(Solution a, Solution b) {
  if (a.fitness < b.fitness) { 
    //cout << b.fitness << " --> " << a.fitness << endl; 
    return a; 
  }
  else { 
    //cout << a.fitness << " --> " << b.fitness << endl; 
    return b; 
  }
}

// Función que sirve para mutar un hijo. Esta función se 
// invoca dependiendo de la probabilidad de mutación. 
// Para mutar se toma una pieza aleatoria de un grupo 
// de corta y se pone en otro grupo de corte.
void mutate(Solution &child, vector<int> &rlength,
            vector<int> &lpiece) {
  int origin;
  int destiny;
  int piece;
  int space;
  int M = child.size;
  while (true) {
    origin = (int) round(random()) % M;
    piece = (int) round(random()) % M;
    destiny = (int) round(random()) % M;
    // Se puede tomar pieza
    if (child.cgs[origin][piece]) {
      if (destiny != origin && rlength[child.rollType[destiny]] >= lpiece[piece]) {
        space = lpiece[piece];
        if (checkConstraints(lpiece, child, destiny,piece,rlength)) {
          // Se puede hacer movimiento de pieza
          child.cgs[origin][piece]--;
          child.update(origin,lpiece,rlength);
          child.cgs[destiny][piece]++;
          child.update(destiny, lpiece, rlength);
          // Actualización de fitness
          child.fitnessEval();
          break;
        }
        else continue; //No se cumplen constraints
      }
      else continue;
    }
    else continue; //No hay pieza de tipo piece
  }
  return;
}

// Selecciona los padres para un cruce. El primer componente 
// que se toma es aquel que tiene la probabilidad más pequeña
// que indica el método de la ruleta. Luego el segundo 
// componente se elige de manera aleatoria de entre el resto 
// de los padres que quedan como candidatos.
pair<Solution,Solution> getParents(vector<Solution> &people, vector<double> &prob) {
  double chance = random() % people.size();
  vector<pair<int,double> > probSelect;
  for(int i = 0; i < prob.size(); i++) 
    probSelect.push_back(make_pair(i, prob[i]));
  sort(probSelect.begin(), probSelect.end(), comparePairDouble);
  int mother = ((int) round(random()) % (people.size()-1)) + 1;
  pair<Solution,Solution> result = make_pair(people[probSelect[0].first], people[probSelect[mother].first]);
  return result;
}

// Agrega o quita un pieza a un grupo de corte
void addPiece(vector<int> & targetIndex, Solution &son,
              int udiff, vector<int> &rlength, 
              vector<int> &lpiece, int pieceType) {
  int candidate;
  pair<int,int> newConfig;
  while (true) {
    candidate = (int) round(random()) % targetIndex.size();
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
