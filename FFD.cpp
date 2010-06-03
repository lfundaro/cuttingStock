#include "FFD.h"
#include "utilities.h"
using namespace std;


// Algoritmo First-Fit Decreasing. Este algoritmo construye     
// un "Cutting Group" ordenando el conjunto factible de piezas  
// en orden decreciente. Luego asigna secuencialmente todas     
// las piezas en dicho orden usando la menor cantidad de rolls  
// posibles siempre y cuando exista espacio en el k-ésimo roll.
//pair <int, vector<vector<int> > > 
int FFD(int rollSize, int *lpiece,int *pieceSet, int npieces) {
  // Construcción de vector tl de par(tipo,longitud) 
  vector <pair <int,int> > tl(npieces);
  for(int i = 0; i < npieces; i++) {
    tl[i] = make_pair(pieceSet[i], lpiece[i]);
  }
  // Ordenamiento del arreglo tl
  sort(tl.begin(), tl.end(), comparePair);
  return 5;
} 

