#include "Application.h"

#include "main_funcs.h"

Application::Application(Settings settings_)
    : settings(settings_),
      window(settings.mainWindowRendererSettings, *this),
      layout(window),
      net(settings.netLayers.size() - 1, settings.netLayers, NeironArch::Logistic, 1)
{
}

void Application::init()
{
    //	net.loadData("neironetData.ndat");
    /*
		teacher.addExample(std::vector<double>{1, 1, 1, 0}, std::vector<double>{0});
		teacher.addExample(std::vector<double>{1, 1, 0, 0}, std::vector<double>{0});
		teacher.addExample(std::vector<double>{1, 0, 1, 1}, std::vector<double>{1});
		teacher.addExample(std::vector<double>{1, 0, 0, 1}, std::vector<double>{1});
		teacher.addExample(std::vector<double>{1, 0, 0, 0}, std::vector<double>{1});
		teacher.addExample(std::vector<double>{0, 1, 1, 0}, std::vector<double>{0});
		teacher.addExample(std::vector<double>{0, 1, 0, 1}, std::vector<double>{0});
		teacher.addExample(std::vector<double>{0, 1, 0, 0}, std::vector<double>{0});
		teacher.addExample(std::vector<double>{0, 0, 0, 0}, std::vector<double>{1});
		*/
    teacher.setDataset(settings.learnDataset);
    teacher.setLearnSpeed(settings.startLearningSpeed);
    teacher.assign(net);

    tester.setDataset(settings.testingDataset);
    tester.assign(net);
    //teacher.startLearn(0.6);
}

void Application::start()
{
    while (window.isOpen())
    {
        loop();
    }
}

void Application::loop()
{
    window.checkInput();
    teacher.doLearnCycle();
    updateGraphics();
}

void Application::updateGraphics()
{
    window.clear();

    layout.setLearnAveregeError(teacher.getTotalError());
    layout.setTestingAveregeError(tester.getTotalError());
    layout.setLearningSpeed(teacher.getLearnSpeed());
    layout.draw();

    std::vector<std::vector<sf::Color>> Neirons = getColors(net, teacher.getCurrentDataUnit().data);
    double x = 10.0, y = 100.0;
    double xstep = 600 / Neirons.size();
    double ystep = 40.0;
    std::vector<sf::CircleShape> layer;
    std::vector<std::vector<sf::Vector2f>> coords(Neirons.size());
    int xi = 0;
    for (auto el : Neirons)
    {
        sf::CircleShape tmp(10.f);
        y = (600.f - ((el.size() - 1) * 40 + 10)) / 2;
        for (auto color : el)
        {
            tmp.setFillColor(color);
            tmp.setOutlineThickness(2.f);
            tmp.setOutlineColor(sf::Color(255, 255, 255));
            tmp.setPosition(sf::Vector2f(x, y));

            layer.push_back(tmp);
            coords[xi].push_back(sf::Vector2f(x, y + 10.f));
            y += ystep;
        }
        xi++;
        x += xstep;
    }
    for (auto el : layer)
    {
        window.draw(el);
    }
    std::vector<Sinaps> snp = getConnections(net);
    for (const auto &el : snp)
    {
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
        a = std::asin(lenV2f(f1, f3) / l) / 3.141592653589793238462643 * 180;
        if (f1.y > f2.y)
        {
            a = -a;
        }
        sf::RectangleShape tmp(sf::Vector2f(l, t));
        tmp.setFillColor(el.col);
        f1.y = f1.y - t / 2;
        tmp.setPosition(f1);
        tmp.setRotation(a);
        window.draw(tmp);
    }

    window.display();
}

void Application::onWindowClose()
{
    //cout << "saveData" << &net;
    net.saveData("neironetData.ndat");
}
