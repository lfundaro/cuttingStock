#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FFD.h"
#include "utilities.h"
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
    vector<int> rlenght(nroll_type);
    for(i = 0; i < nroll_type; i++) {
      instance.getline(line, 60);
      sscanf(line,"%d", &rlenght[i]);
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
    
    vector<int> a;
    a.push_back(5);
    a.push_back(3);
    vector<int> f;
    f.push_back(2);
    f.push_back(4);
     
    pair <int,int> c = FFD(10,a,f);
    
   
    // vector<vector<int>*> cg = genInitSol(rlenght,lpiece,
    //                                      dpiece, leftover,
    //                                      used_rolls);
    // for(i = 0; i < ntpieces; i++) {
    //   if (used_rolls[i]) {
    //     cout << "Tipo " << i << endl;
    //     cout << used_rolls[i] << endl;
    //   }
    // }
    //    free_vector(cg);
    free(line);
    instance.close();
  }
  else 
    cout << "Error leyendo instancia" << endl;

}







    // vector<int> f;
    // f.push_back(4);
    // f.push_back(2);
    // vector<int> a;
    // a.push_back(4);
    // a.push_back(2);
    
    // pair<int,vector<vector<int>*> > c = FFD(11,f,a);
    // cout << leftOver(c.second,11, f) << endl;

