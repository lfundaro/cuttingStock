#include "FFD.h"
#include "utilities.h"
using namespace std;


// Algoritmo First-Fit Decreasing. Este algoritmo construye     
// un "Cutting Group" ordenando el conjunto factible de piezas  
// en orden decreciente. Luego asigna secuencialmente todas     
// las piezas en dicho orden usando la menor cantidad de rolls  
// posibles siempre y cuando exista espacio en el k-ésimo roll.
// Se devuelve como resultado un par que tiene el número de rolls 
// empleados y el desperdicio entre todos los roles.
pair <int, int>  FFD(int rollSize, vector<int> lpiece,
		     vector<int> pieceSet) {
  vector<vector<int>*> output;
  if (!emptyPieceSet(pieceSet)) // El Cutting group está vacío 
    return make_pair(0,0);
  else {
    int i;
    int npieces = 0;
    // Número de piezas en conjunto factible
    for(i = 0; i < pieceSet.size(); i++)  
      npieces += pieceSet[i];
      
    // Número de tipo de piezas
    int M = lpiece.size();           
    // Construcción de vector tl de par(tipo,longitud) 
    vector <pair <int,int> > tl;
    for(i = 0; i < M; i++) {
      // if (pieceSet[i] != 0)    // Sólo se toman piezas existentes
        tl.push_back(make_pair(i, lpiece[i]));
    }

    // Ordenamiento del arreglo tl
    sort(tl.begin(), tl.end(), comparePair);

    // Se arregla pieceSet para que exista correspondencia
    // de índices con tl.
    vector<pair<int,int> >::iterator it;
    vector<int> npieceSet;
    for(it = tl.begin(); it != tl.end(); it++) 
      npieceSet.push_back(pieceSet[(*it).first]);
    
    int r = 0;    // Tipo de pieza actual
    int v;        // Lleva la cuenta de cuántas piezas de un tipo 
                  // se ha usado.       
    int k;        // Número de roll actual
    int nroll = 1;  // Número de rolls usados
    int allocation; // Piezas a poner en roll 
    output.push_back(new vector<int>(M, 0)); // Primer roll
    while(r < M) {    // Step 3
      if (npieceSet[r] == 0) {r++; continue;}
      v = npieceSet[r];
      k = -1;
      while(v > 0) {
        k++;
        if (k >= nroll) { 
          nroll++;
          output.push_back(new vector<int>(M, 0)); // Nuevo roll
        }
        int sum = 0; // Sumatoria acumulada
        for(i = 0; i <= r - 1; i++) {
          sum += output[k]->at(tl[i].first) * lpiece[tl[i].first];
        }
        allocation = (rollSize - sum)/lpiece[tl[i].first];
        if (v <= allocation) 
          output[k]->at(tl[r].first) = v;
        else
          output[k]->at(tl[r].first) = allocation;

        v -= allocation;
      }
      r += 1;
    }

    pair <int,int> result;
    result.first = nroll;
    result.second = leftOver(output, rollSize, lpiece);
    free_vector(output);
    return result;
  }
} 


// Libera los vectores que representan un roll 
// con piezas en un determinado cutting group
void free_vector(vector<vector<int>*> &a) {
  vector<vector<int>*>::iterator it;
  for(it = a.begin(); it != a.end(); it++) 
    delete (*it);
}

// Calcula el desperdicio (en espacio) que un 
// cutting group puede tener.
int leftOver(vector<vector<int>*> g, int rlength, vector<int> lpiece) {
  vector<vector<int>*>::iterator it;
  int rollIt;
  int v;
  int leftover = 0;
  for(it = g.begin(); it != g.end(); it++) {
    v = rlength;
    for(rollIt = 0; rollIt < (**it).size(); rollIt++) {
      if ((**it)[rollIt] != 0) 
        v -= lpiece[rollIt] * (**it)[rollIt];
    }
    if (v != rlength)
      leftover += v;
  }
  return leftover;
}

// Chequea si un conjunto de piezas factibles 
// está vació o no.
int emptyPieceSet(vector<int> pieceSet) {
  int k = 0;
  for(int i = 0; i < pieceSet.size(); i++) 
    k = k || pieceSet[i];
  return k;

    

}

