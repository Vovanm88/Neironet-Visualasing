#include "window_classes.h"
#include "Application.h"

namespace fs = std::filesystem;

std::vector<fs::directory_entry> getAllFiles(std::string dirName)
{
    if (!fs::exists(dirName) || !fs::is_directory(dirName))
        return {};

    std::vector<fs::directory_entry> files;

    for (fs::directory_entry entry : fs::directory_iterator(dirName))
    {
        if (entry.is_directory())
        {
            auto new_files = getAllFiles(entry.path());
            files.insert(files.end(), new_files.begin(), new_files.end());
        }
        else
        {
            files.push_back(entry);
        }
    }
    return files;
}

std::string getFontPath(std::string fontName)
{
#ifdef __linux__
    std::string fontPaths[] = {"/usr/share/fonts",
                               "/usr/local/share/fonts",
                               "~/.fonts"};
#elif _WIN32
    std::string fontPaths[] = {"%WINDIR%/Fonts"};
#else
    throw "Unsupported system";
#endif

    boost::to_lower(fontName);

    std::regex font_file_regex(fontName + "\\.(ttf|otf|cff|aat|sil|psf|)");

    for (std::string path : fontPaths)
    {
        for (auto entry : getAllFiles(path))
        {
            auto name = entry.path().filename().string();
            boost::to_lower(name);
            if (std::regex_match(name, font_file_regex))
            {
                return entry.path();
            }
        }
    }

    return "";
}

MainWindowRenderer::MainWindowRenderer(Settings::MainWindowRendererSettings settings, Application &app_)
    : window(sf::VideoMode(settings.width, settings.height),
             settings.title, sf::Style::Default, settings.contextSettings),
      app(&app_),
      clearColor(settings.clearColor)
{
}

bool MainWindowRenderer::isOpen() { return window.isOpen(); }
bool MainWindowRenderer::pollEvent(sf::Event &ev) { return window.pollEvent(ev); }
void MainWindowRenderer::close()
{
    app->onWindowClose();
    window.close();
}
void MainWindowRenderer::clear() { window.clear(clearColor); }
void MainWindowRenderer::draw(sf::Drawable &obj) { window.draw(obj); }
void MainWindowRenderer::display() { window.display(); }
void MainWindowRenderer::checkInput()
{
    sf::Event ev;
    while (window.pollEvent(ev))
    {
        if (ev.type == sf::Event::Closed)
        {
            cout << "closed";
            close();
        }
        if (ev.key.code == sf::Keyboard::Escape)
        {
            cout << "escaped";
            close();
        }
    }
}

MainLayout::MainLayout(MainWindowRenderer &window_)
    : window(&window_)
{
    if (!font.loadFromFile(getFontPath(fontName)))
    {
        std::cout << "Warning: font not found. loading random font";
        assert(font.loadFromFile(getFontPath(".*")));
    }
    totalErrorText.setFont(font);
    totalErrorText.setCharacterSize(24); // in pixels, not points!
    totalErrorText.setFillColor(sf::Color::White);

    learningSpeedText.setFont(font);
    learningSpeedText.setCharacterSize(16); // in pixels, not points!
    learningSpeedText.setFillColor(sf::Color::White);
    learningSpeedText.setPosition(sf::Vector2f(500.f, 30.f));
}

void MainLayout::setTotalError(double val)
{
    sprintf(buffer, "Total Error = %f", val);
    totalErrorText.setString(buffer);
}

void MainLayout::setLearningSpeed(double val)
{
    sprintf(buffer, "Learning speed %f", val);
    learningSpeedText.setString(buffer);
}

void MainLayout::draw()
{
    window->draw(totalErrorText);
    window->draw(learningSpeedText);
}
