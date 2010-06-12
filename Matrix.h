#ifndef MATRIX
#define MATRIX
#include <vector>
#include "Byte.h"
using namespace std;

class Matrix {
 public:
  vector<pair<int,vector<Byte> > > info;

  void addCol (int num, vector <Byte> pref);
};

#endif
