#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Settings
{
    public:
    struct MainWindowRendererSettings
    {
        unsigned int width = 800;
        unsigned int height = 600;
        std::string title = "Neironet";
        sf::ContextSettings context_settings{
            0, // depth (0)
            0, // stencil (0)
            8, // antialiasing (0)
            1, // major (1)
            1, // minor (1)
            0, // arrtributes (Default = 0)
            false // sRgb (false)
        };
        sf::Color clear_color = sf::Color(30, 30, 31);
    };
    MainWindowRendererSettings mainWindowRendererSettings;
    std::vector<unsigned int> net_layers = {1, 1, 1};
};
