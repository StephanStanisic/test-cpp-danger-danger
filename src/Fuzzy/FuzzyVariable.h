#pragma once
#include <vector>

using std::vector;

enum class FuzzyVariableShape { LeftShoulder, RightShoulder, Triangle };

class FuzzyVariable 
{
private:
	char* name;
	double startA, startB;
	double endA, endB;
	FuzzyVariableShape shape;

public:
	
		//standaard constructor + destructor
		FuzzyVariable();
		~FuzzyVariable();

		FuzzyVariable(char* name, double start_a, double start_b, double end_a, double endB, FuzzyVariableShape shape);

		// functies
		double Fuzzify(double x);
		vector<double> DeFuzzify(double y);
		char* getName();
		FuzzyVariableShape getShape();
		double getCenter(double xMax);
};
