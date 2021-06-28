#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Dataset.h"

class Settings
{
    public:
    struct MainWindowRendererSettings
    {
        unsigned int width = 800;
        unsigned int height = 600;
        std::string title = "Neironet";
        sf::ContextSettings contextSettings{
            0, // depth (0)
            0, // stencil (0)
            8, // antialiasing (0)
            1, // major (1)
            1, // minor (1)
            0, // arrtributes (Default = 0)
            false // sRgb (false)
        };
        sf::Color clearColor = sf::Color(30, 30, 31);
    };
    MainWindowRendererSettings mainWindowRendererSettings;
    std::vector<unsigned int> netLayers = {1, 1, 1};
    double startLearningSpeed = 0.0001;
    Dataset dataset{{
        DataUnit({1},{1}),
        DataUnit({0},{0}),
    }};
};
