#pragma once
#include "MyNetwork.h"
#include <utility>
#include <algorithm>
#include <iostream>

#include "Dataset.h"

using std::cout;
class NetworkTeacher
{
public:
	void addExample(std::vector<double> Data, std::vector<double> Answer);
	void startLearn(double stop);
	void doLearnCycle();
	void setLearnSpeed(double);
	double getLearnSpeed();
	double getTotalError();
	void assign(MyNetwork &);
	void setDataset(Dataset);
	DataUnit getCurrentDataUnit();

private:
	Dataset dataset;
	static int getRandomNumber(int min, int max);
	void selectRandomDataUnit();
	static double oError(double e);
	double learnSpeed;
	DataUnit *selectedDataUnit;
	void raiseLearnSpeed();
	MyNetwork *net;
	double calcTotalError();
};
