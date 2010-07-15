#include "scatterSearch.h"
using namespace std;

/*Funcion que calcula que tan diferentes son un par
  de soluciones.
  Primero contrasta que cantidad de cada tipo de piezas
  hay en cada tipo de roll
  Segundo, resta las varianzas de la distribucion de
  las piezas dentro de cada cutting group de cada solucion.
  Suma estos dos valores luego de aplicarles
  dos modificadores
 */
double diff(Solution& sol1, Solution& sol2){
  int ngroups = min(sol1.cgs.size(),sol2.cgs.size());
  int npieces = sol1.cgs[0].size();

  double variance_diff;
  double diff;

  variance_diff = fabs(cg_standarDev(sol1.cgs)-cg_standarDev(sol2.cgs));
    
  return variance_diff;
}

vector<Solution> makeRefSet(vector< Solution >& P, int b){
  int Psize = P.size();

  vector< pair<int,int> > divs(Psize-(b/2),pair<int,int>());
  vector<Solution> refSet(0);
  refSet.reserve(b);

  //Busco los b/2 primeros
  sort(P.begin(),P.end(),compareFitnessReverse);
  for (int i = 0; i < b/2; ++i){
    refSet.push_back(P[Psize-1-i]);
  }

  //Busco los b/2 mas variados
  P.resize(Psize-(b/2));    //Elimino los b/2 mejores
  diversity(P,refSet,divs); //Busco los b/2 mas variados

  for (int i = b/2; i < b; ++i){
    int sol = divs[i-b/2].first;
    refSet.push_back(P[sol]);
  }

  return refSet;
}

void diversity(vector< Solution >& P,
	       vector< Solution >& sols,
	       vector< pair<int,int> >& divs){
  int nsols = sols.size();
  int nP = P.size();
  int div = 0;

  for (int i = 0; i<nP; ++i){
    for (int j = 0; j<nsols; ++j){
      div += diff(sols[j],P[i]);
    }
    divs[i].first = i;
    divs[i].second = div;
    div = 0;
  }

  sort(divs.begin(),divs.end(),compareDivs);
}

// Algoritmo de Búsqueda Dispersa. Consiste 
// en generar un conjunto inicial llamado P, 
// el cual consta de soluciones generadas alea-
// toriamente. Cada una de estas soluciones 
// es mejorada utilizando búsqueda local. 
// Luego se construye el conjunto de referencia 
// donde el en la primera mitad van las soluciones
// con mejor calidad (fitness) y en la segunda 
// mitad las soluciones más diversas. 
// Finalmente se cruzan las soluciones y se hace 
// búsqueda local en el resultado de dicho cruce.
// El algoritmo itera hasta una cantidad prefijada 
// de construcciones.
Solution scatterSearch(int P_size, int b, 
                       vector<int> &rlength,
                       vector<int> &lpiece, 
                       vector<int> &dpiece,
                       vector<int> &lot_s) {
  vector<Solution> P = genPset(rlength,lpiece,dpiece,
                               P_size,lot_s);
  vector<Solution> refSet = makeRefSet(P,b);
  int M = refSet.size();
  int* pair;
  int candidates[M];
  for(int i = 0; i < M; i++) candidates[i] = i;
  std::pair<Solution,Solution> children;
  int index0;
  int index1;
  Solution theOne;
  bool newSolution = true;
  int next_swap[2];
  int h = 0;
  vector<std::pair<int,int> > divs(P_size,std::pair<int,int>());
  int recs = 1;
  while(newSolution) {
    cout << "Combinando soluciones" << endl;
    next_swap[0] = 0;
    next_swap[1] = 2;
    pair = twoOnN(candidates,next_swap,M);
    newSolution = false;
    while (pair != NULL) {
      index0 = pair[0];
      index1 = pair[1];
      if (!refSet[index0].label || !refSet[index1].label) {
        // Conjuntos sin examinar
        refSet[index0].label = true;
        refSet[index1].label = true;
        children = Cross(&refSet[index0],&refSet[index1]);
        fixSolution(children.first, dpiece, rlength,lpiece);
        fixSolution(children.second, dpiece, rlength,lpiece);
        // Se toma la mejor solución resultante del cruce
        if (children.first.fitness < children.second.fitness) 
          theOne = children.first;
        else 
          theOne = children.second;
      
        localSearchBB(theOne,rlength, lot_s, lpiece, dpiece);
        if (theOne.fitness < refSet.back().fitness &&
            !find(theOne,refSet)) {
          refSet.pop_back();
          refSet.push_back(theOne);
          sort(refSet.begin(), refSet.end(), compareFitness);
          newSolution = true;
        }
      }
      free(pair);
      pair = twoOnN(candidates,next_swap,M);
    }
    if (!newSolution) { // Regenero P
      if (recs > recons)   {
        cout << "====================" << endl;
        cout << "Resultado" << endl;
        cout << "====================" << endl;
        return refSet.front();
      }
      recs++;
      cout << "====================" << endl;
      cout << "Reconstrucción " << recs << " de conjunto P" 
           << endl;
      cout << "====================" << endl;
      newSolution = true;
      P = genPset(rlength,lpiece,dpiece,
                  P_size,lot_s);
      refSet.resize(b/2);
      diversity(P,refSet,divs);
      int sol;
      for(int q = b/2; q < b; q++) {
        sol = divs[q-b/2].first;
        refSet.push_back(P[sol]);
      }
    }
  }
}

