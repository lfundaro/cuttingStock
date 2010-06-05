#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FFD.h"
using namespace std;

int main(int argc, char *argv[]) {
  char * line = (char *) malloc(sizeof(char)*60);
  ifstream instance(argv[1]);
  int i;
  if (instance.is_open()) {
    int nroll_type; // Número de Stock Roll type
    instance.getline(line, 60);
    sscanf(line,"%*s %d", &nroll_type);
    int npieces;  // Número de piezas 
    instance.getline(line, 60);
    sscanf(line,"%*s %d", &npieces);
    // Se leen nroll_type líneas para almacenar 
    // las longitudes de cada tipo de roll.
    vector<int> rlenght(nroll_type);
    for(i = 0; i < nroll_type; i++) {
      instance.getline(line, 60);
      sscanf(line,"%d", &rlenght[i]);
    }
    vector<int> lot_s(npieces); // Tamaño de lote
    vector<int> lpiece(npieces); // Longitud de pieza
    vector<int> dpiece(npieces); // Demanda de pieza
    // Se leen npieces líneas para almacenar 
    // la longitud de cada pieza, demanda y tamaño de 
    // lote
    for(i = 0; i < npieces; i++) {
      instance.getline(line, 60);
      sscanf(line,"%d %d %d",&lpiece[i],&dpiece[i],&lot_s[i]);
    }
    // Representación de la solución (cgroups)
    vector<vector<int>*> cgroups(npieces);
    for(i = 0; i < npieces; i++) 
      cgroups[i] = new vector<int>(npieces);

    // Arreglo de desperdicios por cutting group
    vector<int> leftover(npieces);
    for(i = 0; i < npieces; i++) 
      leftover[i] = 0;

    vector<int> f;
    f.push_back(4);
    f.push_back(2);
    vector<int> a;
    a.push_back(4);
    a.push_back(2);
    
    pair<int,vector<vector<int>*> > c = FFD(11,f,a);
    cout << leftOver(c.second,11, f) << endl;

    free_vector(c.second);
    free_vector(cgroups);
    free(line);
    instance.close();
  }
  else 
    cout << "Error leyendo instancia" << endl;

}
