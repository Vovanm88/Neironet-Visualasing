#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>

//#include <thread>
//#include <mutex>

#include <string>

#include <sstream>

class Application;

#include "window_classes.h"
#include "NetworkTeacher.h"
#include "MyNetwork.h"
#include "Settings.h"


class Application
{
	public:
    Application(Settings settings_);
    void init();
	void start();
    void onWindowClose();
    private:
    void loop();
    void updateGraphics();
    Settings settings;
    MainWindowRenderer window;
    MainLayout layout;
    NetworkTeacher teacher;
    NetworkTeacher tester;
    MyNetwork net;
};