// Algoritmo para generar el conjunto P. 
// Genera una solución aleatoria y la 
// mejora para luego ser integrada al 
// conjunto P. El algoritmo no mete soluciones
// repetidas en el conjunto. Para esto se vale 
// del algoritmo de diferencia entre soluciones 
// llamado diff. 
vector<Solution> genPset(vector<int> &rlength,
                         vector<int> &lpiece,
                         vector<int> &dpiece,
                         int P_size,
                         vector<int> &lot_s) {
  cout << "====================" << endl;
  cout << "Generando conjunto P" << endl;
  cout << "====================" << endl;
  vector<Solution> Pset;
  Solution initial = Solution(rlength, lpiece, dpiece);
  Solution ramdSol;
  vector<std::pair<int,double> > control;
  Pset.reserve((size_t) P_size);
  int index;
  int cycle = 1000;
  for(int i = 0; i < P_size; i++) {
    ramdSol = randomSol(initial, lpiece, rlength);
    ramdSol.fitnessEval();
    localSearchBB(ramdSol, rlength, lot_s, lpiece, dpiece);
    index = linSearch(control, ramdSol.fitness);
    if (index == -1) {
      // Se agrega solución ya que no está en conjunto P
      Pset.push_back(ramdSol);
      control.push_back(make_pair(i,ramdSol.fitness));
      sort(control.begin(), control.end(), comparePairDouble);
      cout << "Solución " << i << " generada" << endl;
      cycle--;
    }
    else { // Hay un elemento que tiene el mismo fitness 
           // que la solución generada aleatoriamente. 
           // Por lo que verificamos si son verdaderamente
           // iguales.
      if (diff(ramdSol, Pset[index]) < min_diff_solutions) {
        // Si true entonces se descarta la solución 
        i--;
      }
      else {  // No son iguales => se agrega ramdSol a Pset
        Pset.push_back(ramdSol);
        control.push_back(make_pair(i,ramdSol.fitness));
        sort(control.begin(), control.end(), comparePairDouble);
        cycle--;
        cout << "Solución " << i << " generada" << endl;
      }
    }
  }
  return Pset;
}

// Sirve para buscar una solución en un 
// conjunto de soluciones.
bool find(Solution t, vector<Solution> set) {
  vector<Solution>::iterator it;
  for(it = set.begin(); it < set.end(); it++) {
    if (t.fitness == (*it).fitness) 
      if (diff(t,*it) == 0) return true;
  }
  return false;
}


// Comparación de pares de la forma p(id_pieza, longitud).
bool compareFitness(Solution a, Solution b) {
  return (a.fitness < b.fitness);
}

bool compareFitnessReverse(Solution a, Solution b) {
  return (a.fitness > b.fitness);
}

bool compareDivs(pair<int,int> a, pair<int,int> b){
  return (a.second > b.second);
}

