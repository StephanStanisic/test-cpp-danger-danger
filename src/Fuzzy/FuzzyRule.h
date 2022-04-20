#pragma once

#include "FuzzyVariable.h"


enum class FuzzyOperator { AND, OR };

class FuzzyRule
{
private:
	FuzzyVariable* Antecedent1;
	FuzzyVariable* Antecedent2;
	FuzzyVariable* Concequent;

	FuzzyOperator Operator;

public:

	// standaard constructor + destructor
	FuzzyRule(FuzzyVariable* Antecedent1, FuzzyVariable* Antecedent2 , FuzzyVariable* Concequent , FuzzyOperator o);
	~FuzzyRule();

	// functies
	double Calculate(double a, double b);
	FuzzyVariable* getConcequent();
	FuzzyVariable* getAntecedent1();
	FuzzyVariable* getAntecedent2();
};

