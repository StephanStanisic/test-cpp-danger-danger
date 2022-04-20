#include <iostream>
#include <vector>
#include "FuzzyRule.h"
#include "FuzzyVariable.h"
#include "FuzzyRuleSet.h"
#include "FuzzySet.h"
#include "../util/VectorMath.hpp"

/*****************************************************/
// standaard constructor + destructor
FuzzySet::FuzzySet(double maxWidth)
{
	//FuzzyVariable = nullptr;
	this->maxWidth = maxWidth;
}

FuzzySet::~FuzzySet()
{

}



/*****************************************************/
// functies
void FuzzySet::Add(FuzzyVariable* a, double value)
{
	std::pair<bool, int> result = findInVector<FuzzyVariable*>(this->Variables, a);

	if (result.first) {
		if (value > this->MaxValues[result.second]) {
			this->MaxValues[result.second] = value;
		}
	}
	else {
		this->Variables.push_back(a);
		this->MaxValues.push_back(value);
	}

}

double FuzzySet::getMaxAV()
{
	// get centers
	double numerator = 0, denumerator = 0;
	for (int i = 0; i < this->Variables.size(); i++) {
		double center = this->Variables[i]->getCenter(this->maxWidth);
		
		numerator += center * this->MaxValues[i];
		denumerator += this->MaxValues[i];
	}
	return numerator / denumerator;
}
