#include <vector>
#include <algorithm>
#include <iostream>

#include "data.h"

// #define VERBOSE
// #define ULTRA_VERBOSE

using namespace std;

struct aideTri
{
    int operator()(const pair<Item*,double> p1,const pair<Item* , double> p2) const
        {
            return p1.second > p2.second;
        }
};

WeightedAllocation MCKP_Greedy_Algorithm(Dataset* d, double max_Weight){
    vector<vector<Item*>> R; // @R va contenir les enveloppes convexes de chaque classes
    vector<pair<Item*,double>> pairItem;

    // j-th class is in the knapsack, 0 if not. (we can take a fraction of the latest item put in the knapsack when
    // we exceed the capacity of the knapsack
    vector<Item*> choosenItems;// i-th element is the item from the i-th class that we put in knapsack
    vector<double> proportion; // j-th element is the proportion we took from each items, we just take two items not entirely


    // OUTPUTS
    double residualCapacity = max_Weight ; // capacity still free with the items in the knapsack
    // (max_Weight - current weight of the knapsack)
    double totalValue = 0; //sum of values of the items which are in the knapsack

    // Beginning of the algorithm
#ifdef VERBOSE
        cout << "STEP 1 AND 2..." << endl;
#endif
    for(int i=0; i<d->getNbClasses(); i++){

        // Etape 1 : construction of upper bound in each class
#ifdef ULTRA_VERBOSE
        cout << "Building upper bound " << to_string(i) << "..."<< endl;
#endif

        R.push_back( ((*d)[i])->upperBound() );
        proportion.push_back(1);
        choosenItems.push_back(R[i][0]);
        // Etape 2 : construction of efficacity vector
#ifdef ULTRA_VERBOSE
        cout << "Building of efficacity vector : PART"<< to_string(i) << "..." << endl;
#endif

        for(unsigned int j=1; j<R[i].size(); j++){
            // R[i] is already sorted by increasing weights because of we sorted classes[i] before deriving the upper
            // bound of this class
            double diff_p = R[i][j]->getValue() - R[i][j-1]->getValue(); // value[i][j] - value[i][j-1]
            double diff_w = R[i][j]->getWeight() - R[i][j-1]->getWeight(); // weight[i][j] - weight[i][j-1]
            // We can't stock thoses numbers for each i and j (as describe in the algorithm) because of a lack of memory

            pairItem.push_back(make_pair(R[i][j],diff_p/diff_w)); // we stock each Item with its efficacity (we don't
            // care about their Class, we'll sort them according to decreasing efficacity thanks to @AideTri)
        }

        // At the beginning, we put the lightest items of each upper bound in the knapsack
        residualCapacity -= ((R[i])[0])->getWeight();
        totalValue += (R[i][0])->getValue();
    }
#ifdef VERBOSE
    cout << "bien passé 1" << endl;
        cout << "STEP 3... " << endl;
#endif
    sort(pairItem.begin(),pairItem.end(),aideTri()); // sorting items (in each class) according to decreasing p/w (efficacity)
    unsigned int l=0;
    while((l<pairItem.size())&(residualCapacity>0)){
        // we're looking for the Class of the l-th item
#ifdef VERBOSE
        cout << "STEP 4..."<< endl;
#endif
        for(unsigned int i=0; i<R.size(); i++){

            for(unsigned int j=0; j<(R[i].size()); j++){

                if(pairItem[l].first->getIndex()==R[i][j]->getIndex()){
                    double diff_w = R[i][j]->getWeight() - R[i][j-1]->getWeight();
                    double diff_p = R[i][j]->getValue() - R[i][j-1]->getValue();
                    if(diff_w<=residualCapacity){
                        //Actualisation
#ifdef ULTRA_VERBOSE
                        cout << "Item " << j+1 << " of class " << i+1 << " is replaced" << endl;
#endif
                        residualCapacity -= diff_w;
                        totalValue += diff_p;
                        choosenItems[i] = R[i][j]; // we take it entirely
                    }else{
#ifdef VERBOSE
                        cout << "STEP 5..." << endl;
#endif
                        cout << "Knapsack too heavy : last item to be replaced (proportionally) is " << j+1 << " of class " << i+1 << endl;
                        proportion[i]= 1- residualCapacity/diff_w ;
                        choosenItems.push_back(R[i][j]);
                        proportion.push_back(residualCapacity/diff_w);
                        residualCapacity = 0;
                        totalValue += diff_p*residualCapacity/diff_w;
                    }
                }

            }
        }
        l++;
    }

    WeightedAllocation res = WeightedAllocation(choosenItems,proportion);
    return res ;
}
