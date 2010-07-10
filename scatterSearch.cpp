#include "scatterSearch.h"
using namespace std;

Solution scatterSearch(int P_size, int b, 
                       vector<int> &rlength,
                       vector<int> &lpiece, 
                       vector<int> &dpiece) {
  vector<Solution> P = genPset(rlength,lpiece,dpiece,P_size);
}

int diff(Solution& sol1, Solution& sol2){

  int diff = 0;
  int ngroups = min(sol1.cgs.size(),sol2.cgs.size());
  int npieces = sol1.cgs[0].size();

  for (int i = 0; i<ngroups; ++i){
    for (int j = 0; j<npieces; ++j){
      diff += abs(sol1.diversity[i][j]-sol2.diversity[i][j]);
    }
  }

  return diff;
}

vector< pair<int,int> > diversity(vector< Solution >& refSet,
				  vector< Solution >& sols){
  int nsols = sols.size();
  int nRefSet = refSet.size();
  int div = 0;
  vector< pair<int,int> > divs(nRefSet,pair<int,int>());

  for (int i = 0; i<nRefSet; ++i){
    for (int j = 0; j<nsols; ++j){
      div += diff(sols[j],refSet[i]);
    }
    divs[i].first = i;
    divs[i].second = div;
    div = 0;
  }

  sort(divs.begin(),divs.end(),compareDivs);

  return divs;
}

vector<Solution> genPset(vector<int> &rlength,
                         vector<int> &lpiece,
                         vector<int> &dpiece,
                         int P_size) {
  vector<Solution> Pset;
  Solution initial = Solution(rlength, lpiece, dpiece);
  Solution ramdSol;
  Pset.reserve((size_t) P_size);
  for(int i = 0; i < P_size; i++) {
    ramdSol = randomSol(initial, lpiece, rlength);
  }
}

bool compareDivs(pair<int,int> a, pair<int,int> b){
  return (a.second > b.second);
}
