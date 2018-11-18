#include <vector>
#include <algorithm>
#include <iostream>

#include "data.h"

using namespace std;

pair<double,double> MCKP_Greedy_Algorithm(Dataset d, double max_Weight){
    vector<Class> R; // @R va contenir les enveloppes convexes de chaque classes
    //vector<vector<double>> p;
    //vector<vector<double>> w;
    vector<pair<Item*,double>> pairItem;
    vector<vector<double>> inKnapsack(d.getNbClasses(),{1});

    cout << "Etape 0" << endl;

    double z = 0; //servira pour l'étape 4 de l'algorithme

    double residualCapacity = max_Weight ; // représente la capacité résiduelle
    double totalValue = 0; //value of the Knapsack
    for(int i=0; i<d.getNbClasses(); i++){
        // Etape 1 : construction des enveloppes convexes

        R.push_back((d[i])->upperBound());
        // Etape 2 : construction de p et w

        for(unsigned int j=1; j<R[i].getNbItems(); j++){
            R[i].sortItemsWeight();

            double diff_p = R[i][j]->getValue() - R[i][j-1]->getValue();
            double diff_w = R[i][j]->getWeight() - R[i][j-1]->getWeight();
            pairItem.push_back(make_pair(R[i][j],diff_p/diff_w));
            inKnapsack[i].push_back(0);
        }
        residualCapacity -= ((R[i])[0])->getWeight();
        z += (R[i][0])->getValue();

    }

    cout << "Etape 2" << endl;
    // Etape 3
    // sorting items (in each class) according to decreasing p/w
    sort(pairItem.begin(),pairItem.end());

    for (unsigned int l = 0;l<pairItem.size();l++){

        //on cherche la classe de l'item
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
