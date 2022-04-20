#include <iostream>
#include "FuzzyVariable.h"

/*****************************************************/
// standaard constructor + destructor
FuzzyVariable::FuzzyVariable() 
{
	//standaard waarden toekennen aan start en end variabelen.
	// dit geeft  y = 0 * x + 0 -> horizontale lijn over y = 0
	startA = 0;
	startB = 0;
	
	// dit geeft  y = 0 * x + 1 -> horizontale lijn over y = 1
	endA = 0;
	endB = 1;
}
FuzzyVariable::~FuzzyVariable()
{

}


FuzzyVariable::FuzzyVariable(char* name, double start_a, double start_b, double end_a, double end_b, FuzzyVariableShape shape)
{
	// waarden toekennen aan name, start, end en shape
	this->name = name;
		
	startA = start_a;
	startB = start_b;
	endA = end_a;
	endB = end_b;

	this->shape = shape;
}


/*****************************************************/
// functies
double FuzzyVariable::Fuzzify(double x)
{
	// maken van fuzzy waarde
	double y = startA * x + startB;
	if (y > 1) {
		y = endA * x + endB;
	}
	if (y <= 0) {
		y = 0;
	}

	return y;
}


vector<double> FuzzyVariable::DeFuzzify(double y)
{
	double x;
	// uitrekenen van de snijpunten
	double x1 = (y - this->startB) / this->startA;
	double x2 = (y - this->endB) / this->endA;

	return vector<double>{ x1, x2 };
}


char* FuzzyVariable::getName()
{
	return this->name;
}

FuzzyVariableShape FuzzyVariable::getShape()
{
	return this->shape;
}

double FuzzyVariable::getCenter(double xMax)
{
	if (this->shape == FuzzyVariableShape::Triangle) {
		return (1 - this->startB) / this->startA;
	}
	else if (this->shape == FuzzyVariableShape::LeftShoulder) {
		return ((1 - this->endB) / this->endA) / 2;
	}
	else if (this->shape == FuzzyVariableShape::RightShoulder) {
		double x = (1 - this->startB) / this->startA;
		return (x + xMax) / 2;
	}

	return -1;
}












