#include <vector>
#include "utilities.h"
#include "FFD.h"
using namespace std;

class Solution {
 public:
  vector<int> leftover;
  vector<int> used_rolls;
  vector<int> rollType;
  vector<vector<int>*> cgs; // cutting group set
  int size;

  Solution(vector<int> &rlength, vector<int> &lpiece,
           vector<int> &dpiece);

  ~Solution();
};
