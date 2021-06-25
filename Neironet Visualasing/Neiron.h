#pragma once
#include <cmath>
#include <vector>
#include <random>
class Neiron {
public:
	Neiron(int size, std::string arc, std::mt19937& mrsnRnd);
	double work(std::vector <double>& input);
	void learn(double dE_do, double lspeed);
	Neiron& operator= (const Neiron& neiron);
	double output; //o(j)
	std::vector <double> W; // w(j)[i]
	double d;
private:
	std::vector <double> S;
	double Sum; //net(j)
	unsigned int size = 0;
	int arch = 0;
	char NEXTNEIRON________________NEXT;
	double findSum(std::vector <double> ss);
	double ReLU(double x, bool d);
	double ISRU(double x, bool d);
	double Softsign(double x, bool d);
	double Logistic(double x, bool d);
	double activationP(std::vector <double>& input);
	double rnd(std::mt19937& mrsnRnd);

};
