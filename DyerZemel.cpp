#include <vector>
#include <algorithm>
#include <iostream>

#include "data.h"

using namespace std;

double DyerZemelAlgorithm(Dataset data, double capacity) {
	// === Initialization ===
	
	// Create a vector of vectors to store the indices of the remaining items.
	vector<vector<int>> remainingItems;
	// Store the indices of all items in the dataset.
	for (int i = 0; i<data.getNbClasses(); i++) {
		Class* currentClass = data[i];
		vector<int> indices;
		for (int j = 0; j<currentClass.getNbItems(); j++) {
			indices.push_back(j);
		}
		remainingItems.push_back(indices);
	}

	// Create a vector to store the fathom classes.
	// Value is true if the class has been fathom (all values are false by default).
	vector<bool> fathomClass(data.getNbClasses(), false);

	while true {

		// === Step 1 ===
		// Pair the items in each class two by two.
		vector<Pair*> pairs;
		for (int i = 0; i<data.getNbClasses(); i++) {
			if (not fanthomClass[i]) {
				Class* currentClass = data[i];
				int nbPaired = 0;
				int nbNotPaired = remainingItems[i].size();
				while (nbNotPaired >= 2) {
					// Get the index of the first two items not paired.
					int j = remainingItems[i][nbPaired];
					int k = remainingItems[i][nbPaired+1];
					// Get the fist two items not paired.
					Item* ij = currentClass[j];
					Item* ik = currentClass[k];
					// Build a pair with the two items.
					Pair pair(currentClass, ij, ik);
					if ( pair.hasDominance() ) {
						// One item is dominated by the other.
						if ( pair.hasSwapped() ) {
							// k dominates j.
							remainingItems[i].erase(nbPaired);
						} else {
							// j dominates k.
							remainingItems[i].erase(nbPaired+1);
						}
						nbNotPaired--;
					} else {
						// Pair is correct, store it.
						pairs.push_back(&pair);
						nbPaired += 2;
						nbNotPaired -= 2;
					}
				}
			}
		}

		// === Step 2 ===
		for (int i = 0; i<data.getNbClasses(); i++) {
			if (not fathomClass[i]) {
				nbRemainingItems = remainingItems[i].size();
				if (nbRemainingItems > 1) {
					// There are still multiple relevant items in the class.
				} else if (nbRemainingItems == 1) {
					// There is only one item left, this is the item chosen.
					// Decreases capacity by the weight of the item.
					capacity -= remainingItems[i][0]->getWeight();
					// Fathom the class.
					fathomClass[i] = true;
				} else {
					cerr < "Class " << i << " has " << nbRemainingItems << " left" << endl;
				}
			}
		}

		// === Step 3 ===
		// Compute the slopes for each pair.
		vector<double> slopes;
		for (int p=0; p<pairs.size(); p++) {
			slopes.push_back( pairs[p]->getSlope() );
		}
		double alpha = getMedian(slopes);

		// === Step 4 ===
		// Compute the two sums of equation (11.11).
		double minSum = 0;
		double maxSum = 0;
		for (int i = 0; i<data.getNbClases(); i++) {
			if (not fathomClass[i]) {
				double maxValue = - INT_MAX;
				double maxWeight = - INT_MAX;
				double minWeight = INT_MAX;
				int nbItems = remainingItems[i].size()
				for (int j = 0; j<nbItems; j++) {
					int ij = remainingItems[i][j]
					Item item = data[i][ij];
					v = item.getValue();
					w = item.getWeight();
					// Compute value of equation (11.9) for the current item.
					double value = v - alpha * w;
					if (value > maxValue) {
						maxValue = value;
						maxWeight = minWeight = w;
					} else if (value == maxValue) {
						if (w > maxWeight) {
							maxWeight = w;
						} else if (w < minWeight) {
							minWeight = w;
						}
					}
				}
				minSum += minWeight;
				maxSum += maxWeight;
			}
		}

		// === Step 5 ===
		if (capacity >= minSum && capacity < maxSum) {
			// alpha is the optimal slope.
			return alpha
		} else {
			// Keep track of the number of items erased in each class.
			vector<int> nbErased(data.getNbClasses(), 0);
			// Keep track of the number of pairs in each class.
			vector<int> nbPairs(data.getNbClasses(), 0);
			if (minSum > capacity) {
				for (int p = 0; p<pairs.size(); p++) {
					Pair pair = pairs[p];
					int i = pair.getClassIndex();
					if (pair.getSlope() <= alpha) {
						if (pair.hasSwapped()) {
							remainingItems[i].erase(2*nbPairs[i]-nbErased[i]+1);
						} else {
							remainingItems[i].erase(2*nbPairs[i]-nbErased[i]);
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
							remainingItems[i].erase(2*nbPairs[i]-nbErased[i]);
						} else {
							remainingItems[i].erase(2*nbPairs[i]-nbErased[i]+1);
						}
						nbErased[i]++;
					}
					nbPairs[i]++;
				}
			}
		}
	}
}

double getMedian(vector<double> v) {
	// Find median value from a vector of doubles.
	size_t n = v.size() / 2;
	nth_element(v.begin(), v.begin()+n, v.end());
	return v[n];
}
