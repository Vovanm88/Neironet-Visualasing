#include "NetworkTeacher.h"

void NetworkTeacher::addExample(std::vector<double> Data, std::vector<double> Answer) {
	DATASET.push_back(std::make_pair(Data, Answer));
}
void NetworkTeacher::startLearn(MyNetwork& net, double stop) {
	//std::srand(static_cast<unsigned int>(time(0)));
	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	double TotalErrorPerCycle = stop + 1;
	double learnSpeed = 0.05;
	std::vector <double> res;
	double d1 = 10e9, d2 = 10e9;
	while (TotalErrorPerCycle >= stop) {
		net.changeLearningSpeed(learnSpeed);
		TotalErrorPerCycle = 0;
		for (unsigned int i = 0; i < DATASET.size(); i++) {
			int r = getRandomNumber(0, DATASET.size() - 1, mrsnRnd);
			res = net.Activate(DATASET[r].first);
			net.LearnNetwork(DATASET[r].second);
			///*
			std::cout << "Input {" << DATASET[r].first[0] << ", " << DATASET[r].first[1] << "} " << '\n';
			std::cout << "Output = " << res[0] << ", Answer = " << DATASET[r].second[0] << ", Error = " << oError(res[0] - DATASET[r].second[0]) << '\n';
			//*/
			for (unsigned int j = 0; j < res.size(); j++) {
				TotalErrorPerCycle += oError(res[j] - DATASET[r].second[j]);
			}
		}
		d2 = d1;
		d1 = TotalErrorPerCycle;
		//cout << "_____CYCLE ENDED______ Total Error = " << TotalErrorPerCycle << ", dE = " << d1 - d2 << '\n' << "n = " << learnSpeed << '\n';
		if (learnSpeed < 0.5) {
			learnSpeed += 0.0001;
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

void NetworkTeacher::startLearnVis(MyNetwork& net, std::vector<double>& in, double& TE, double& E, double& learnSpeed) {
	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	std::vector <double> res;
	double d1 = 10e9, d2 = 10e9;
	net.changeLearningSpeed(learnSpeed);
	int r = getRandomNumber(0, DATASET.size() - 1, mrsnRnd);
	in = DATASET[r].first;

	res = net.Activate(DATASET[r].first);
	net.LearnNetwork(DATASET[r].second);
	E += oError(res[0] - DATASET[r].second[0]);
//	std::cout << "Input {" << DATASET[r].first[0] << ", " << DATASET[r].first[1] << "} " << '\n';
//	std::cout << "Output = " << res[0] << ", Answer = " << DATASET[r].second[0] << ", Error = " << oError(res[0] - DATASET[r].second[0]) << '\n';
	if (learnSpeed < 0.5) {
		learnSpeed += 0.00001;
	}
	else {
		if (learnSpeed < 0.9) {
			learnSpeed += 0.0000001;
		}
	}
	double TotalErrorPerCycle;
	TotalErrorPerCycle = 0;
	MyNetwork nt2tmp = net;
	for (unsigned int i = 0; i < DATASET.size(); i++) {
		res = nt2tmp.Activate(DATASET[i].first);
		for (unsigned int j = 0; j < res.size(); j++) {
			TotalErrorPerCycle += oError(res[j] - DATASET[i].second[j]);
		}
	}
	TE = TotalErrorPerCycle;
}

int NetworkTeacher::getRandomNumber(int min, int max, std::mt19937& mrsnRnd) {

	static const double fraction = 1.0 / (static_cast<double>(4294967296) + 1.0);
	return static_cast<int>(mrsnRnd() * fraction * (max - min + 1) + min);
}
double NetworkTeacher::oError(double e) {
	return e * e / 2;
}
