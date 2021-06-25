#pragma once
#include "MyNetwork.h"
#include <utility>
#include <algorithm>
#include <iostream>

using std::cout;
class NetworkTeacher {
public:
	void addExample(std::vector<double> Data, std::vector<double> Answer);
	void startLearn(MyNetwork& net, double stop);
	void startLearnVis(MyNetwork& net, std::vector<double>& in, double& TE, double& E, double& learnSpeed);
private:
	std::vector <std::pair<std::vector <double>, std::vector <double> > > DATASET;
	int getRandomNumber(int min, int max, std::mt19937& mrsnRnd);
	double oError(double e);
};

