#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>

#include "data.h"

using namespace std;

void exhaustiveSearch(Dataset* data, double capacity) {
	vector<int> nbItemsByClass = data->getNbItemsByClass();
	vector<int> allocation (data->getNbClasses(), 0);

	double bestValue = DBL_MIN;
	double bestWeight = 0;
	vector<int> bestAllocation = allocation;

	double w = allocation.getWeight();
	if ( w <= capacity ) {
		// First allocation is a valid allocation.
		bestValue = allocation.getValue();
		bestWeight = w;
	}

	while ( j < data.getNbClasses() ) {
		j = 0;
		// Find a new allocation.
		while ( true ) {
			allocation[j] += 1;
			if ( allocation[j] = nbItemsByClass[j] ) {
				// Reached last item in class j.
				allocation[j] = 0;
				j++;
			} else {
				break
			}
		}

		// Check if allocation is the best allocation.
		double w = allocation.getWeight();
		if ( w <= capacity ) {
			double v = allocation.getValue();
			if ( v > bestValue ) {
				// New best allocation.
				bestValue = v;
				bestWeight = w;
				bestAllocation = allocation;
			}
		}

	}
}
