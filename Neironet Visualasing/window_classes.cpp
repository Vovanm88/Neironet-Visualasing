#include "window_classes.h"
#include "Application.h"

namespace fs = std::filesystem;

std::vector<fs::directory_entry> GetAllFiles(std::string dirName)
{
    if (!fs::exists(dirName) || !fs::is_directory(dirName))
        return {};

    std::vector<fs::directory_entry> files;

    for (fs::directory_entry entry : fs::directory_iterator(dirName))
    {
        if (entry.is_directory())
        {
            auto new_files = GetAllFiles(entry.path());
            files.insert(files.end(), new_files.begin(), new_files.end());
        }
        else
        {
            files.push_back(entry);
        }
    }
    return files;
}

std::string GetFontPath(std::string fontName)
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
        for (auto entry : GetAllFiles(path))
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
             settings.title, sf::Style::Default, settings.context_settings),
    app(&app_),
    clear_color(settings.clear_color)
{
}

bool MainWindowRenderer::isOpen() { return window.isOpen(); }
bool MainWindowRenderer::pollEvent(sf::Event &ev) { return window.pollEvent(ev); }
void MainWindowRenderer::close()
{
    app->onClose();
    window.close();
}
void MainWindowRenderer::clear() { window.clear(clear_color); }
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

MainLayout::MainLayout(MainWindowRenderer &window)
{
    _window = &window;
    if (!_font.loadFromFile(GetFontPath(_fontName)))
    {
        std::cout << "Warning: font not found. loading random font";
        assert(_font.loadFromFile(GetFontPath(".*")));
    }
    _totalErrorText.setFont(_font);
    _totalErrorText.setCharacterSize(24); // in pixels, not points!
    _totalErrorText.setFillColor(sf::Color::White);

    _learningSpeedText.setFont(_font);
    _learningSpeedText.setCharacterSize(16); // in pixels, not points!
    _learningSpeedText.setFillColor(sf::Color::White);
    _learningSpeedText.setPosition(sf::Vector2f(500.f, 30.f));
}

void MainLayout::SetTotalError(double val)
{
    sprintf(buffer, "Total Error = %f", val);
    _totalErrorText.setString(buffer);
}

void MainLayout::SetLearningSpeed(double val)
{
    sprintf(buffer, "Learning speed %f", val);
    _learningSpeedText.setString(buffer);
}

void MainLayout::Draw()
{
    _window->draw(_totalErrorText);
    _window->draw(_learningSpeedText);
}
