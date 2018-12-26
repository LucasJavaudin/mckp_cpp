#include <vector>
#include <iostream>

#include "data.h"

// #define VERBOSE
// #define ULTRA_VERBOSE

using namespace std;

Allocation MCKP_Discrete_Algorithm(Dataset* d, double max_Weight){
    vector<Item*> inKnapSack; // one item by class, returns a vector because we can't return an array
    pair<Item*,double> bestChangerByClass[d->getNbClasses()]; // contains the most efficient replacement in each class for items in the knapsack
    double residualCapacity=max_Weight;
    double totalValue = 0;

#ifdef VERBOSE
    cout << "Initialization..." << endl;
#endif

    vector<Class*> withoutDominatedItems;

    for(int i=0;i<d->getNbClasses();i++){
        //we're going to eliminate all dominated items
        Class *c = new Class( (*d)[i]->eliminateDominatedItems() );
        withoutDominatedItems.push_back(c) ;
	// we initialize with the lightest item for each class (they are sorted by increasing weight)
	Item* defaultItem = (*c)[0];
        inKnapSack.push_back(defaultItem); 
        residualCapacity -= defaultItem->getWeight();
        totalValue += defaultItem->getValue();
        bestChangerByClass[i] = c->mostEfficientReplacer(defaultItem); // computing best replacer of the current item in the knapsack
    }

#ifdef VERBOSE
    cout << "ITERATION..." << endl;
#endif
    bool finished = false; // tells if the weight of the knapsack is still lower than max_Weight

    while(!finished){
#ifdef ULTRA_VERBOSE
        cout << "NEW ITERATION..." << endl;
#endif
        pair<Item*,double> replacer (nullptr, 0);
        int classReplaced = -1;

        // We're looking for the best changer in the whole dataset
        for(unsigned int i=0;i<d->getNbClasses();i++){
            if(replacer.second<bestChangerByClass[i].second & bestChangerByClass[i].first!=nullptr){
		// Class i has best changer if efficiency (second) is greater and if item pointer (first) is not null.
                replacer = bestChangerByClass[i];
                classReplaced = i;
            }
        }

	double diff_weight;
	double diff_value;
	bool validChange = false;
	if (classReplaced != -1) {
	    diff_weight = replacer.first->getWeight() - inKnapSack[classReplaced]->getWeight();
	    diff_value = replacer.first->getValue() - inKnapSack[classReplaced]->getValue();
	    // Only change the item if weight of the change is not greater than residual capacity.
	    validChange = diff_weight <= residualCapacity;
	}

        if(validChange){

	    // Delete all items in classReplaced for which weight is lower to weight of new item chosen.
	    double new_weight = replacer.first->getWeight();
	    // Pointer to class replaced.
	    Class* ptrClassReplaced = withoutDominatedItems[classReplaced];
            for(unsigned int ind=0; ind<ptrClassReplaced->getNbItems(); ind++){
                if((*ptrClassReplaced)[ind]->getWeight() < new_weight){
                    ptrClassReplaced->deleteItem(ind);
                }
            }

            inKnapSack[classReplaced] = replacer.first;
            totalValue = totalValue + diff_value;
            residualCapacity = residualCapacity - diff_weight; // we update the weight still free
            bestChangerByClass[classReplaced] = ptrClassReplaced->mostEfficientReplacer(replacer.first); //we update the best replacer in the class in which we removed the item
#ifdef ULTRA_VERBOSE
            cout << "Item " << replacer.first->getIndex() << " in class " << classReplaced+1 << " is chosen"<< endl;
#endif
        } else {
	    finished = true;
	}
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
