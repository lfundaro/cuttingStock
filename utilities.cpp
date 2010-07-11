#include "utilities.h"
#include "FFD.h"
using namespace std;


// Comparación de pares de la forma p(id_pieza, longitud).
bool comparePair(pair<int,int> a, pair<int,int> b) {
  return (a.second > b.second);
}

// Ordena de manera decreciente.
bool comparePairDouble(pair<int,double> a, pair<int,double> b) {
  return (a.second < b.second);
}

// Dado un movimiento evaluado por la función 
// shiftNeighbourhood, esta función decide si el
// movimiento permite minizar la cantidad de 
// desperdicios generados.
bool group_quality(int * info, vector<int> &leftover,
                   vector<int> &used_rolls) {
  if (info[0] == -1)
    return false; //No se consiguio un movimiento
                  //valido o se acabaron los origenes
                  //y destinos (lo ultimo no se deberia
                  //detectar aca)

  if (info[4] < leftover[info[1]])
    return true;
  else {
    // Se compara si leftovers son iguales
    if (info[4] == leftover[info[1]]) {
      // Se compara por numero de rolls usados
      if (info[6] < used_rolls[info[1]])
        return true;
      else {
        if (info[6] == used_rolls[info[1]]) {
          // Se comparan los orígenes
          if (info[5] < used_rolls[info[0]])
            return true;
          else 
            return false;
        }
        return false;
      }
    }
    else 
      return false;
  }
}

// Generador de solución inicial.
// Consiste en generar M cutting groups 
// donde M es la cantidad de tipos de piezas.
// En cada grupo se coloca un tipo de pieza sin 
// importar la cantidad de rolls que estas ocupen
vector<vector<int>*> genInitSol(vector<int> &rlenght, 
                                vector<int> &lpiece,
                                vector<int> &dpiece,
                                vector<int> &leftover,
                                vector<int> &used_rolls,
                                vector<int> &rollType) {
  int M = lpiece.size();
  vector<vector<int>*> cgroups; // Solucion
  int i;
  int j;
  int k;
  pair <int,int> target;
  pair <int,int> ffdresult;
  int minimum;
  int left = 0;
  vector<int>::iterator it;

  for(i = 0; i < M; i++)
    cgroups.push_back(new vector<int>(M,0));

  for(i = 0; i < M; i++) {
    vector<int> pieceSet(M,0);
    pieceSet[i] = dpiece[i];
    target.first = MAX_INT;
    target.second = -1;
    // Se prueba con todos los tipos de rolls 
    // para ver donde se acomoda mejor la n cantidad 
    // de piezas del tipo i.
    for(j = 0; j < rlenght.size(); j++) {
      ffdresult = FFD(rlenght[j], lpiece, pieceSet);
      minimum = min(target.first,ffdresult.first);
      if (target.first != minimum){
        target.first = minimum;
        target.second = j;
        left = ffdresult.second;
      }
    }
    used_rolls[i] = target.first;
    leftover[i] = left;
    rollType[i] = target.second;
    
    //cgroups[i] = &pieceSet;
    //cout << dpiece[i] <<"\n";
    cgroups[i]->at(i) = dpiece[i];
  }
  return cgroups;
}

int* bestCutting(vector<int>& pieceSet, vector<int>& rlength,
                 vector<int>& lpiece){
  int i;
  int j;
  int nrolls = rlength.size();
  int npieces = lpiece.size();
  bool safe_move;
  pair<int,int> temp_result;
  int* result = new int[3];
  //result[0]: leftover
  //result[1]: rollos
  //result[2]: tipo de rollo
  result[0] = MAX_INT;

  //Recorro los rolls posibles
  for(j=0; j<nrolls; ++j){
    safe_move = true;
    //Recorro las piezas del pieceSet
    for(i=0; i<npieces; i++){
      if (pieceSet[i] > 0)//Si hay piezas
        if (lpiece[i] > rlength[j]){//Si el largo de esa pieza es muy grande
          safe_move = false;
          break;
        }
    }
    if (safe_move){
      temp_result = FFD(rlength[j],lpiece,pieceSet);
      if (result[0] > temp_result.second){
        result[0] = temp_result.second; //leftover
        result[1] = temp_result.first;  //rollos
        result[2] = j; //tipo de rollo
      }
    }
  }

  return result;
}

