#include <vector>
#include <iostream>

#include "data.h"

using namespace std;

Allocation MCKP_Discrete_Algorithm(Dataset d, double max_Weight){
    vector<Item*> inKnapSack; // one item by class, returns a vector because we can't return an array
    pair<Item*,double> bestChangerByClass[d.getNbClasses()]; // contains the most efficient replacement in each class for items in the knapsack
    double residualCapacity=max_Weight;
    double totalValue = 0;

#ifdef VERBOSE
    cout << "Initialization..." << endl;
#endif

    vector<Class> withoutDominatedItems;

    for(int i=0;i<d.getNbClasses();i++){
        //we're going to eliminate all dominated items
        Class *c = new Class(d[i]->eliminateDominatedItems());
        withoutDominatedItems.push_back(*c) ;
        inKnapSack.push_back(withoutDominatedItems[i][0]); // we initialize with the lightest item for each class (they are sorting in
        //@eliminateDominatedItems
        residualCapacity -= withoutDominatedItems[i][0]->getWeight();
        totalValue += withoutDominatedItems[i][0]->getValue();
        bestChangerByClass[i] = withoutDominatedItems[i].mostEfficientReplacer(withoutDominatedItems[i][0]); // computing best replacer of the current item in the knapsack
    }

#ifdef VERBOSE
    cout << "ITERATION..." << endl;
#endif
    bool finished = false; // tells if the weight of the knapsack is still lower than max_Weight

    while(!finished){
#ifdef ULTRA_VERBOSE
        cout << "NEW ITERATION..." << endl;
#endif
        pair<Item*,double> replacer = bestChangerByClass[0];
        int classReplaced = 0;

        // We're looking for the best changer in the whole dataset
        for(unsigned int i=1;i<d.getNbClasses();i++){
            if(replacer.second<bestChangerByClass[i].second){
                replacer = bestChangerByClass[i];
                classReplaced = i;
            }
        }

        // We change
        double diff_weight = replacer.first->getWeight() - inKnapSack[classReplaced]->getWeight();
        double diff_value = replacer.first->getValue() - inKnapSack[classReplaced]->getValue();

        cout << endl;
        if(diff_weight<=residualCapacity & bestChangerByClass[classReplaced].second>0){

            for(unsigned int ind=0; ind<withoutDominatedItems[classReplaced].getNbItems(); ind++){
                if(*inKnapSack[classReplaced]==(*withoutDominatedItems[classReplaced][ind])){
                    withoutDominatedItems[classReplaced].deleteItem(ind);
                }
            }

            inKnapSack[classReplaced] = replacer.first;
            totalValue = totalValue + diff_value;
            residualCapacity = residualCapacity - diff_weight; // we update the weight still free
            bestChangerByClass[classReplaced] = withoutDominatedItems[classReplaced].mostEfficientReplacer(replacer.first); //we update the best replacer in the class in which we removed the item
            cout << "Class replaced is " << classReplaced+1 << endl;
#ifdef ULTRA_VERBOSE
            cout << "Item " << replacer->getIndex() << " in class " << classReplaced << " is removed"<< endl;
#endif
        }else finished = true;
#ifdef VERBOSE
            cout << "END... " << endl;
#endif
    }

#ifdef VERBOSE
            cout << "TOTAL WEIGHT : " << max_Weight - residualCapacity << " ON " << max_Weight << endl;
            cout << "TOTAL VALUE : " << totalValue << endl;
#endif
    return Allocation(inKnapSack);
}
