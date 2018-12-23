#include "pch.h"
#include "Solver.hpp"



Solver::Solver(string filename,int op){
    inputData(filename);

    //int op = 0;

    //cout<<"please choose solver:"<<endl
    //    <<"0:greedy"<<endl
     //   <<"1:SA"<<endl;

    //cin>>op;

    
    switch (op)
    {
        case 0:
            this->solveWithGreedy();
            break;
    
        default:
            this->solveWithSA();
            break;
    }    

	fstream fs;
	fs.open((filename+"_res_"+to_string(op)+  ".txt").c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);

	if (!fs.is_open()) {
		cout << "WTF!" << endl;
		exit(1);
	}

	fs << getTotalCost(customer) << endl;

	vector<int> tempFixCost = fixCost;
	float cost = 0;
	for (int customerIndex = 0; customerIndex < customer.size(); customerIndex++) {
		tempFixCost[customer[customerIndex]] = 0;
	}

	for (int factoryIndex = 0; factoryIndex < fixCost.size(); factoryIndex++) {
		if (tempFixCost[factoryIndex] == 0)
			fs << 0 << " ";
		else
			fs << 1 << " ";
	}

	fs << endl;

	for (int customerIndex = 0; customerIndex < customer.size(); customerIndex++) {
		fs << customer[customerIndex] << " ";
	}
	fs << endl;
}

void Solver::inputData(string filename){
    ifstream input;
    input.open(filename.c_str());
    if(input.is_open()){
        int customSize=0;
        int factorySize=0;
        input>>factorySize>>customSize;

        // input capcity and fix cost for each factory
        int cap=0,cost = 0;
        for(int c1=0;c1<factorySize;c1++){
            input>>cap>>cost;
            this->capcity.push_back(cap);
            this->fixCost.push_back(cost);
        }

        float dem=0;
        // input demand for each costomer
        for(int c1=0;c1<customSize;c1++){
            input>>dem;
            this->demand.push_back(dem);
        }

        float spCosts=0;
        // input expect cost for each costomer to each factory
        for(int customerIndex=0;customerIndex<customSize;customerIndex++){
            this->customer.push_back(-1);
            vector<float> tcost;
            for(int factoryIndex=0;factoryIndex<factorySize;factoryIndex++){
                input>>spCosts;
                tcost.push_back(spCosts);
            }

            this->eCost.push_back(tcost);
        }
    } else{
        cout<<"ERROR IN READ DATA:"<<filename<<endl;
    }

	input.close();
}

void Solver::solveWithGreedy(){

	vector<vector<float>> eCost_copy = eCost;
    cout<<"in to greedy"<<endl;
    // init 
    vector<int> unselectCustomer=this->customer;

    // if not all get the factory, select
    while(!checkWhetherCustomerSatisfied()){
        
        // select for each
        // if someone have nothing to select, just print error and exit
        if(greedy_select(unselectCustomer)){
            
            // calculate which factory no satisfy the capcity constraits
            vector<int> badFactoryIndex = greedy_CheckFactoryCapcity(unselectCustomer);

            // reset the worst customer for each factory
            //reflesh the factories' capcity, and of couse fressh the good customer
            greedy_freshCustomer(badFactoryIndex,unselectCustomer);
            
        } else {
            cout<<"ERROR TO FIND FACTORY"<<endl;
            exit(1);
        }
    }

	eCost = eCost_copy;

    cout<<"------over------"<<endl
        <<"total cost"<<this->getTotalCost(this->customer)<<endl;
}

