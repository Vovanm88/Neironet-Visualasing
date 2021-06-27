#include <vector>
#include <utility>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <iostream>
using std::cout;

class Neiron {
public:
	Neiron(int size, std::string arc) {
		std::srand(static_cast<unsigned int>(time(0)));
		W.resize(size);
		for (double& weight : W) {
			weight = rnd() * 1.8 - 0.5;
		}
		if (arc == "ReLU") {
			arch = 0;
		}
		if (arc == "ISRU") {
			arch = 1;
		}
		if (arc == "Softsign") {
			arch = 2;
		}
		if (arc == "Logistic") {
			arch = 3;
		}
	}
	double work(std::vector <double>& input) {
		S = input;
		output = activationP(input);
		return output;
	}
	void learn(double dE_do, double lspeed) {
		double do_dnet = 0;
		switch (arch) {
		case 0:
			do_dnet = ReLU(output, true);
			break;
		case 1:
			do_dnet = ISRU(output, true);
			break;
		case 2:
			do_dnet = Softsign(output, true);
			break;
		case 3:
			do_dnet = Logistic(output, true);
			break;
		}
		d = dE_do * do_dnet;
		for (unsigned int i = 0; i < W.size(); i++) {
			W[i] += -lspeed * d * S[i];
		}
	}
	Neiron& operator= (const Neiron& neiron) {
		
		if (this == &neiron)
			return *this;
		
		output = neiron.output;
		W = neiron.W;
		d = neiron.d;
		S = neiron.S;
		Sum = neiron.Sum;
		size = neiron.size;
		arch = neiron.arch;
		return *this;
	}
	double output; //o(j)
	std::vector <double> W; // w(j)[i]
	double d;
private:
	std::vector <double> S;
	double Sum; //net(j)
	unsigned int size = 0;
	int arch = 0;
	char NEXTNEIRON________________NEXT;
	double findSum(std::vector <double> ss) {
		std::sort(ss.begin(), ss.end());
		double sum = 0;
		size_t first, last;
		first = 0;
		last = ss.size() - 1;
		while (first < last) {
			sum += ss[first];
			sum += ss[last];
			first++;
			last--;
			if (first == last) {
				sum += ss[last];
			}
		}
		return sum;
	}
	double ReLU(double x, bool d = false) {
		if (!d) {
			if (x >= 0) {
				return x;
			}
			else {
				return 0;
			}
		}
		else {
			if (x >= 0) {
				return 1;
			}
			else {
				return 0;
			}
		}
	}
	double ISRU(double x, bool d = false) {
		if (!d) {
			return x / std::sqrt(1 + x * x);
		}
		else {
			double tmp = x / std::sqrt(1 + x * x);
			return tmp * tmp * tmp;
		}
	}
	double Softsign(double x, bool d = false) {
		if (!d) {
			return x / (1 + std::fabs(d));
		}
		else {
			double tmp = (1 + std::fabs(d));
			return x / (tmp * tmp);
		}
	}
	double Logistic(double x, bool d = false) {
		if (!d) {
			return 1.0 / (1 + std::exp(-x));
		}
		else {
			return Logistic(x) * (1 - Logistic(x));
		}
	}
	double activationP(std::vector <double>& input) {
		if (input.size() <= W.size()) {
			std::vector <double> tmp;
			for (unsigned int i = 0; i < input.size(); i++) {
				tmp.push_back(input[i] * W[i]);
			}
			Sum = findSum(tmp);
			switch (arch) {
			case 0:
				return ReLU(Sum);
			case 1:
				return ISRU(Sum);
			case 2:
				return Softsign(Sum);
			case 3:
				return Logistic(Sum);
			}
			return 0;
		}
		else {
			return 0;
		}
	}

	/*double rnd(){
		int r = std::rand();
		double rd = ((double)RAND_MAX/2) /((double)r+0.01);
		return std::max(std::min(491.4679873*(1-rd), 1.0), -1.0);
	}*/
	double rnd() {
		unsigned int r = std::rand();;
		long long r2 = r % 10000;
		return (double)r2 / 10000.0;
	}

};


class MyNetwork {
private:
	std::vector <std::vector <Neiron> > network;
	size_t layers;
	double learnSpeed;
	std::vector <unsigned int> NeironLayers;
	std::vector <double> output;
public:
	MyNetwork(size_t nlayers, size_t nInLayer, std::string arch, double n) {
		std::srand(static_cast<unsigned int>(time(0)));
		learnSpeed = n;
		layers = nlayers;
		network.resize(layers);
		NeironLayers.resize(layers);
		for (unsigned int i = 0; i < layers; i++) {
			NeironLayers[i] = nInLayer;
			std::vector <Neiron> nntmp(nInLayer, Neiron(nInLayer + 1, arch));

			//network[i].resize(nInLayer);
			network[i] = nntmp;
			//std::fill(network[i].begin(), network[i].end(),Neiron(nInLayer, arch));
		}
	}

