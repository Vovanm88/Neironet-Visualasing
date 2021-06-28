#pragma once

#include <fstream>
#include <sstream>

#include "Neiron.h"
class MyNetwork {
private:
	std::vector <std::vector <Neiron> > network;
	size_t layers;
	double learnSpeed;
	std::vector <unsigned int> NeironLayers;
	std::vector <double> output;
	std::string activationFunction;
public:
	MyNetwork(size_t nlayers, size_t nInLayer, std::string arch, double n);
	MyNetwork(size_t nlayers, std::vector<unsigned int> vnInLayer, std::string arch, double n);
	std::vector<double> run(std::vector <double> input);
	void correctLastRun(std::vector <double> Need);
	void setLearningSpeed(double ls);
	std::vector <std::vector <Neiron> > getNetworkData();
	int saveData(std::string dataPath);
	int loadData(std::string dataPath);
};
