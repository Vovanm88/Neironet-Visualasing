#pragma once
#include "MyNetwork.h"
#include <utility>
#include <algorithm>
#include <iostream>

#include "Dataset.h"

using std::cout;
class NetworkTeacher {
public:
	void addExample(std::vector<double> Data, std::vector<double> Answer);
	void startLearn(double stop);
	void doLearnCycle(std::vector<double>& in, double& TE, double& E);
	void setLearnSpeed(double);
	double getLearnSpeed();
	void assign(MyNetwork&);
	void setDataset(Dataset);
private:
	Dataset dataset;
	static int getRandomNumber(int min, int max);
	static double oError(double e);
	double learnSpeed;
	void raiseLearnSpeed();
	MyNetwork *net;
	double calcTotalError();
};

