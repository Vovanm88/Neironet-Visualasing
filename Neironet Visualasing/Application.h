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
    void onClose();
    private:
    bool is_running;
    Settings settings;
    MainWindowRenderer window;
    MainLayout layout;
    NetworkTeacher teacher;
    MyNetwork net;
};
