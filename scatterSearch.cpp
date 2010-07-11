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

  int piece_distr_diff = 0;
  double variance_diff;
  double diff;

  if(sol1.diversity.size()>1 &&
     sol2.diversity.size()>1){//Si hay mas de un roll
    for (int i = 0; i<ngroups; ++i){
      for (int j = 0; j<npieces; ++j){
	piece_distr_diff += abs(sol1.diversity[i][j]-sol2.diversity[i][j]);
      }
    }
  }
  else
    piece_distr_diff = 0;

  variance_diff = fabs(cg_variace(sol1.cgs)-cg_variace(sol2.cgs));
    
  return DIFF_VAR*variance_diff + DIFF_DISTR*((double)piece_distr_diff);
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
  P.resize(Psize-(b/2));    //Elimino los b/2 mejoes
  diversity(P,refSet,divs); //Busco los b/2 mas variados

  for (int i = b/2; i < b; ++i){
    int sol = P[divs[i-b/2].first];
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

Solution scatterSearch(int P_size, int b, 
                       vector<int> &rlength,
                       vector<int> &lpiece, 
                       vector<int> &dpiece,
                       vector<int> &lot_s) {
  vector<Solution> P = genPset(rlength,lpiece,dpiece,P_size,
                               lot_s);
}

vector<Solution> genPset(vector<int> &rlength,
                         vector<int> &lpiece,
                         vector<int> &dpiece,
                         int P_size,
                         vector<int> &lot_s) {
  vector<Solution> Pset;
  Solution initial = Solution(rlength, lpiece, dpiece);
  Solution ramdSol;
  vector<pair<int,double> > control;
  Pset.reserve((size_t) P_size);
  int index;
  for(int i = 0; i < P_size; i++) {
    ramdSol = randomSol(initial, lpiece, rlength);
    //    localSearchBB(ramdSol, rlength, lot_s, lpiece, dpiece);
    index = linSearch(control, ramdSol.fitness);
    if (index == -1) {
      // Se agrega solución ya que no está en conjunto P
      Pset.push_back(ramdSol);
      control.push_back(make_pair(i,ramdSol.fitness));
      sort(control.begin(), control.end(), comparePairDouble);
    }
    else { // Hay un elemento que tiene el mismo fitness 
           // que la solución generada aleatoriamente. 
           // Por lo que verificamos si son verdaderamente
           // iguales.
      if (diff(ramdSol, Pset[index]) == 0)
        // Si true entonces se descarta la solución 
        i--;
      else {  // No son iguales => se agrega ramdSol a Pset
        Pset.push_back(ramdSol);
        control.push_back(make_pair(i,ramdSol.fitness));
        sort(control.begin(), control.end(), comparePairDouble);
      }
    }
  }
}
