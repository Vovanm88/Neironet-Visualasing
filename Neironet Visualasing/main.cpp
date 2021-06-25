#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib> 

//#include <thread>
//#include <mutex>

#include <string>

#include <sstream>

#include "MyNetwork.h"
#include "NetworkTeacher.h"

/*void xorAct() {
	std::vector <unsigned int> LS = {2, 8, 6, 3, 1};
	MyNetwork net(LS.size() - 1, LS, "Logistic", 1);
	NetworkTeacher nt;
	nt.addExample(std::vector<double>{1, 1}, std::vector<double>{0});
	nt.addExample(std::vector<double>{0, 1}, std::vector<double>{1});
	nt.addExample(std::vector<double>{1, 0}, std::vector<double>{1});
	nt.addExample(std::vector<double>{0, 0}, std::vector<double>{0});
	nt.startLearn(net, 0.9);
}
*/
sf::Color Colorise(double i) {
	int r, g, b;
	b = (int)(255 * std::min(1.0, std::max(0.0, i)));
	r = (int)(255 * (0-std::min(0.0, std::max(-1.0, i))));
	g = (int)(std::max(255 - (r + b), 0));
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
struct Sinaps{
	float w=0;
	sf::Color col;
	std::pair<int, int> connect = {0, 0};
	std::pair<int, int> owner = {0, 0};
};

std::vector < Sinaps > getConnections(MyNetwork& nt) {
	std::vector <std::vector <Neiron> > ntwrk = nt.getNetworkData();
	std::vector <Sinaps> output;
	for (size_t i = 0; i < ntwrk.size(); i++) {
		for (size_t j = 0; j < ntwrk[i].size(); j++) {
			for (size_t k = 0; k < ntwrk[i][j].W.size(); k++) {
				Sinaps tmp;
				tmp.w = static_cast<float>(5.0 / (1 + std::exp(5 - 3 * (std::abs(ntwrk[i][j].W[k])))) + 1);
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


int main(){
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")){
		cout << "ERROR! FONT CAN'T BE LOADED!";
		return 0;
	}

//	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	sf::RenderWindow w1(sf::VideoMode(800, 600), "Neironet", sf::Style::Default, settings);
	std::vector <unsigned int> LS = {4, 10, 6, 1};
	MyNetwork net(LS.size() - 1, LS, "Logistic", 1);
	NetworkTeacher nt;

	
	nt.addExample(std::vector<double>{1, 1, 1, 0}, std::vector<double>{0});

	//nt.startLearn(net, 0.6);
	double TE, E, learnSpeed=0.0001;
	while(w1.isOpen()){
		sf::Event ev;
		while(w1.pollEvent(ev)){
			if(ev.type ==  sf::Event::Closed){
				w1.close();
			}
			if (ev.key.code == sf::Keyboard::Escape){
				w1.close();
			}
		}
		std::vector <double> in;
		nt.startLearnVis(net, in, TE, E, learnSpeed);

		sf::Text text;
		std::stringstream sstr;
		std::string s;
		sstr << "Total Error = " << TE;
		s = sstr.str();
		text.setFont(font); // font is a sf::Font
		text.setString(s);
		text.setCharacterSize(24); // in pixels, not points!
		text.setFillColor(sf::Color::White);

		w1.clear(sf::Color(30, 30, 31));
		w1.draw(text);


		sf::Text text2;
		std::stringstream sst2;
		sst2 << "Learning speed " << learnSpeed;
		
		s = sst2.str();
		text2.setFont(font); // font is a sf::Font
		text2.setString(s);
		text2.setCharacterSize(16); // in pixels, not points!
		text2.setFillColor(sf::Color::White);
		text2.setPosition(sf::Vector2f(500.f, 30.f));
		w1.draw(text2);

		std::vector <std::vector <sf::Color> > Neirons = getColors(net, in);
		double x = 10.0, y=100.0;
		double xstep = 600 / Neirons.size();
		double ystep = 40.0;
		std::vector <sf::CircleShape> layer;
		std::vector <std::vector <sf::Vector2f> > coords(Neirons.size());
		int xi = 0;
		for (auto el : Neirons) {
			sf::CircleShape tmp(10.f);
			y = (600.f-((el.size()-1)*40+10))/2;
			for (auto color : el) {
				tmp.setFillColor(color);
				tmp.setOutlineThickness(2.f);
				tmp.setOutlineColor(sf::Color(255, 255, 255));
				tmp.setPosition(sf::Vector2f(x, y));
				
				layer.push_back(tmp);
				coords[xi].push_back(sf::Vector2f(x, y+10.f));
				y += ystep;
			}
			xi++;
			x += xstep;
		}
		for (auto el : layer) {
			w1.draw(el);
		}
		std::vector < Sinaps > snp = getConnections(net);
		for (const auto &el : snp) {
		//auto el = snp[4];
			
			sf::Vector2f f1, f2, f3;
			f1 = coords[el.connect.first][el.connect.second];
			f1.x = f1.x + 20.f;
			f2 = coords[el.owner.first][el.owner.second];
			f3.x = f1.x;
			f3.y = f2.y;

			double l, t;
			double a;
			
			t = el.w;
			l = lenV2f(f1, f2);
			a = std::asin(lenV2f(f1, f3) / l)/3.141592653589793238462643 * 180;
			if (f1.y > f2.y) {
				a = -a;
			}
			sf::RectangleShape tmp(sf::Vector2f(l, t));
			tmp.setFillColor(el.col);
			f1.y = f1.y-t/2;
			tmp.setPosition(f1);
			tmp.setRotation(a);
			w1.draw(tmp);
			/*
			sf::RectangleShape tmp2(sf::Vector2f(l, t));
			tmp2.setFillColor(el.col);
			f2.x = f2.x + 20.f;
			tmp2.setPosition(f2);
			tmp2.setRotation(a);
			w1.draw(tmp2);
			*/
		}
		w1.display();
	}
}