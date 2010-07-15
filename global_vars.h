#ifndef GLOBAL_VARS
#define GLOBAL_VARS

/*Scatter*/
extern int Psize;
extern int refSize;
extern int recons; //Numero de reconstrucciones del refSet
extern float min_diff_solutions; //Ver Nota al final del archivo

/*Constrains*/
extern int max_open_stacks;// Maxima cantidad de open stacks abiertos simultaneamente
extern int min_size_diff;  // Minima diferencia de tamano entre los cortes de un mismo cutting group

/*Local Search*/
extern int localSearchMaxIts;// Cantidad maxima de iteraciones de local search

/*Move evaluation*/
extern int dst_lo_mod;   // Modificador del valor del indice de leftover para el destino 
extern int dst_frg_mod;  // Modificador del valor del indice de fragmentacion para el destino
extern int orig_lo_mod;  // Modificador del valor del indice de leftover para el origen
extern int orig_frg_mod; // Modificador del valor del indice de fragmentacion para el origen
#endif

//NOTA: min_diff_solutions es:
/*La diferencia minima entre dos soluciones
  para que el algoritmo de generacion del
  conjunto P la acepte
*/
