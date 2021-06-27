#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib> 

//#include <thread>
//#include <mutex>

#include <string>

//#include <format>

#include <sstream>

#include "MyNetwork.h"
#include "NetworkTeacher.h"

//void xorAct();

sf::Color Colorise(double i);

std::vector <std::vector <sf::Color> > getColors(MyNetwork &nt, std::vector <double> input);
struct Sinaps{
	float w=0;
	sf::Color col;
	std::pair<int, int> connect = {0, 0};
	std::pair<int, int> owner = {0, 0};
};

std::vector < Sinaps > getConnections(MyNetwork& nt);
double lenV2f(sf::Vector2f a, sf::Vector2f b);