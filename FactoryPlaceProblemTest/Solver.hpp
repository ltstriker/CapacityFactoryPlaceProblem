#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>
using namespace std;



class Solver
{
private:
    vector<int> capcity;

    vector<int> fixCost;

    vector<int> demand;
    
    vector<vector<float>> eCost;

	void inputData(string filename);

	void solveWithGreedy();

	// select for each
	// if someone have nothing to select, just print error and exit
	// unselectCustomer: if it has been selected, it will be -2;
	// else it will be -1 meanning unselected
	// just choose and change the -1 one 
	// return if every one get a factory
	bool greedy_select(vector<int> &unselectCustomer);

	// calculate which factory no satisfy the capcity constraits
	// input the choices
	// return the bad's index
	vector<int> greedy_CheckFactoryCapcity(vector<int> &unselectCustomer);

	// input the badFactoryIndex, and the choice
	// reset customer for each factory
	void greedy_freshCustomer(vector<int> badFactoryIndex, vector<int> &unselectCustomer);

	// if not all get the factory, return to select
	bool checkWhetherCustomerSatisfied();

	void solveWithSA();


public:
    Solver(string filename,int op);

	vector<int> customer;

    float getTotalCost(vector<int> tempSolve);
};

