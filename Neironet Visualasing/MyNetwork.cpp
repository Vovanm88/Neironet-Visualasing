#include "MyNetwork.h"


MyNetwork::MyNetwork(size_t nlayers, size_t nInLayer, std::string arch, double n) {

	std::random_device rd;
	std::mt19937 mrsnRnd(rd());

	learnSpeed = n;
	layers = nlayers;
	network.resize(layers);
	NeironLayers.resize(layers);
	for (unsigned int i = 0; i < layers; i++) {
		NeironLayers[i] = nInLayer;
		std::vector <Neiron> nntmp(nInLayer, Neiron(nInLayer + 1, arch, mrsnRnd));

		//network[i].resize(nInLayer);
		network[i] = nntmp;
		//std::fill(network[i].begin(), network[i].end(),Neiron(nInLayer, arch));
	}
}
MyNetwork::MyNetwork(size_t nlayers, std::vector<unsigned int> vnInLayer, std::string arch, double n) {
	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	learnSpeed = n;
	layers = nlayers;
	network.resize(layers);
	NeironLayers.resize(layers);
	for (size_t i = 0; i < layers; i++) {
		NeironLayers[i] = vnInLayer[i + 1];
		std::vector <Neiron> nntmp(vnInLayer[i + 1], Neiron(vnInLayer[i] + 1, arch, mrsnRnd));
		//network[i].resize(nInLayer);
		network[i] = nntmp;
		//std::fill(network[i].begin(), network[i].end(),Neiron(nInLayer, arch));
	}
}
std::vector<double> MyNetwork::Activate(std::vector <double> input) {
	std::vector<double> lastLayerOutput, thisLayer;
	lastLayerOutput = input;
	for (unsigned int i = 0; i < layers; i++) {
		std::vector<double> thisLayer;
		lastLayerOutput.push_back(0);
		for (unsigned int j = 0; j < NeironLayers[i]; j++) {
			thisLayer.push_back(network[i][j].work(lastLayerOutput));
		}
		lastLayerOutput = thisLayer;
	}
	output = lastLayerOutput;
	return lastLayerOutput;
}
void MyNetwork::LearnNetwork(std::vector <double> Need) {
	std::vector<double> dE_dO;
	for (int i = 0; i < output.size(); i++) {
		dE_dO.push_back(output[i] - Need[i]);
	}
	for (unsigned int i = layers - 1; i > 0; i--) {

		std::vector<double> dHelper(NeironLayers[i - 1], 0);
		for (unsigned int k = 0; k < NeironLayers[i - 1]; k++) {
			for (unsigned int j = 0; j < NeironLayers[i]; j++) {
				dHelper[k] += (network[i][j].d * network[i][j].W[k]);
			}
		}
		for (unsigned int j = 0; j < NeironLayers[i]; j++) {
			network[i][j].learn(dE_dO[j], learnSpeed);
		}
		dE_dO = dHelper;
	}
	for (unsigned int j = 0; j < NeironLayers[0]; j++) {
		network[0][j].learn(dE_dO[j], learnSpeed);
	}
}
void MyNetwork::changeLearningSpeed(double ls) {
	learnSpeed = ls;
}
std::vector <std::vector <Neiron> > MyNetwork::getNetworkData() {
	return network;
}
