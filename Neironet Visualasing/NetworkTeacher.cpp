#include "NetworkTeacher.h"

void NetworkTeacher::addExample(std::vector<double> data, std::vector<double> answer)
{
	dataset.add(DataUnit(data, answer));
}
void NetworkTeacher::startLearn(double stop)
{
	double TotalErrorPerCycle = stop + 1;
	std::vector<double> res;
	//double d1 = 10e9, d2 = 10e9; // unused
	while (TotalErrorPerCycle >= stop)
	{
		net->setLearningSpeed(learnSpeed);
		TotalErrorPerCycle = 0;
		for (unsigned int i = 0; i < dataset.size(); i++)
		{
			selectRandomDataUnit();
			res = net->run(selectedDataUnit->data);
			net->correctLastRun(selectedDataUnit->answer);
			///*
			std::cout << "Input {" << selectedDataUnit->data[0] << ", " << selectedDataUnit->data[1] << "} " << '\n';
			std::cout << "Output = " << res[0] << ", Answer = " << selectedDataUnit->answer[0] << ", Error = " << oError(res[0] - selectedDataUnit->answer[0]) << '\n';
			//*/
			for (unsigned int j = 0; j < res.size(); j++)
			{
				TotalErrorPerCycle += oError(res[j] - selectedDataUnit->answer[j]);
			}
		}
		//d2 = d1; // unused
		//d1 = TotalErrorPerCycle; // unused
		//cout << "_____CYCLE ENDED______ Total Error = " << TotalErrorPerCycle << ", dE = " << d1 - d2 << '\n' << "n = " << learnSpeed << '\n';
		raiseLearnSpeed();
	}
}

void NetworkTeacher::doLearnCycle()
{
	net->setLearningSpeed(learnSpeed);
	selectRandomDataUnit();

	net->run(selectedDataUnit->data);
	net->correctLastRun(selectedDataUnit->answer);
	//	std::cout << "Input {" << selectedDataUnit->data[0] << ", " << selectedDataUnit->data[1] << "} " << '\n';
	//	std::cout << "Output = " << res[0] << ", Answer = " << selectedDataUnit->answer[0] << ", Error = " << oError(res[0] - selectedDataUnit->answer[0]) << '\n';
	raiseLearnSpeed();
}

void NetworkTeacher::raiseLearnSpeed()
{
	if (learnSpeed < 0.5)
	{
		learnSpeed += 0.00001;
	}
	else
	{
		if (learnSpeed < 0.9)
		{
			learnSpeed += 0.0000001;
		}
	}
}

double NetworkTeacher::calcTotalError()
{
	double TotalErrorPerCycle = 0;
	std::vector<double> res;
	MyNetwork network_copy = *net;
	for (unsigned int i = 0; i < dataset.size(); i++)
	{
		res = network_copy.run(dataset[i].data);
		for (unsigned int j = 0; j < res.size(); j++)
		{
			TotalErrorPerCycle += oError(res[j] - dataset[i].answer[j]);
		}
	}
	return TotalErrorPerCycle;
}

// [min; max]
int NetworkTeacher::getRandomNumber(int min, int max)
{

	//std::srand(static_cast<unsigned int>(time(0)));
	std::random_device rd;
	std::mt19937 mrsnRnd(rd());
	static const double fraction = 1.0 / (static_cast<double>(4294967296) + 1.0);
	return static_cast<int>(mrsnRnd() * fraction * (max - min + 1) + min);
}

void NetworkTeacher::selectRandomDataUnit()
{
	int r = getRandomNumber(0, dataset.size() - 1);
	selectedDataUnit = &dataset[r];
}

double NetworkTeacher::oError(double e)
{
	return e * e / 2;
}

// Getters // Setters //

void NetworkTeacher::setLearnSpeed(double value) { learnSpeed = value; }
double NetworkTeacher::getLearnSpeed() { return learnSpeed; }
void NetworkTeacher::assign(MyNetwork &network) { net = &network; }
void NetworkTeacher::setDataset(Dataset dataset_) { dataset = dataset_; }
double NetworkTeacher::getTotalError() { return calcTotalError(); }
DataUnit NetworkTeacher::getCurrentDataUnit() { return *selectedDataUnit; }
