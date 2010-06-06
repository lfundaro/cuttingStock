#include "FFD.h"
#include "utilities.h"
using namespace std;


// Algoritmo First-Fit Decreasing. Este algoritmo construye     
// un "Cutting Group" ordenando el conjunto factible de piezas  
// en orden decreciente. Luego asigna secuencialmente todas     
// las piezas en dicho orden usando la menor cantidad de rolls  
// posibles siempre y cuando exista espacio en el k-ésimo roll.
pair <int, vector<vector<int>* > >  FFD(int rollSize, vector<int> lpiece,vector<int> pieceSet) {
  pair <int, vector<vector<int>* > > output = make_pair(0, vector<vector<int>* >());
  if (pieceSet.size() == 0) // El Cutting group está vacío
    return output;
  else {
    int i;
    int npieces = 0;
    // Número de piezas en conjunto factible
    for(i = 0; i < pieceSet.size(); i++)  
      npieces += pieceSet[i];
      
    // Número de tipo de piezas
      int M = lpiece.size();           
    // Construcción de vector tl de par(tipo,longitud) 
    vector <pair <int,int> > tl(M);
    for(i = 0; i < M; i++) {
      if (pieceSet[i] != 0)    // Sólo se toman nodos existentes
        tl[i] = make_pair(i, lpiece[i]);
    }

    // Ordenamiento del arreglo tl
    sort(tl.begin(), tl.end(), comparePair);
    
    int r = 0;    // Tipo de pieza actual
    int v;        // Lleva la cuenta de cuántas piezas de un tipo 
                  // se ha usado.       
    int k;        // Número de roll actual
    int nroll = 1;   // Número de rolls usados
    int allocation; // Piezas a poner en roll 
    output.second.push_back(new vector<int>(M, 0)); // Primer roll
    while(r < M) {    // Step 3
      v = pieceSet[tl[r].first];
      k = -1;
      while(v > 0) {
        k++;
        cout << "h" << endl;
        if (k >= nroll) { 
          nroll++;
          output.second.push_back(new vector<int>(M, 0)); // Nuevo roll
        }
        int sum = 0; // Sumatoria acumulada
        for(i = 0; i <= r - 1; i++) {
          sum += output.second[k]->at(i) * lpiece[tl[i].first];
        }
        allocation = (rollSize - sum)/lpiece[tl[i].first];
        if (v <= allocation) 
          output.second[k]->at(r) = v;
        else
          output.second[k]->at(r) = allocation;

        v -= allocation;
      }
      r += 1;
    }
    output.first = nroll;
  }
  return output;
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
    leftover += v;
  }
  return leftover;
}

