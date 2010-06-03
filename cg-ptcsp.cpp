#include <iostream>
#include <fstream>
#include <cstdlib>
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
    instance.close();
    delete[] lot_s;
    delete[] lpiece;
    delete[] dpiece;
    delete[] rlenght;
    free(line);
  }
  else 
    cout << "Error leyendo instancia" << endl;

}