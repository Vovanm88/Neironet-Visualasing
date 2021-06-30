#include "MyNetwork.h"

MyNetwork::MyNetwork(size_t nlayers, size_t nInLayer, NeironArch arch, double n)
{

	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	activationFunction = arch;
	learnSpeed = n;
	layers = nlayers;
	network.resize(layers);
	NeironLayers.resize(layers);
	for (unsigned int layerIndx = 0; layerIndx < layers; layerIndx++)
	{
		NeironLayers[layerIndx] = nInLayer;
		std::vector<Neiron *> nntmp(nInLayer, Neiron::factory(nInLayer + 1, arch, mrsnRnd));

		//network[layerIndx].resize(nInLayer);
		network[layerIndx] = nntmp;
		//std::fill(network[layerIndx].begin(), network[layerIndx].end(),Neiron(nInLayer, arch));
	}
	NeironLayers.push_back(nInLayer);
}
MyNetwork::MyNetwork(size_t nlayers, std::vector<unsigned int> vnInLayer, NeironArch arch, double n)
{
	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	learnSpeed = n;
	layers = nlayers;
	activationFunction = arch;
	network.resize(layers);
	NeironLayers.resize(layers);
	NeironLayers = vnInLayer;
	for (size_t layerIndx = 0; layerIndx < layers; layerIndx++)
	{
		//NeironLayers[layerIndx] = vnInLayer[layerIndx];
		std::vector<Neiron *> nntmp(vnInLayer[layerIndx + 1]);
		for (size_t j = 0; j < nntmp.size(); j++)
		{
			nntmp[j] = Neiron::factory(vnInLayer[layerIndx] + 1, arch, mrsnRnd);
		}
		//network[layerIndx].resize(nInLayer);
		network[layerIndx] = nntmp;
		//std::fill(network[layerIndx].begin(), network[layerIndx].end(),Neiron(nInLayer, arch));
	}
}
std::vector<double> MyNetwork::run(std::vector<double> input)
{
	std::vector<double> lastLayerOutput;
	lastLayerOutput = input;
	for (unsigned int layerIndx = 0; layerIndx < layers; layerIndx++)
	{
		std::vector<double> thisLayer;
		lastLayerOutput.push_back(1);
		for (unsigned int j = 0; j < NeironLayers[layerIndx + 1]; j++)
		{
			thisLayer.push_back(network[layerIndx][j]->work(lastLayerOutput));
		}
		lastLayerOutput = thisLayer;
	}
	output = lastLayerOutput;
	return lastLayerOutput;
}
void MyNetwork::correctLastRun(std::vector<double> target)
{
	std::vector<double> errorValues;
	for (int i = 0; i < output.size(); i++)
	{
		errorValues.push_back(output[i] - target[i]);
	}
	for (unsigned int layerIndx = layers - 1; layerIndx > 0; layerIndx--)
	{
		for (unsigned int j = 0; j < NeironLayers[layerIndx + 1]; j++)
		{
			network[layerIndx][j]->learn(errorValues[j], learnSpeed);
		}

		std::vector<double> newErrorValues(NeironLayers[layerIndx], 0);
		for (unsigned int k = 0; k < NeironLayers[layerIndx]; k++)
		{
			for (unsigned int j = 0; j < NeironLayers[layerIndx + 1]; j++)
			{
				newErrorValues[k] += (network[layerIndx][j]->d * network[layerIndx][j]->weights[k]);
			}
		}
		errorValues = newErrorValues;
	}
	for (unsigned int j = 0; j < NeironLayers[1]; j++)
	{
		network[0][j]->learn(errorValues[j], learnSpeed);
	}
}
void MyNetwork::setLearningSpeed(double ls)
{
	learnSpeed = ls;
}
std::vector<std::vector<Neiron *>> MyNetwork::getNetworkData()
{
	return network;
}
int MyNetwork::saveData(std::string dataPath)
{

	// The save file had this content:
	// first line contain number of layers - nL and type of activation function
	// second line coitain (nL+1) numbers that means number of neirons of each layers
	// next lines contains weights of each neiron, one line per neiron

	std::ofstream fout(dataPath, std::ios_base::trunc);
	if (!fout.is_open())
	{
		return 1;
	}
	fout << layers << ' ' << activationFunction << '\n';
	for (const auto &onelayer : NeironLayers)
	{
		fout << onelayer << ' ';
	}
	fout << '\n';
	for (const auto &it_layer : network)
	{
		for (const auto &it_neiron : it_layer)
		{
			//fout << it_neiron.weights.size()<<" ";
			for (const double &it_weight : it_neiron->weights)
			{
				fout << it_weight << ' ';
			}
			fout << '\n';
		}
	}
	fout.close();
	return 0;
}
int MyNetwork::loadData(std::string dataPath)
{
	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	/*
	activationFunction = arch;
	network.resize(layers);
	NeironLayers.resize(layers);
	NeironLayers = vnInLayer;
	for (size_t layerIndx = 0; layerIndx < layers; layerIndx++) {
		//NeironLayers[layerIndx] = vnInLayer[layerIndx];
		std::vector <Neiron> nntmp(vnInLayer[layerIndx + 1], Neiron(vnInLayer[layerIndx] + 1, arch, mrsnRnd));
		//network[layerIndx].resize(nInLayer);
		network[layerIndx] = nntmp;
		//std::fill(network[layerIndx].begin(), network[layerIndx].end(),Neiron(nInLayer, arch));
	}
	*/
	std::ifstream finput(dataPath);
	if (!finput.is_open())
	{
		return 1;
	}

	int af;
	finput >> layers >> af;
	activationFunction = (NeironArch)af;

	NeironLayers.clear();
	NeironLayers.resize(layers + 1);
	for (auto &nInLayer : NeironLayers)
	{
		finput >> nInLayer;
	}
	network.resize(layers);
	for (size_t layerIndx = 0; layerIndx < layers; layerIndx++)
	{
		std::vector<Neiron *> nntmp(NeironLayers[layerIndx + 1], Neiron::factory(NeironLayers[layerIndx] + 1, activationFunction, mrsnRnd));
		for (auto &it_neiron : nntmp)
		{
			for (auto &it_weight : it_neiron->weights)
			{
				finput >> it_weight;
			}
		}
		network[layerIndx] = nntmp;
	}
	return 0;
}

MyNetwork::~MyNetwork()
{
	for (auto layer : network)
	{
		for (auto neiron : layer)
		{
			delete neiron;
		}
	}
}

MyNetwork MyNetwork::clone()
{
	MyNetwork net_copy = *this;
	for (size_t layerIndx = 0; layerIndx < this->network.size(); layerIndx++)
	{
		for (size_t j = 0; j < this->network[layerIndx].size(); j++)
		{
			net_copy.network[layerIndx][j] = this->network[layerIndx][j]->clone();
		}
	}
	return net_copy;
}