// Función que Calcula mínimo
inline int min(int a, int b) {
  if (a < b) 
    return a;
  else
    return b;
}

// Función que Calcula máximo
inline int max(int a, int b) {
  if (a > b) 
    return a;
  else
    return b;
}

// Libera los vectores que representan un roll 
// con piezas en un determinado cutting group
void free_vector(vector<vector<int>*> &a) {
  vector<vector<int>*>::iterator it;
  for(it = a.begin(); it != a.end(); it++) 
    delete (*it);
}

// Chequea si un grupo de corte está vacío.
int notEmptyColumn(vector<int> column) {
  int decision = 0;
  for(int i = 0; i < column.size(); i++) {
    decision |= column[i];
  }
  return decision;
}

vector<int> calcVariety(vector<vector<int> > &cgs) {
  vector<int>::iterator itset;
  vector<int> variety(cgs.size(), 0);
  int count;
  for(int i = 0; i < cgs.size(); i++) {
    count = 0;
    for(itset = cgs[i].begin(); itset < cgs[i].end(); itset++) {
      if (*itset) {
        count++;
      }
    }
    // Actualización de variety
    variety[i] = count;
  }
  return variety;
}


vector<vector<int> > calcDiversity(int rolltypes, 
                                   vector<vector<int> > &cgs,
                                   vector<int> &rollType,
                                   vector<int> &used_rolls) 
{
  vector<vector<int> > diversity;
  int M = cgs.size();
  for(int i = 0; i < M; i++) 
    diversity.push_back(vector<int>(M,0));
  int cgsize = used_rolls.size();
  // Por cada tipo de roll se busca que 
  // cutting group lo está usando actualmente.
  for(int i = 0; i < rolltypes; i++) {
    // Se busca si hay un cutting group que use el roll i
    for(int j = 0; j < cgsize; j++) {
      // Se verifica que el cutting group j use al menos 
      // un roll.
      if (used_rolls[j]) {
        // Si el roll i coincide con el del cgroup actual
        if (rollType[j] == i) {
          for(int k = 0; k < cgsize; k++) {
            if (cgs[j][k]) diversity[i][k] += cgs[j][k];
          }
        }
      }
    }
  }
  return diversity;
}

// Búsqueda lineal sobre una estructura compuesta 
// de pares (int,double)
int linSearch(vector<pair<int,double> > control, 
              double target) {
  vector<pair<int,double> >::iterator it;
  for(it = control.begin(); it < control.end(); it++) 
    if ((*it).second == target) return (*it).first;

  return -1;  // En caso de no estar el elemento
}

double variance(vector<int>& v){
  double n = (double)v.size();
  
  double p = 1.0/(double)n;
  double hope_sum = 0;
  double mean_sum = 0;
  
  for (int i = 0; i<n; i++){
    hope_sum += pow((double)v[i],2.0) * p;
    mean_sum += (double)v[i];
  }

  cout << hope_sum-pow(mean_sum/n,2.0) <<"\n";
  return hope_sum-pow(mean_sum/n,2.0);
}

double variance(vector<double>& v){
  double n = (double)v.size();
  
  double p = 1.0/n;
  double hope_sum = 0;
  double mean_sum = 0;
  
  for (int i = 0; i<n; i++){
    hope_sum += pow(v[i],2.0) * p;
    mean_sum += v[i];
  }

  cout << hope_sum-pow(mean_sum/n,2.0) <<"\n";

  return hope_sum-pow(mean_sum/n,2.0);
}

double cg_variace(vector< vector<int> >& cg){
  int ngroups = cg.size();
  vector<double> vars(ngroups);

  for (int i = 0; i<ngroups; ++i){
    vars[i] = variance(cg[i]);
  }

  return variance(vars);
}

void printCG(vector< vector<int> > &cg){
  for (int i=0; i<cg.size(); ++i){
    cout << "cg "<< i << ":";
      for (int j=0; j<cg[i].size(); ++j){
      cout << cg[i][j] <<" ";
    }
    cout << "\n";
  }
}
