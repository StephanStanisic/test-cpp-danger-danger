#include <iostream>
#include <algorithm>
#include "FuzzyRule.h"

using std::min, std::max;

/*****************************************************/
// standaard constructor + destructor
FuzzyRule::FuzzyRule(FuzzyVariable* Antecedent1, FuzzyVariable* Antecedent2, FuzzyVariable* Concequent, FuzzyOperator o) {
	// toekennen van de variabelen
	this->Antecedent1 = Antecedent1;
	this->Antecedent2 = Antecedent2;

	this->Concequent = Concequent;

	this->Operator = o;
}

FuzzyRule::~FuzzyRule()
{

}



/*****************************************************/
// functies
double FuzzyRule::Calculate(double a, double b)
{
	auto resultingAntecedent1 = this->Antecedent1->Fuzzify(a);
	auto resultingAntecedent2 = this->Antecedent2->Fuzzify(b);
	double resultingConcequent = -1;


	if (this->Operator == FuzzyOperator::AND) {
		resultingConcequent = min(resultingAntecedent1, resultingAntecedent2);
	}
	else if (this->Operator == FuzzyOperator::OR) {
		resultingConcequent = max(resultingAntecedent1, resultingAntecedent2);
	}
	
	return resultingConcequent;
}

FuzzyVariable* FuzzyRule::getConcequent()
{
	return this->Concequent;
}

FuzzyVariable* FuzzyRule::getAntecedent1()
{
	return this->Antecedent1;
}
FuzzyVariable* FuzzyRule::getAntecedent2()
{
	return this->Antecedent2;
}

