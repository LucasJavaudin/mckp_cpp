#include <vector>
#include <algorithm>
#include <iostream>

#include "data.h"

using namespace std;

pair<double,double> MCKP_Greedy_Algorithm(Dataset d, double max_Weight){
    vector<Class> R; // @R va contenir les enveloppes convexes de chaque classes
    vector<pair<Item*,double>> pairItem;
    vector<vector<double>> inKnapsack(d.getNbClasses(),{1}); // vector : the  element [i][j] = 1 if the i-th item of
    // j-th class is in the knapsack, 0 if not. (we can take a fraction of the latest item put in the knapsack when
    // we exceed the capacity of the knapsack

    // OUTPUTS
    double residualCapacity = max_Weight ; // capacity still free with the items in the knapsack
    // (max_Weight - current weight of the knapsack)
    double totalValue = 0; //sum of values of the items which are in the knapsack

    // Begin of the algorithm
    for(int i=0; i<d.getNbClasses(); i++){

        // Etape 1 : construction of upper bound in each class
        R.push_back((d[i])->upperBound());

        // Etape 2 : construction of efficacity vector
        for(unsigned int j=1; j<R[i].getNbItems(); j++){
            // R[i] is already sorted by increasing weights because of we sorted classes[i] before deriving the upper
            // bound of this class
            double diff_p = R[i][j]->getValue() - R[i][j-1]->getValue(); // value[i][j] - value[i][j-1]
            double diff_w = R[i][j]->getWeight() - R[i][j-1]->getWeight(); // weight[i][j] - weight[i][j-1]
            // We can't stock thoses numbers for each i and j (as describe in the algorithm) because of a lack of memory

            pairItem.push_back(make_pair(R[i][j],diff_p/diff_w)); // we stock each Item with its efficacity (we don't
            // care about their Class, we'll sort them according to decreasing efficacity thanks to @AideTri)
            inKnapsack[i].push_back(0);
        }

        // At the beginning, we put the lightest items of each upper bound in the knapsack
        residualCapacity -= ((R[i])[0])->getWeight();
        totalValue += (R[i][0])->getValue();

    }

    // Etape 3
    sort(pairItem.begin(),pairItem.end()); // sorting items (in each class) according to decreasing p/w (efficacity)

    for (unsigned int l = 0;l<pairItem.size();l++){

        // we're looking for the Class of the l-th item
        for(unsigned int i=0; i<R.size(); i++){

            for(unsigned int j=0; j<(R[i].getNbItems()); j++){

                if(pairItem[l].first->getIndex()==R[i][j]->getIndex()){
                    double diff_w = R[i][j]->getWeight() - R[i][j-1]->getWeight();
                    double diff_p = R[i][j]->getValue() - R[i][j-1]->getValue();
                    if(diff_w<=residualCapacity){
                        // Etape 4
                        inKnapsack[i][j] = 0;
                        inKnapsack[i][j-1] = 1;
                        residualCapacity -= diff_w;
                        totalValue += diff_p;
                    }else{
                        // Etape 5
                        inKnapsack[i][j] = residualCapacity/diff_w;
                        inKnapsack[i][j-1] = 1 - residualCapacity/diff_w;
                        totalValue += diff_p*inKnapsack[i][j];
                    }
                }

            }
        }

    }

    return make_pair(residualCapacity,totalValue);
}

struct aideTri
{
    int operator()(const pair<Item*,double> p1,const pair<Item* , double> p2) const
        {
            return p1.second <p2.second;
        }
};
