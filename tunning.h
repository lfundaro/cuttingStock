#ifndef TUNNING
#define TUNNING

//ILS
#define ILS_MAXIT 1000  // Cantidad de interaciones de ILS
#define MAX_PERTURB 4   // Fuerza de la perturbacion
#define INIT_TEMP 1000  // Temperatura inicial (ver NOTA)
#define EXP_DESC 0.999  // Descenso exponencial de la temperatura (ver NOTA)

//GRASP
#define GRASP_MAXIT 1000  // Cantidad de iteraciones de GRASP
#define RCL_SIZE 4        // Tamano de la lista RCL

//Local Search
#define LSBB_MAXIT 1000 // Cantidad maxima de iteraciones de local search

#endif


/*
  El empeoramiento de la solucion estrella se
  decide con la probabilidad de recosido simulado
*/
