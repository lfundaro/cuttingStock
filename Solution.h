#include <vector>
using namespace std;

class Solution {
 public:
  vector<int> leftover;
  vector<int> used_rolls;
  vector<int> rollType;
  vector<vector<int>*> cgs; // cutting group set

  Solution(vector<int> &rlength, vector<int> &lpiece,
           vector<int> &dpiece);

  ~Solution();
};
