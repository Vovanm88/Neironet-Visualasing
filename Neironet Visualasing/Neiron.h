#pragma once
#include <cmath>
#include <vector>
#include <random>

enum NeironArch
{
	ReLU = 0,
	ISRU = 1,
	Softsign = 2,
	Logistic = 3,
};

class Neiron
{
public:
	Neiron(int size, std::mt19937 &mrsnRnd);
	static Neiron *factory(int size, NeironArch arc, std::mt19937 &mrsnRnd);
	virtual Neiron *clone() = 0;
	double work(std::vector<double> &input);
	void learn(double dE_do, double lspeed);
	Neiron &operator=(const Neiron &neiron);
	double output;		   //o(j)
	std::vector<double> weights; // w(j)[i]
	double d;

private:
	std::vector<double> S;
	double Sum; //net(j)
	unsigned int size = 0;
	int arch = 0;
	char NEXTNEIRON________________NEXT;
	virtual double postprocessSum(double x, bool d) = 0; // TODO get proper naming
	double activationP(std::vector<double> &input);
	double rnd(std::mt19937 &mrsnRnd);
};

class NeironReLU : public Neiron
{
	using Neiron::Neiron;
	Neiron *clone() override;
private:
	double postprocessSum(double x, bool d) override;
};

class NeironISRU : public Neiron
{
	using Neiron::Neiron;
	Neiron *clone() override;
private:
	double postprocessSum(double x, bool d) override;
};

class NeironSoftsign : public Neiron
{
	using Neiron::Neiron;
	Neiron *clone() override;
private:
	double postprocessSum(double x, bool d) override;
};

class NeironLogistic : public Neiron
{
	using Neiron::Neiron;
	Neiron *clone() override;
private:
	double postprocessSum(double x, bool d) override;
};
