#include "Neiron.h"
#include <iostream>
#include <algorithm>
Neiron::Neiron(int size, std::string arc, std::mt19937& mrsnRnd) {
	//std::srand(static_cast<unsigned int>(time(0)));
	/*for (int i = 0; i < 100; i++) {
		std::cout << rnd(mrsnRnd) << " ";
	}
	//*/
	W.resize(size);
	for (double& weight : W) {
		weight = rnd(mrsnRnd);
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
double Neiron::work(std::vector <double>& input) {
	S = input;
	output = activationP(input);
	return output;
}
void Neiron::learn(double dE_do, double lspeed) {
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
Neiron& Neiron::operator= (const Neiron& neiron) {

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

double Neiron::findSum(std::vector <double> ss) {
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
double Neiron::ReLU(double x, bool d = false) {
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
double Neiron::ISRU(double x, bool d = false) {
	if (!d) {
		return x / std::sqrt(1 + x * x);
	}
	else {
		double tmp = x / std::sqrt(1 + x * x);
		return tmp * tmp * tmp;
	}
}
double Neiron::Softsign(double x, bool d = false) {
	if (!d) {
		return x / (1 + std::fabs(x));
	}
	else {
		double tmp = (1 + std::fabs(x));
		return x / (tmp * tmp);
	}
}
double Neiron::Logistic(double x, bool d = false) {
	if (!d) {
		return 1.0 / (1 + std::exp(-x));
	}
	else {
		return Logistic(x) * (1 - Logistic(x));
	}
}
double Neiron::activationP(std::vector <double>& input) {
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
double Neiron::rnd(std::mt19937& mrsnRnd) {
	//std::random_device rd;
	//std::mt19937 mrsnRnd(rd());
	unsigned int r = mrsnRnd();
	long long r2 = r % 16384;
	return (double)r2/ 16384.0 - 0.45;
	//return 1;
}
