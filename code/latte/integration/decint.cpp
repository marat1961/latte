#define COEFF_MAX 10000

#include "PolyRep.h"
#include "newIntegration.h"

#include <iostream>
#include <fstream>
#include <NTL/ZZ.h>

using namespace std;

void delSpace(string &line)
{
	for (int i=0;i<line.length();i++)
	{
		while ((i<line.length())&&(line.at(i)==32)) {line.erase(i,1);};
	};
};
int main(int argc, char *argv[])
{
	if (argc < 3) { cout << "Usage: ./integrate fileIn polynomial (or linear)" << endl; return 1; };
	string line,line2,ll;
	monomialSum myPoly;
	linFormSum lForm;
	ifstream myStream (argv[1]);
	if (!myStream.is_open()) { cout << "Error opening file " << argv[1] << ", please make sure it is spelled correctly." << endl; return 1; };
	if (!strcmp("polynomial",argv[2])) 
	{
		getline(myStream,line);
		delSpace(line);
		loadMonomials(myPoly, line);
		lForm.termCount = 0;
		lForm.varCount = myPoly.varCount;		
		cout << "Decomposing";
		for (int i = 0; i < myPoly.termCount; i++)
		{
			cout << ".";
			decompose(myPoly, lForm, i);
		};
		cout << endl;
 		cout << "The polynomial is:" <<line<<endl;
		cout << "Integrating by decomposition" << endl;
		myStream >> line2;	//line2 is the simplex for example, [[0,0],[1,2],[2,3]]
		integrateFlatVector(lForm, line2);
		destroyLinForms(lForm);
		destroyMonomials(myPoly);
	}
	else if (!strcmp("linear",argv[2])) 
	{
		getline(myStream,line);
		delSpace(line);
		getline(myStream,line2);
		delSpace(line2);
		cout << "The linear form is:"<<line<<endl;
		integrateList(line, line2);
	};
	myStream.close();
	return 0; 
}
