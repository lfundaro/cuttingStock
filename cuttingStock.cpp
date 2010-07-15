#include <iostream>
#include <fstream>
#include <cstdlib>
#include "FFD.h"
#include "utilities.h"
#include "localSearchBB.h"
#include "genetic.h"
#include "scatterSearch.h"
#include "Solution.h"
#include "tunning.h"
using namespace std;

int main(int argc, char *argv[]) {
  char * line = (char *) malloc(sizeof(char)*60);
  ifstream instance(argv[1]);
  ifstream confFile(argv[2]);

  if (confFile.is_open()) { 
    //Tamano del conjunto P
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &Psize);

    //Tamano del refSet
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &refSize);

    //Numero de reconstruciones del refSet
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &recons);

    //Diferencia minima entre dos soluciones
    //para que el algoritmo de generacion del
    //conjunto P la acepte
    confFile.getline(line, 60);
    sscanf(line,"%*s %f", &min_diff_solutions);

    //Maxima cantidad de open stacks simultaneos
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &max_open_stacks);

    //Minima diferencia de tamanos los rollos
    //que estan en un cutting group
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &min_size_diff);

    //Iteraciones de localSearch
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &localSearchMaxIts);

    // Modificador del valor del indice de leftover para el destino 
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &dst_lo_mod);

    // Modificador del valor del indice de fragmentacion para el destino
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &dst_frg_mod);

    // Modificador del valor del indice de leftover para el origen
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &orig_lo_mod);

    // Modificador del valor del indice de fragmentacion para el origen
    confFile.getline(line, 60);
    sscanf(line,"%*s %d", &orig_frg_mod);
  }
  else 
    cout << "Error leyendo archivo de configuracion" << endl;

  cout << "Psize: "<< Psize <<endl;
  cout << "refSize: "<< refSize <<endl;
  cout << "recons: "<< recons <<endl;
  cout << "min_diff_solutions: "<< ((double)min_diff_solutions) <<endl;
  cout << "max_open_stacks: "<< max_open_stacks <<endl;
  cout << "min_size_diff: "<< min_size_diff <<endl;
  cout << "localSearchMaxIts: "<< localSearchMaxIts <<endl;
  cout << "dst_lo_mod: "<< dst_lo_mod <<endl;
  cout << "dst_frg_mod: "<< dst_frg_mod <<endl;
  cout << "orig_lo_mod: "<< orig_lo_mod <<endl;
  cout << "orig_frg_mod: "<< orig_frg_mod <<endl;

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

    Solution f = scatterSearch(Psize,refSize, rlength,lpiece,
                               dpiece,lot_s);
    f.printAsPaper(rlength,lpiece);
    free(line);
    instance.close();
  }
  else 
    cout << "Error leyendo instancia" << endl;
}