// select for each
// if someone have nothing to select, just print error and exit
// unselectCustomer: if it has been selected, it will be -2;
// else it will be -1 meanning unselected
// just choose and change the -1 one 
// return if every one get a factory
bool Solver::greedy_select(vector<int> &unselectCustomer){

    for(int customerIndex=0;customerIndex<unselectCustomer.size();customerIndex++){
        if(unselectCustomer[customerIndex]!=-2){
            auto min = min_element(eCost[customerIndex].begin(), eCost[customerIndex].end());
            //cout << *max << " " << distance(eCost[customerIndex].begin(), max) << endl;
            if(*min!= INT_MAX){
                unselectCustomer[customerIndex] = distance(eCost[customerIndex].begin(), min);
            } else {
                return false;
            }
        }
    }

    return true;
}

// calculate which factory no satisfy the capcity constraits
// input the choices
// return the bad's index
vector<int> Solver::greedy_CheckFactoryCapcity(vector<int> &unselectCustomer){
    vector<int> allCapcity = this->capcity;

    for(int customerIndex=0;customerIndex<unselectCustomer.size();customerIndex++){
        if(unselectCustomer[customerIndex] >=0 ){
            allCapcity[unselectCustomer[customerIndex]] -= demand[customerIndex];
        }
    }

    vector<int> badFactoryIndex;
    for(int factoryIndex=0;factoryIndex<this->capcity.size();factoryIndex++){
        if(allCapcity[factoryIndex] <0){
            badFactoryIndex.push_back(factoryIndex);
        }
    }

    return badFactoryIndex;
}

// input the badFactoryIndex, and the choice
// reflesh the factories' capcity
// reset customer for each factory
void Solver::greedy_freshCustomer(vector<int> badFactoryIndex,vector<int> &unselectCustomer){

    // for each bad factory, remove the worst one untill it satisfied the capcity constaints
    for(auto factoryIndex:badFactoryIndex){
        
        int need=0;
        for(int customerIndex=0;customerIndex<unselectCustomer.size();customerIndex++){
            if(unselectCustomer[customerIndex] == factoryIndex){
                need += demand[customerIndex];
            }
        }

        while(need > capcity[factoryIndex]){
            int maxIndex = -1;
            for(int customerIndex=0;customerIndex<unselectCustomer.size();customerIndex++){
                if(unselectCustomer[customerIndex] == factoryIndex){
                    if(maxIndex == -1 || demand[maxIndex]<demand[customerIndex])
                    {
                        maxIndex = customerIndex;
                    }
                }
            }

            if(maxIndex != -1){
                need -= demand[maxIndex];

				eCost[maxIndex][unselectCustomer[maxIndex]] = INT_MAX;

                unselectCustomer[maxIndex] = -1;
            } else {
                cout<<"ERROR IN REMOVE CUSTOMEER";
                exit(1);
            }
        }

        capcity[factoryIndex] -= need;

        for(int customerIndex=0;customerIndex<unselectCustomer.size();customerIndex++){
            if(unselectCustomer[customerIndex] == factoryIndex){
                unselectCustomer[customerIndex] = -2;
                this->customer[customerIndex] = factoryIndex;
            }
        }
    }

	// for the good
	for (int customerIndex = 0; customerIndex < unselectCustomer.size(); customerIndex++) {
		if (unselectCustomer[customerIndex] >= 0) {
			capcity[unselectCustomer[customerIndex]] -= demand[customerIndex];
			customer[customerIndex] = unselectCustomer[customerIndex];
			unselectCustomer[customerIndex] = -2;
		}
	}
}


// if not all get the factory, return to select
bool Solver::checkWhetherCustomerSatisfied(){
    int sum = 0;
    cout<<"check: ";
    for(int customerIndex=0;customerIndex<this->customer.size();customerIndex++){
        cout<<this->customer[customerIndex]<<" ";
        if(this->customer[customerIndex] >= 0){
            sum++;
        }
    }
    cout<<endl;

    return this->customer.size() == sum;
}

