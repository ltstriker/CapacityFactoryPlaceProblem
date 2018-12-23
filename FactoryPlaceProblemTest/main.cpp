#include "pch.h"
#include "Solver.hpp"
#include <iostream>
#include <cstring>
#include <ctime>
#include <fstream>

using namespace std;



int main(){

    //while(1){
    //    cout<<"please input data file path:"<<endl;
    //    string path;
    //    cin>>path;
    //    Solver solver(path);
    //}
	fstream fs;
	fs.open("total_res.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
	fs<<"|  |result|time(s)|\n|---|---|---|\n";
	for (int dataIndex = 1; dataIndex < 72; dataIndex++) {
		string path = "data/p" + to_string(dataIndex);
		time_t st = time(0);
		Solver solver(path,0);
		time_t et = time(0);
		
		fs << "|" << dataIndex << "|" << solver.getTotalCost(solver.customer) << "|" << et - st << endl;
	}
	fs.close();

	fs.open("total_res_sa.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
	fs << "|  |result|time(s)|\n|---|---|---|\n";
	for (int dataIndex = 1; dataIndex < 72; dataIndex++) {
		string path = "data/p" + to_string(dataIndex);
		time_t st = time(0);
		Solver solver(path, 1);
		time_t et = time(0);

		fs << "|" << dataIndex << "|" << solver.getTotalCost(solver.customer) << "|" << et - st << endl;
	}
	fs.close();
    return 0;
}