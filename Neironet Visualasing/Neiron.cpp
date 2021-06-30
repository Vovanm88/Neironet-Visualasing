#include "Neiron.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

Neiron *Neiron::factory(int size, NeironArch arch, std::mt19937& mrsnRnd)
{
	switch (arch)
	{
		case NeironArch::ReLU:
			return new NeironReLU(size, mrsnRnd);
		case NeironArch::ISRU:
			return new NeironISRU(size, mrsnRnd);
		case NeironArch::Logistic:
			return new NeironLogistic(size, mrsnRnd);
		case NeironArch::Softsign:
			return new NeironSoftsign(size, mrsnRnd);
	}
	throw std::runtime_error("unsupported NeironArch");
}

Neiron::Neiron(int size, std::mt19937& mrsnRnd) {
	//std::srand(static_cast<unsigned int>(time(0)));
	/*for (int i = 0; i < 100; i++) {
		std::cout << rnd(mrsnRnd) << " ";
	}
	//*/
	weights.resize(size);
	for (double& weight : weights) {
		weight = rnd(mrsnRnd);
	}
}
double Neiron::work(std::vector <double>& input) {
	S = input;
	output = activationP(input);
	return output;
}
void Neiron::learn(double errorValue, double lspeed) {
	double do_dnet = postprocessSum(output, true);
	d = errorValue * do_dnet;
	for (unsigned int i = 0; i < weights.size(); i++) {
		weights[i] += -lspeed * d * S[i];
	}
}
Neiron& Neiron::operator= (const Neiron& neiron) {

	if (this == &neiron)
		return *this;

	output = neiron.output;
	weights = neiron.weights;
	d = neiron.d;
	S = neiron.S;
	Sum = neiron.Sum;
	size = neiron.size;
	arch = neiron.arch;
	return *this;
}

double NeironReLU::postprocessSum(double x, bool d = false) {
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
double NeironISRU::postprocessSum(double x, bool d = false) {
	if (!d) {
		return x / std::sqrt(1 + x * x);
	}
	else {
		double tmp = x / std::sqrt(1 + x * x);
		return tmp * tmp * tmp;
	}
}
double NeironSoftsign::postprocessSum(double x, bool d = false) {
	if (!d) {
		return x / (1 + std::fabs(x));
	}
	else {
		double tmp = (1 + std::fabs(x));
		return x / (tmp * tmp);
	}
}
double NeironLogistic::postprocessSum(double x, bool d = false) {
	if (!d) {
		return 1.0 / (1 + std::exp(-x));
	}
	else {
		return postprocessSum(x) * (1 - postprocessSum(x));
	}
}
double Neiron::activationP(std::vector <double>& input) {
	if (input.size() <= weights.size()) {
		std::vector <double> tmp;
		for (unsigned int i = 0; i < input.size(); i++) {
			tmp.push_back(input[i] * weights[i]);
		}
		Sum = std::reduce(tmp.begin(), tmp.end());
		return postprocessSum(Sum, false);
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

Neiron *NeironReLU::clone() { return new NeironReLU(*this); }
Neiron *NeironISRU::clone() { return new NeironISRU(*this); }
Neiron *NeironSoftsign::clone() { return new NeironSoftsign(*this); }
Neiron *NeironLogistic::clone() { return new NeironLogistic(*this); }
