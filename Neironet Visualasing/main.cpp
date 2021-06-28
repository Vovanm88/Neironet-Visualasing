#include "main_funcs.h"

sf::Color Colorise(double i) {
	int r, g, b;
	/*
	b = (int)(255 * std::min(1.0, std::max(0.0, i)));
	r = (int)(255 * (0-std::min(0.0, std::max(-1.0, i))));
	g = (int)(std::max(255 - (r + b), 0));
	*/
	r = (1 - std::max(i, 0.0)) * 255;
	g = (1 - std::fabs(i)) * 255;
	b = (1 - std::max(-i, 0.0)) * 255;
	return sf::Color(r, g, b);
}

std::vector <std::vector <sf::Color> > getColors(MyNetwork &nt, std::vector <double> input){
	std::vector <std::vector <Neiron> > ntwrk = nt.getNetworkData();
	std::vector <std::vector <sf::Color> > output(ntwrk.size()+1);
	for (double el : input) {
		output[0].push_back(Colorise(el));
	}
	output[0].push_back(sf::Color(255, 255, 0));
	for (size_t i = 0; i < ntwrk.size(); i++) {
		for (size_t j = 0; j < ntwrk[i].size(); j++) {
			output[i+1].push_back(Colorise(ntwrk[i][j].output));
		}
		if(i+1<ntwrk.size())
			output[i+1].push_back(sf::Color(255, 255, 0));
	}
	return output;
}


std::vector < Sinaps > getConnections(MyNetwork& nt) {
	std::vector <std::vector <Neiron> > ntwrk = nt.getNetworkData();
	std::vector <Sinaps> output;
	for (size_t i = 0; i < ntwrk.size(); i++) {
		for (size_t j = 0; j < ntwrk[i].size(); j++) {
			for (size_t k = 0; k < ntwrk[i][j].W.size(); k++) {
				Sinaps tmp;
				tmp.w = static_cast<float>(4.0 / (1 + std::exp(3 - (std::abs(ntwrk[i][j].W[k]))))+0.6);
				if (ntwrk[i][j].W[k] < 0) {
					tmp.col =sf::Color(255, 0, 0);
				}else {
					tmp.col = sf::Color(0, 0, 255);
				}
				tmp.owner = {i+1, j};
				tmp.connect = {i, k};
				output.push_back(tmp);
			}
		}
	}
	return output;
}
double lenV2f(sf::Vector2f a, sf::Vector2f b) {
	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

#include "Application.h"

void launchApp()
{
	Settings settings;
	Application app(settings);
	app.init();
	app.start();
}

int main()
{
	launchApp();
}
