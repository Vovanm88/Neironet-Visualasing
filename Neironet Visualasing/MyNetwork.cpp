#include "MyNetwork.h"


MyNetwork::MyNetwork(size_t nlayers, size_t nInLayer, std::string arch, double n) {

	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	activationFunction = arch;
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
	NeironLayers.push_back(nInLayer);
}
MyNetwork::MyNetwork(size_t nlayers, std::vector<unsigned int> vnInLayer, std::string arch, double n) {
	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	learnSpeed = n;
	layers = nlayers;
	activationFunction = arch;
	network.resize(layers);
	NeironLayers.resize(layers);
	NeironLayers = vnInLayer;
	for (size_t i = 0; i < layers; i++) {
		//NeironLayers[i] = vnInLayer[i];
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
		for (unsigned int j = 0; j < NeironLayers[i+1]; j++) {
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

		std::vector<double> dHelper(NeironLayers[i], 0);
		for (unsigned int k = 0; k < NeironLayers[i]; k++) {
			for (unsigned int j = 0; j < NeironLayers[i + 1]; j++) {
				dHelper[k] += (network[i][j].d * network[i][j].W[k]);
			}
		}
		for (unsigned int j = 0; j < NeironLayers[i+1]; j++) {
			network[i][j].learn(dE_dO[j], learnSpeed);
		}
		dE_dO = dHelper;
	}
	for (unsigned int j = 0; j < NeironLayers[1]; j++) {
		network[0][j].learn(dE_dO[j], learnSpeed);
	}
}
void MyNetwork::changeLearningSpeed(double ls) {
	learnSpeed = ls;
}
std::vector <std::vector <Neiron> > MyNetwork::getNetworkData() {
	return network;
}
int MyNetwork::saveData(std::string dataPath) {
	
	// The save file had this content:
	// first line contain number of layers - nL and type of activation function
	// second line coitain (nL+1) numbers that means number of neirons of each layers
	// next lines contains weights of each neiron, one line per neiron
	
	std::ofstream fout(dataPath, std::ios_base::trunc);
	if (!fout.is_open()) {
		return 1;
	}
	fout << layers << ' ' << activationFunction << '\n';
	for (const auto &onelayer: NeironLayers) {
		fout << onelayer <<' ';
	}
	fout << '\n';
	for (const auto &it_layer: network) {
		for (const auto &it_neiron : it_layer) {
			//fout << it_neiron.W.size()<<" ";
			for (const double &it_weight : it_neiron.W) {
				fout << it_weight << ' ';
			}
			fout <<'\n';
		}
	}
	fout.close();
	return 0;
}
int MyNetwork::loadData(std::string dataPath) {
	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	/*
	activationFunction = arch;
	network.resize(layers);
	NeironLayers.resize(layers);
	NeironLayers = vnInLayer;
	for (size_t i = 0; i < layers; i++) {
		//NeironLayers[i] = vnInLayer[i];
		std::vector <Neiron> nntmp(vnInLayer[i + 1], Neiron(vnInLayer[i] + 1, arch, mrsnRnd));
		//network[i].resize(nInLayer);
		network[i] = nntmp;
		//std::fill(network[i].begin(), network[i].end(),Neiron(nInLayer, arch));
	}
	*/
	std::ifstream finput(dataPath);
	if (!finput.is_open()) {
		return 1;
	}
	finput >> layers >> activationFunction;
	NeironLayers.clear();
	NeironLayers.resize(layers + 1);
	for (auto& nInLayer : NeironLayers) {
		finput >> nInLayer;
	}
	network.resize(layers);
	for (size_t i = 0; i < layers; i++) {
		std::vector <Neiron> nntmp(NeironLayers[i + 1], Neiron(NeironLayers[i] + 1, activationFunction, mrsnRnd));
		for (auto &it_neiron : nntmp) {
			for (auto &it_weight : it_neiron.W) {
				finput >> it_weight;
			}
		}
		network[i] = nntmp;
	}
	return 0;
}