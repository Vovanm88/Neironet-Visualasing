#pragma once

#include "Neiron.h"
class MyNetwork {
private:
	std::vector <std::vector <Neiron> > network;
	size_t layers;
	double learnSpeed;
	std::vector <unsigned int> NeironLayers;
	std::vector <double> output;
public:
	MyNetwork(size_t nlayers, size_t nInLayer, std::string arch, double n);
	MyNetwork(size_t nlayers, std::vector<unsigned int> vnInLayer, std::string arch, double n);
	std::vector<double> Activate(std::vector <double> input);
	void LearnNetwork(std::vector <double> Need);
	void changeLearningSpeed(double ls);
	std::vector <std::vector <Neiron> > getNetworkData();
};
