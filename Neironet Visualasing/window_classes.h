#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include <filesystem>

#include "Settings.h"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <regex>
#include <filesystem>

#ifdef _WIN32
#include <format>
#endif

namespace fs = std::filesystem;

std::vector<fs::directory_entry> getAllFiles(std::string dirName);

std::string getFontPath(std::string fontName);

class Application;

class MainWindowRenderer
{
public:
    MainWindowRenderer(Settings::MainWindowRendererSettings settings, Application &app);

    sf::Font font;

    bool isOpen();
    bool pollEvent(sf::Event &ev);
    void close();
    void clear();
    void draw(sf::Drawable &obj);
    void display();
    void checkInput();

private:
    sf::RenderWindow window;
    sf::Color clearColor;
    Application *app;
};


MainWindowRenderer InitWindow();

class MainLayout
{
public:
    MainLayout(MainWindowRenderer &window_);
    void setTotalError(double val);
    void setLearningSpeed(double val);
    void draw();
private:
    const std::string fontName = "Arial";
    sf::Font font;
    MainWindowRenderer *window;
    sf::Text totalErrorText;
    sf::Text learningSpeedText;
    char buffer[32];
};
