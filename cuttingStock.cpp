#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FFD.h"
using namespace std;

int main(int argc, char *argv[]) {
  char * line = (char *) malloc(sizeof(char)*60);
  ifstream instance(argv[1]);
  if (instance.is_open()) {
    int nroll_type; // Número de Stock Roll type
    instance.getline(line, 60);
    sscanf(line,"%*s %d", &nroll_type);
    int npieces;  // Número de piezas 
    instance.getline(line, 60);
    sscanf(line,"%*s %d", &npieces);
    // Se leen nroll_type líneas para almacenar 
    // las longitudes de cada tipo de roll.
    int * rlenght = new int[nroll_type];
    for(int i = 0; i < nroll_type; i++) {
      instance.getline(line, 60);
      sscanf(line,"%d", &rlenght[i]);
    }
    int * lot_s = new int[npieces]; // Tamaño de lote
    int * lpiece = new int[npieces]; // Longitud de pieza
    int * dpiece = new int[npieces]; // Demanda de pieza
    // Se leen npieces líneas para almacenar 
    // la longitud de cada pieza, demanda y tamaño de 
    // lote
    for(int i = 0; i < npieces; i++) {
      instance.getline(line, 60);
      sscanf(line,"%d %d %d",&lpiece[i],&dpiece[i],&lot_s[i]);
    }
    // Representación de la solución (cgroups)
    int **cgroups = new int*[npieces];
    for(int i = 0; i < npieces; i++) 
      cgroups[i] = new int[npieces];
    
    vector<int> f;
    f.push_back(4);
    f.push_back(2);
    vector<int> a;
    a.push_back(4);
    a.push_back(2);
    
    pair<int,vector<vector<int>*> > c = FFD(9,f,a);

    free_vector(c.second);
    
    instance.close();
    delete[] lot_s;
    delete[] lpiece;
    delete[] dpiece;
    delete[] rlenght;
    for(int i = 0; i < npieces; i++) 
      delete[] cgroups[i];
    delete[] cgroups;
    free(line);
  }
  else 
    cout << "Error leyendo instancia" << endl;

}
