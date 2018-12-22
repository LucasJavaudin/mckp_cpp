#include <vector>

using namespace std;

pair<Allocation,vector<double>> MCKP_Greedy_Algorithm(Dataset, double);

struct aideTri {
	int operator() (const pair<Item*,double>, const pair<Item*,double>) const;
};
