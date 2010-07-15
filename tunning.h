#ifndef TUNNING
#define TUNNING
#include "global_vars.h"

//cuttingStock main
#define METAHEURISTIC SCATTER

//Local Search
#define LSBB_MAXIT localSearchMaxIts //4000

//Constrain de open stacks y diferencia de tamanos
#define MAX_OS max_open_stacks //4
#define MIN_SD min_size_diff   //50       

//Calculo de puntaje de origenes y destinos
#define COMB_OP +       // Operacion para combinar las dos partes de la funcion de puntaje de origenes y destinos
#define OUTSIDE_FUNC    // Funcion que se le aplica al resultado de la funcion de puntaje de origenes y destinos
#define DST_LO_MOD dst_lo_mod   
#define DST_FRG_MOD dst_frg_mod   
#define ORIG_LO_MOD orig_lo_mod   
#define ORIG_FRG_MOD orig_frg_mod  

#endif

//NOTA: min_diff_solutions es:
/*La diferencia minima entre dos soluciones
  para que el algoritmo de generacion del
  conjunto P la acepte
*/