	MyNetwork(size_t nlayers, std::vector<unsigned int> vnInLayer, std::string arch, double n) {
		srand(static_cast<unsigned int>(time(0)));
		learnSpeed = n;
		layers = nlayers;
		network.resize(layers);
		NeironLayers.resize(layers);
		for (size_t i = 0; i < layers; i++) {
			NeironLayers[i] = vnInLayer[i + 1];
			std::vector <Neiron> nntmp(vnInLayer[i + 1], Neiron(vnInLayer[i] + 1, arch));
			//network[i].resize(nInLayer);
			network[i] = nntmp;
			//std::fill(network[i].begin(), network[i].end(),Neiron(nInLayer, arch));
		}
	}
	std::vector<double> Activate(std::vector <double> input) {
		std::vector<double> lastLayerOutput, thisLayer;
		lastLayerOutput = input;
		for (unsigned int i = 0; i < layers; i++) {
			std::vector<double> thisLayer;
			lastLayerOutput.push_back(1);
			for (unsigned int j = 0; j < NeironLayers[i]; j++) {
				thisLayer.push_back(network[i][j].work(lastLayerOutput));
			}
			lastLayerOutput = thisLayer;
		}
		output = lastLayerOutput;
		return lastLayerOutput;
	}
	void LearnNetwork(std::vector <double> Need) {
		std::vector<double> dE_dO;
		for (int i = 0; i < output.size(); i++) {
			dE_dO.push_back(output[i] - Need[i]);
		}
		for (unsigned int i = layers - 1; i > 0; i--) {
			for (unsigned int j = 0; j < NeironLayers[i]; j++) {
				network[i][j].learn(dE_dO[j], learnSpeed);
			}
			std::vector<double> dHelper(NeironLayers[i - 1], 0);
			for (unsigned int k = 0; k < NeironLayers[i - 1]; k++) {
				for (unsigned int j = 0; j < NeironLayers[i]; j++) {
					dHelper[k] += (network[i][j].d * network[i][j].W[k]);
				}
			}
			dE_dO = dHelper;
		}
		for (unsigned int j = 0; j < NeironLayers[0]; j++) {
			network[0][j].learn(dE_dO[j], learnSpeed);
		}
	}
	void changeLearningSpeed(double ls) {
		learnSpeed = ls;
	}
};
class NetworkTeacher {
public:
	void addExample(std::vector<double> Data, std::vector<double> Answer) {
		DATASET.push_back(std::make_pair(Data, Answer));
	}
	void startLearn(MyNetwork& net, double stop) {
		std::srand(static_cast<unsigned int>(time(0)));
		double TotalErrorPerCycle = stop + 1;
		double learnSpeed = 0.05;
		std::vector <double> res;
		double d1 = 10e9, d2 = 10e9;
		while (TotalErrorPerCycle >= stop) {
			net.changeLearningSpeed(learnSpeed);
			TotalErrorPerCycle = 0;
			for (unsigned int i = 0; i < DATASET.size(); i++) {
				int r = getRandomNumber(0, DATASET.size() - 1);
				res = net.Activate(DATASET[r].first);
				net.LearnNetwork(DATASET[r].second);
				/*
				std::cout << "Input {" << DATASET[r].first[0] << ", " << DATASET[r].first[1] << "} " << '\n';
				std::cout << "Output = " << res[0] << ", Answer = " << DATASET[r].second[0] << ", Error = " << oError(res[0] - DATASET[r].second[0]) << '\n';
				//*/
				for (unsigned int j = 0; j < res.size(); j++) {
					TotalErrorPerCycle += oError(res[j] - DATASET[r].second[j]);
				}
			}
			d2 = d1;
			d1 = TotalErrorPerCycle;
			cout << "_____CYCLE ENDED______ Total Error = " << TotalErrorPerCycle << ", dE = " << d1 - d2 << '\n' << "n = " << learnSpeed << '\n';
			if (learnSpeed < 0.5) {
				learnSpeed += 0.01;
			}
			else {
				if (learnSpeed < 0.9) {
					learnSpeed += 0.0001;
				}
			}
			if (TotalErrorPerCycle < stop) {
				TotalErrorPerCycle = 0;
				for (unsigned int i = 0; i < DATASET.size(); i++) {
					res = net.Activate(DATASET[i].first);
					for (unsigned int j = 0; j < res.size(); j++) {
						TotalErrorPerCycle += oError(res[j] - DATASET[i].second[j]);
					}
				}
			}
		}
	}

	void startLearnVis(MyNetwork& net, double stop, MyNetwork &netToVisual, double &TE, double &E, double &learnSpeed) {
		std::srand(static_cast<unsigned int>(time(0)));
		std::vector <double> res;
		double d1 = 10e9, d2 = 10e9;
		net.changeLearningSpeed(learnSpeed);
		int r = getRandomNumber(0, DATASET.size() - 1);
		res = net.Activate(DATASET[r].first);
		net.LearnNetwork(DATASET[r].second);
		//E += oError(res[j] - DATASET[r].second[j]);
		if (learnSpeed < 0.5) {
			learnSpeed += 0.01;
		}
		else {
			if (learnSpeed < 0.9) {
				learnSpeed += 0.0001;
			}
		}
		double TotalErrorPerCycle;
		TotalErrorPerCycle = 0;
		for (unsigned int i = 0; i < DATASET.size(); i++) {
			res = net.Activate(DATASET[i].first);
			for (unsigned int j = 0; j < res.size(); j++) {
				TotalErrorPerCycle += oError(res[j] - DATASET[i].second[j]);
			}
		}
		TE = TotalErrorPerCycle;
	}
private:
	std::vector <std::pair<std::vector <double>, std::vector <double> > > DATASET;
	int getRandomNumber(int min, int max) {
		static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
		return static_cast<int>(std::rand() * fraction * (max - min + 1) + min);
	}
	double oError(double e) {
		return e * e / 2;
	}
};



void xorAct() {
	std::vector <unsigned int> LS = {2, 4, 1};
	MyNetwork net(LS.size() - 1, LS, "Logistic", 1);
	NetworkTeacher nt;
	nt.addExample(std::vector<double>{1, 1}, std::vector<double>{0});
	nt.addExample(std::vector<double>{0, 1}, std::vector<double>{1});
	nt.addExample(std::vector<double>{1, 0}, std::vector<double>{1});
	nt.addExample(std::vector<double>{0, 0}, std::vector<double>{0});
	nt.startLearn(net, 0.1);
}

