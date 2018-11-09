#include <vector>
#include <algorithm>

#include "data.h"

using namespace std;

void GreedyAlgorithm(Dataset d, double max_Weight){
    vector<Class> R; // @R va contenir les enveloppes convexes de chaque classes
    vector<vector<double>> p;
    vector<vector<double>> w;
    vector<double> e;

    double residualCapacity = max_Weight ; // représente la capacité résiduelle
    for(int i=0; i<d.getNbClasses(); i++){

        // Etape 1 : construction des enveloppes convexes
        R.push_back((d[i])->upperBound());

        // Etape 2 : construction de p et w
        for(int j=1; i<R[i].getNbItems(); j++){
            p[i].push_back(((R[i])[j])->getValue() - ((R[i])[j-1])->getValue());
            w[i].push_back(((R[i])[j])->getWeight() - ((R[i])[j-1])->getWeight());
            e.push_back(p[i][j]/w[i][j]);
        }
        residualCapacity -= ((R[i])[0])->getWeight();
    }

    // Etape 3
    // sorting items (in each class) according to decreasing p/w
    sort(e.begin(),e.end()); // question, comment les identifier après le tri ?



}
