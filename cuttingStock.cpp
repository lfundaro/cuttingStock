#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FFD.h"
#include "utilities.h"
#include "localSearchBB.h"
#include "genetic.h"
#include "scatterSearch.h"
#include "Solution.h"
using namespace std;

int main(int argc, char *argv[]) {
  char * line = (char *) malloc(sizeof(char)*60);
  ifstream instance(argv[1]);
  int i;
  if (instance.is_open()) {
    int nroll_type; // Número de Stock Roll type
    instance.getline(line, 60);
    sscanf(line,"%*s %d", &nroll_type);
    int ntpieces;  // Número de tipo piezas 
    instance.getline(line, 60);
    sscanf(line,"%*s %d", &ntpieces);
    // Se leen nroll_type líneas para almacenar 
    // las longitudes de cada tipo de roll.
    vector<int> rlength(nroll_type);
    for(i = 0; i < nroll_type; i++) {
      instance.getline(line, 60);
      sscanf(line,"%d", &rlength[i]);
    }
    vector<int> lot_s(ntpieces); // Tamaño de lote
    vector<int> lpiece(ntpieces); // Longitud de pieza
    vector<int> dpiece(ntpieces); // Demanda de pieza
    // Se leen ntpieces líneas para almacenar 
    // la longitud de cada pieza, demanda y tamaño de 
    // lote
    for(i = 0; i < ntpieces; i++) {
      instance.getline(line, 60);
      sscanf(line,"%d %d %d",&lpiece[i],&dpiece[i],&lot_s[i]);
    }

    // Arreglo de desperdicios por cutting group
    vector<int> leftover(ntpieces);
    for(i = 0; i < ntpieces; i++) 
      leftover[i] = 0;
    
    // Vector de Rolls usados por cada cutting group
    vector<int> used_rolls(ntpieces);
    for(i = 0; i < ntpieces; i++) 
      used_rolls[i] = 0;
    
    // Vector que indica el tipo de roll que se usa 
    // en la i-ésima posición de la solución
    vector<int> rollType(ntpieces);
    for(i = 0; i < ntpieces; i++) 
      rollType[i] = -1;

    vector<int> variety(ntpieces,1);
    
    srand(time(NULL));

    Solution f = scatterSearch(20,10, rlength,lpiece,
                               dpiece,lot_s);
    f.printAsPaper(rlength,lpiece);
    free(line);
    instance.close();
  }
    else 
    cout << "Error leyendo instancia" << endl;
}
