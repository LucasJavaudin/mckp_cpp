#include <vector>

using namespace std;

pair<double,double> MCKP_Greedy_Algorithm(Dataset, double);

struct aideTri {
	int operator() (const pair<Item*,double>, const pair<Item*,double>) const;
};
