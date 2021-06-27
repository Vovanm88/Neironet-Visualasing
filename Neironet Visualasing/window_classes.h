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

namespace fs = std::filesystem;

std::vector<fs::directory_entry> GetAllFiles(std::string dirName);

std::string GetFontPath(std::string fontName);

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
    sf::Color clear_color;
    Application *app;
};


MainWindowRenderer InitWindow();

class MainLayout
{
public:
    MainLayout(MainWindowRenderer &window);
    void SetTotalError(double val);
    void SetLearningSpeed(double val);
    void Draw();
private:
    const std::string _fontName = "Arial";
    sf::Font _font;
    MainWindowRenderer *_window;
    sf::Text _totalErrorText;
    sf::Text _learningSpeedText;
    char buffer[32];
};
