#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>

#include "data.h"

// #define VERBOSE
// #define ULTRA_VERBOSE

using namespace std;

double getMedian(vector<double> v) {
	// Find median value from a vector of doubles.
	size_t n = v.size() / 2;
	nth_element(v.begin(), v.begin()+n, v.end());
	return v[n];
}

pair <double, Allocation> DyerZemelAlgorithm(Dataset* data, double capacity) {
	// === Initialization ===
#ifdef VERBOSE
	cout << "Initialization..." << endl;
#endif
	
	// Create a vector of vectors to store the indices of the remaining items.
	vector<vector<int>> remainingItems;
	// Store the indices of all items in the dataset.
	for (unsigned int i = 0; i<data->getNbClasses(); i++) {
		Class* currentClass = (*data)[i];
		vector<int> indices;
		for (int j = 0; j<currentClass->getNbItems(); j++) {
			indices.push_back(j);
		}
		remainingItems.push_back(indices);
	}

	// Create a vector to store the fathom classes.
	// Value is true if the class has been fathom (all values are false by default).
	vector<bool> fathomClass(data->getNbClasses(), false);

	// Create an allocation of items.
	// By default, we choose the first item in each class.
	vector<Item*> items;
	for (unsigned int i = 0; i<data->getNbClasses(); i++) {
		Class* currentClass = (*data)[i];
		Item* item = (*currentClass)[0];
		items.push_back(item);
	}
	Allocation allocation(items);

	while ( true ) {

#ifdef VERBOSE
		cout << "===== NEW ITERATION =====" << endl;
#endif

		// === Step 1 ===
#ifdef VERBOSE
		cout << "STEP 1..." << endl;
#endif
		// Pair the items in each class two by two.
		vector<Pair> pairs;
		for (int i = 0; i<data->getNbClasses(); i++) {
			if (not fathomClass[i]) {
#ifdef ULTRA_VERBOSE
				cout << "Creating pairs for class " + to_string(i) << endl;
#endif
				Class* currentClass = (*data)[i];
				int nbPaired = 0;
				int nbNotPaired = remainingItems[i].size();
				while (nbNotPaired >= 2) {
					// Get the index of the first two items not paired.
					int j = remainingItems[i][nbPaired];
					int k = remainingItems[i][nbPaired+1];
					// Get the fist two items not paired.
					Item* ij = (*currentClass)[j];
					Item* ik = (*currentClass)[k];
					// Build a pair with the two items.
					Pair pair(i, ij, ik);
					if ( pair.hasDominance() ) {
						// One item is dominated by the other.
						if ( pair.hasSwapped() ) {
#ifdef ULTRA_VERBOSE
							cout << "Item " + to_string(j) + " in class " + to_string(i) + " is removed" << endl;
#endif
							// k dominates j.
							remainingItems[i].erase(remainingItems[i].begin()+nbPaired);
						} else {
#ifdef ULTRA_VERBOSE
							cout << "Item " + to_string(k) + " in class " + to_string(i) + " is removed" << endl;
#endif
							// j dominates k.
							remainingItems[i].erase(remainingItems[i].begin()+nbPaired+1);
						}
						nbNotPaired--;
					} else {
#ifdef ULTRA_VERBOSE
							cout << "Paired items " + to_string(j) + " and " + to_string(k) + " in class " + to_string(i) << endl;
#endif
						// Pair is correct, store it.
						pairs.push_back(pair);
						nbPaired += 2;
						nbNotPaired -= 2;
					}
				}
#ifdef ULTRA_VERBOSE
				cout << "Class " + to_string(i) + " has " + to_string(nbPaired/2) + " pairs" << endl;
#endif
			}
		}

		// === Step 2 ===
#ifdef VERBOSE
		cout << "STEP 2..." << endl;
#endif
		for (int i = 0; i<data->getNbClasses(); i++) {
			if (not fathomClass[i]) {
				int nbRemainingItems = remainingItems[i].size();
				if (nbRemainingItems > 1) {
					// There are still multiple relevant items in the class.
				} else if (nbRemainingItems == 1) {
#ifdef VERBOSE
					cout << "Class " + to_string(i) + " is fathomed" << endl;
#endif
					// There is only one item left, this is the item chosen.
					// Decreases capacity by the weight of the item.
					Class* currentClass = (*data)[i];
					Item* lastItem = (*currentClass)[remainingItems[i][0]];
					capacity -= lastItem->getWeight();
					// Change optimal allocation.
					allocation.changeItem(i, lastItem);
					// Fathom the class.
					fathomClass[i] = true;
				} else {
#ifdef ULTRA_VERBOSE
					cerr << "Class " + to_string(i) + " has " + to_string(nbRemainingItems) + " left" << endl;
#endif
				}
			}
		}

		// === Step 3 ===
#ifdef VERBOSE
		cout << "STEP 3..." << endl;
#endif
		// Compute the slopes for each pair.
		vector<double> slopes;
		for (int p=0; p<pairs.size(); p++) {
			Pair pair = pairs[p];
			slopes.push_back( pair.getSlope() );
		}
		if ( slopes.size() == 0 ) {
			// All clases are fathom, return the current allocation.
			return make_pair(-1.0, allocation);
		}
		double alpha = getMedian(slopes);
#ifdef VERBOSE
		cout << "Median slope is " + to_string(alpha) << endl;
#endif

		// === Step 4 ===
#ifdef VERBOSE
		cout << "STEP 4..." << endl;
#endif
		// Compute the two sums of equation (11.11).
		double minSum = 0;
		double maxSum = 0;
		for (int i = 0; i<data->getNbClasses(); i++) {
			if (not fathomClass[i]) {
				double maxValue = - INT_MAX;
				double maxWeight = - INT_MAX;
				double minWeight = INT_MAX;
				int nbItems = remainingItems[i].size();
				for (int j = 0; j<nbItems; j++) {
					int ij = remainingItems[i][j];
					Class* currentClass = (*data)[i];
					Item* item = (*currentClass)[ij];
					double v = item->getValue();
					double w = item->getWeight();
					// Compute value of equation (11.9) for the current item.
					double value = v - alpha * w;
					if (value > maxValue+.000001) {
						maxValue = value;
						maxWeight = minWeight = w;
						allocation.changeItem(i, item);
					} else if (value >= maxValue-.000001) {
						if (w > maxWeight) {
							maxWeight = w;
						} else if (w < minWeight) {
							minWeight = w;
							allocation.changeItem(i, item);
						}
					}
				}
				minSum += minWeight;
				maxSum += maxWeight;
			}
		}

		// === Step 5 ===
#ifdef VERBOSE
		cout << "STEP 5..." << endl;
#endif
#ifdef ULTRA_VERBOSE
		cout << "Lower sum is " + to_string(minSum) << endl;
		cout << "Upper sum is " + to_string(maxSum) << endl;
		cout << "Remaining capacity is " + to_string(capacity) << endl;
#endif
		if (capacity+0.000001 >= minSum && capacity-0.000001 < maxSum) {
			// alpha is the optimal slope.
			return make_pair(alpha, allocation);
		} else {
			// Keep track of the number of items erased in each class.
			vector<int> nbErased(data->getNbClasses(), 0);
			// Keep track of the number of pairs in each class.
			vector<int> nbPairs(data->getNbClasses(), 0);
			if (minSum > capacity) {
				for (int p = 0; p<pairs.size(); p++) {
					Pair pair = pairs[p];
					int i = pair.getClassIndex();
					if (pair.getSlope() <= alpha) {
						if (pair.hasSwapped()) {
#ifdef ULTRA_VERBOSE
							cout << "Deleting item " + to_string(remainingItems[i][2*nbPairs[i]-nbErased[i]]) + " in class " + to_string(i) << endl;
#endif
							remainingItems[i].erase(remainingItems[i].begin()+2*nbPairs[i]-nbErased[i]);
						} else {
#ifdef ULTRA_VERBOSE
							cout << "Deleting item " + to_string(remainingItems[i][2*nbPairs[i]-nbErased[i]+1]) + " in class " + to_string(i) << endl;
#endif
							remainingItems[i].erase(remainingItems[i].begin()+2*nbPairs[i]-nbErased[i]+1);
						}
						nbErased[i]++;
					}
					nbPairs[i]++;
				}
			} else if (maxSum <= capacity) {
				for (int p = 0; p<pairs.size(); p++) {
					Pair pair = pairs[p];
					int i = pair.getClassIndex();
					if (pair.getSlope() >= alpha) {
						if (pair.hasSwapped()) {
#ifdef ULTRA_VERBOSE
							cout << "Deleting item " + to_string(remainingItems[i][2*nbPairs[i]-nbErased[i]+1]) + " in class " + to_string(i) << endl;
#endif
							remainingItems[i].erase(remainingItems[i].begin()+2*nbPairs[i]-nbErased[i]+1);
						} else {
#ifdef ULTRA_VERBOSE
							cout << "Deleting item " + to_string(remainingItems[i][2*nbPairs[i]-nbErased[i]]) + " in class " + to_string(i) << endl;
#endif
							remainingItems[i].erase(remainingItems[i].begin()+2*nbPairs[i]-nbErased[i]);
						}
						nbErased[i]++;
					}
					nbPairs[i]++;
				}
			}
		}
	}
}