void Solver::solveWithSA(){
    // TODO:: init a solve to  customer
	solveWithGreedy();

    srand(time(0));

    // input arg
    int T=1000;
    int lowThreshold=5;
    
    int convergenceStep = 100000;
    int cost = getTotalCost(customer);

    int miniCost = cost;
    vector<int> miniSolve = customer;
    vector<int> miniCapcity = capcity;

    int overCount = 0;

    while(1){

		for (int repeatInT = 0; repeatInT < 10000; repeatInT++) {

			// find a new solve
			vector<int> tempSolve = customer;
			vector<int> tempCapcity = capcity;
			int op = rand() % 2;

			int currentCustomer = rand() % customer.size();
			int newFactory;
			int anotherCustomer;
			switch (op)
			{
			case 0:
				// move a customer to anthor factory
				newFactory = rand() % fixCost.size();
				if (newFactory == tempSolve[currentCustomer]) {
					newFactory = (newFactory + 1) % fixCost.size();
				}

				// check for capcity
				if (capcity[newFactory] >= demand[currentCustomer]) {
					tempCapcity[newFactory] -= demand[currentCustomer];
					tempCapcity[tempSolve[currentCustomer]] += demand[currentCustomer];
					tempSolve[currentCustomer] = newFactory;
				}

				break;

			default:
				// exchange tow customers' factory
				anotherCustomer = (currentCustomer + 1 + rand() % (customer.size() - 1)) % customer.size();

				if (demand[currentCustomer] > demand[anotherCustomer])
				{
					if (capcity[tempSolve[anotherCustomer]] >= demand[currentCustomer] - demand[anotherCustomer]) {
						tempCapcity[tempSolve[anotherCustomer]] += demand[currentCustomer] - demand[anotherCustomer];
						tempCapcity[tempSolve[currentCustomer]] -= demand[currentCustomer] - demand[anotherCustomer];

						swap(tempSolve[anotherCustomer], tempSolve[currentCustomer]);
					}
				}
				else {
					if (capcity[tempSolve[currentCustomer]] >= demand[anotherCustomer] - demand[currentCustomer]) {
						tempCapcity[tempSolve[anotherCustomer]] += demand[currentCustomer] - demand[anotherCustomer];
						tempCapcity[tempSolve[currentCustomer]] -= demand[currentCustomer] - demand[anotherCustomer];

						swap(tempSolve[currentCustomer], tempSolve[anotherCustomer]);
					}
				}

				break;
			}


			// check it for whether recive it
			int tempCost = getTotalCost(tempSolve);

			if (tempCost < cost || (T != 0 && (((double)rand()) / RAND_MAX < 1.0 / (1 + exp((tempCost - cost) / T))))) {
				// 更新容量
				capcity = tempCapcity;
				// 更新解
				customer = tempSolve;
				// 更新当前开销
				cost = tempCost;
			}

			// check for over
			if (cost < miniCost) {
				miniCost = cost;
				miniCapcity = capcity;
				miniSolve = customer;
				overCount = 0;
			}
			else {
				if (overCount++ >= convergenceStep) {
					customer = miniSolve;
					capcity = miniCapcity;
					cout << "FINISH SA" << endl << "___COST:" << miniCost << endl;
					return;
				}
			}


			if (overCount % 1000 == 0)
				cout << "T:" << T << "  cost" << cost << endl;
		}

		if(T> lowThreshold)
			T = 0.99*T;

    }
}


float Solver::getTotalCost(vector<int> tempSolve){
	vector<int> tempFixCost = fixCost;
    float cost = 0;
    for(int customerIndex = 0;customerIndex<tempSolve.size();customerIndex++){
        cost+= eCost[customerIndex][tempSolve[customerIndex]];
		cost += tempFixCost[tempSolve[customerIndex]];
		tempFixCost[tempSolve[customerIndex]] = 0;
    }

	
	//for (int factoryIndex = 0; factoryIndex < fixCost.size(); factoryIndex++) {
	//	if (tempFixCost[factoryIndex] == 0)
	//		cout << 0 << " ";
	//	else
	//		cout << 1 << " ";
	//}

	//cout << endl;

    return cost;
}