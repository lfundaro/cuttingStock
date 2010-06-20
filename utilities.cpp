#include "utilities.h"
using namespace std;

vector<vector<int>*> duplicate(vector<vector<int>*> &solution,
			       int items){
  int i;
  int j;
  vector<vector<int>*> dup;

  for (i=0; i<items; ++i){
    dup.push_back(new vector<int>);
    for (j=0; j<items; ++j){
      dup.at(i)->push_back(solution.at(i)->at(j));
    }
  }
  
  return dup;
}

void overwrite(vector<vector<int>*> &orig,
	       vector<vector<int>*> &dst,
	       int items){
  int i;
  int j;

  for (i=0; i<items; ++i){
    for (j=0; j<items; ++j){
      dst.at(i)->at(j) = orig.at(i)->at(j);
    }
  }
}

void substitute(vector<vector<int>*> &orig,
		vector<vector<int>*> &dst,
		int items){
  int i;
  int j;
  
  int norig = orig.size();

  dst.clear();

  for (i=0; i<norig; ++i){
    dst.push_back(new vector<int>(0));
    for (j=0; j<items; ++j){
      dst.at(i)->push_back(orig.at(i)->at(j));
    }
  }
}

// Comparación de pares de la forma p(id_pieza, longitud).
bool comparePair(pair<int,int> a, pair<int,int> b) {
  return (a.second > b.second);
}

// Dado un movimiento evaluado por la función 
// shiftNeighbourhood, esta función decide si el
// movimiento permite minizar la cantidad de 
// desperdicios generados.
bool group_quality(int * info, vector<int> &leftover,
                   vector<int> &used_rolls) {
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
  int left =  MAX_INT;
  vector<int>::iterator it;

  bool safe_move;
  int npieces = lpiece.size();

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
      safe_move = true;
      //Recorro las piezas del pieceSet
      for(k=0; k<M; k++){
	if (pieceSet[k] > 0)//Si hay piezas
	  if (lpiece[k] > rlenght[j]){//Si el largo de esa pieza es muy grande
	    safe_move = false;
	    break;
	  }
      }

      if (safe_move){
	ffdresult = FFD(rlenght[j], lpiece, pieceSet);
	minimum = min(left,ffdresult.second);
	if (left != minimum){
	  target.first = minimum;
	  target.second = j;
	  left = ffdresult.second;
	}
      }
    }
    cout << left << " "<< i <<" left\n";
    used_rolls[i] = target.first;
    leftover[i] = left;
    rollType[i] = target.second;
    
    //cgroups[i] = &pieceSet;
    //cout << dpiece[i] <<"\n";
    cgroups[i]->at(i) = dpiece[i];
  }

  return cgroups;
}


// Función que Calcula mínimo
int min(int a, int b) {
  if (a < b) 
    return a;
  else
    return b;
}

int max_item(vector<int>& vect) {

  int i;
  int vect_size = vect.size();
  int max = MIN_INT;
  
  for (i=0; i<vect_size; ++i){
    if (vect[i] > max)
      max = vect[i];
  }

  return max;
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

