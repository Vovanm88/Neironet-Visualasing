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

/*void xorAct() {
	std::vector <unsigned int> LS = {2, 8, 6, 3, 1};
	MyNetwork net(LS.size() - 1, LS, "Logistic", 1);
	NetworkTeacher nt;
	nt.addExample(std::vector<double>{1, 1}, std::vector<double>{0});
	nt.addExample(std::vector<double>{0, 1}, std::vector<double>{1});
	nt.addExample(std::vector<double>{1, 0}, std::vector<double>{1});
	nt.addExample(std::vector<double>{0, 0}, std::vector<double>{0});
	nt.startLearn(net, 0.9);
}
*/
sf::Color Colorise(double i) {
	int r, g, b;
	/*
	b = (int)(255 * std::min(1.0, std::max(0.0, i)));
	r = (int)(255 * (0-std::min(0.0, std::max(-1.0, i))));
	g = (int)(std::max(255 - (r + b), 0));
	*/
	r = (1 - std::max(i, 0.0)) * 255;
	g = (1 - std::fabs(i)) * 255;
	b = (1 - std::max(-i, 0.0)) * 255;
	return sf::Color(r, g, b);
}

std::vector <std::vector <sf::Color> > getColors(MyNetwork &nt, std::vector <double> input){
	std::vector <std::vector <Neiron> > ntwrk = nt.getNetworkData();
	std::vector <std::vector <sf::Color> > output(ntwrk.size()+1);
	for (double el : input) {
		output[0].push_back(Colorise(el));
	}
	output[0].push_back(sf::Color(255, 255, 0));
	for (size_t i = 0; i < ntwrk.size(); i++) {
		for (size_t j = 0; j < ntwrk[i].size(); j++) {
			output[i+1].push_back(Colorise(ntwrk[i][j].output));
		}
		if(i+1<ntwrk.size())
			output[i+1].push_back(sf::Color(255, 255, 0));
	}
	return output;
}
struct Sinaps{
	float w=0;
	sf::Color col;
	std::pair<int, int> connect = {0, 0};
	std::pair<int, int> owner = {0, 0};
};

std::vector < Sinaps > getConnections(MyNetwork& nt) {
	std::vector <std::vector <Neiron> > ntwrk = nt.getNetworkData();
	std::vector <Sinaps> output;
	for (size_t i = 0; i < ntwrk.size(); i++) {
		for (size_t j = 0; j < ntwrk[i].size(); j++) {
			for (size_t k = 0; k < ntwrk[i][j].W.size(); k++) {
				Sinaps tmp;
				tmp.w = static_cast<float>(4.0 / (1 + std::exp(3 - (std::abs(ntwrk[i][j].W[k]))))+0.6);
				if (ntwrk[i][j].W[k] < 0) {
					tmp.col =sf::Color(255, 0, 0);
				}else {
					tmp.col = sf::Color(0, 0, 255);
				}
				tmp.owner = {i+1, j};
				tmp.connect = {i, k};
				output.push_back(tmp);
			}
		}
	}
	return output;
}
double lenV2f(sf::Vector2f a, sf::Vector2f b) {
	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

#include <boost/algorithm/string.hpp>
#include <regex>
#include <filesystem>
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

struct MainWindowRenderrerSettings
{
	public:
	unsigned int width = 800;
	unsigned int height = 600;
	std::string title = "Neironet";
	sf::ContextSettings contextSettings;
	sf::Color clearColor = sf::Color(30, 30, 31);
};

class MainWindowRenderrer
{	
	public:
	MainWindowRenderrer(MainWindowRenderrerSettings settings)
	: window(sf::VideoMode(settings.width, settings.height),
		settings.title, sf::Style::Default, settings.contextSettings)
	{
		clearColor = settings.clearColor;
	}

	sf::Font font;

	bool isOpen() { return window.isOpen(); }
	bool pollEvent(sf::Event &ev) { return window.pollEvent(ev); }
	void close() { window.close(); }
	void clear() { window.clear(clearColor); }
	void draw(sf::Drawable &obj) { window.draw(obj); }
	void display() { window.display(); }

	private:
	sf::RenderWindow window;
	sf::Color clearColor;
};

MainWindowRenderrer InitWindow()
{
	MainWindowRenderrerSettings settings;
	settings.contextSettings.antialiasingLevel = 8;
	return MainWindowRenderrer(settings);
}

class MainLayout
{
	public:
	MainLayout(MainWindowRenderrer &window)
	{
		_window = &window;
		if (!_font.loadFromFile(GetFontPath(_fontName))){
			cout << "Warning: font not found. loading random font";
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

	void SetTotalError(double val)
	{
		sprintf(buffer, "Total Error = %f", val);
		_totalErrorText.setString(buffer);
	}

	void SetLearningSpeed(double val)
	{
		sprintf(buffer, "Learning speed %f", val);
		_learningSpeedText.setString(buffer);
	}

	void Draw()
	{
		_window->draw(_totalErrorText);
		_window->draw(_learningSpeedText);
	}

	private:
	const std::string _fontName = "Arial";
	sf::Font _font;
	MainWindowRenderrer *_window;
	sf::Text _totalErrorText;
	sf::Text _learningSpeedText;

	char buffer[32];
};

int main(){
	MainWindowRenderrer window = InitWindow();
	MainLayout layout(window);

	//std::vector <unsigned int> LS = {4, 12, 5, 1};
	std::vector <unsigned int> LS = {1, 1, 1};

	
	MyNetwork net(LS.size() - 1, LS, "Logistic", 1);
	NetworkTeacher nt;
//	net.loadData("neironetData.ndat");
/*
	nt.addExample(std::vector<double>{1, 1, 1, 0}, std::vector<double>{0});
	nt.addExample(std::vector<double>{1, 1, 0, 0}, std::vector<double>{0});
	nt.addExample(std::vector<double>{1, 0, 1, 1}, std::vector<double>{1});
	nt.addExample(std::vector<double>{1, 0, 0, 1}, std::vector<double>{1});
	nt.addExample(std::vector<double>{1, 0, 0, 0}, std::vector<double>{1});
	nt.addExample(std::vector<double>{0, 1, 1, 0}, std::vector<double>{0});
	nt.addExample(std::vector<double>{0, 1, 0, 1}, std::vector<double>{0});
	nt.addExample(std::vector<double>{0, 1, 0, 0}, std::vector<double>{0});
	nt.addExample(std::vector<double>{0, 0, 0, 0}, std::vector<double>{1});
	*/
	//
	nt.addExample(std::vector<double>{1}, std::vector<double>{1});
	nt.addExample(std::vector<double>{0}, std::vector<double>{0});
	//nt.startLearn(net, 0.6);
	double TE, E, learnSpeed=0.0001;
	while(window.isOpen()){
		sf::Event ev;
		while(window.pollEvent(ev)){
			if(ev.type ==  sf::Event::Closed){
				net.saveData("neironetData.ndat");
				window.close();
			}
			if (ev.key.code == sf::Keyboard::Escape){
				net.saveData("neironetData.ndat");
				window.close();
			}
		}
		std::vector <double> in;
		nt.startLearnVis(net, in, TE, E, learnSpeed);

		window.clear();

		layout.SetTotalError(TE);
		layout.SetLearningSpeed(learnSpeed);
		layout.Draw();

		std::vector <std::vector <sf::Color> > Neirons = getColors(net, in);
		double x = 10.0, y=100.0;
		double xstep = 600 / Neirons.size();
		double ystep = 40.0;
		std::vector <sf::CircleShape> layer;
		std::vector <std::vector <sf::Vector2f> > coords(Neirons.size());
		int xi = 0;
		for (auto el : Neirons) {
			sf::CircleShape tmp(10.f);
			y = (600.f-((el.size()-1)*40+10))/2;
			for (auto color : el) {
				tmp.setFillColor(color);
				tmp.setOutlineThickness(2.f);
				tmp.setOutlineColor(sf::Color(255, 255, 255));
				tmp.setPosition(sf::Vector2f(x, y));
				
				layer.push_back(tmp);
				coords[xi].push_back(sf::Vector2f(x, y+10.f));
				y += ystep;
			}
			xi++;
			x += xstep;
		}
		for (auto el : layer) {
			window.draw(el);
		}
		std::vector < Sinaps > snp = getConnections(net);
		for (const auto &el : snp) {
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
			a = std::asin(lenV2f(f1, f3) / l)/3.141592653589793238462643 * 180;
			if (f1.y > f2.y) {
				a = -a;
			}
			sf::RectangleShape tmp(sf::Vector2f(l, t));
			tmp.setFillColor(el.col);
			f1.y = f1.y-t/2;
			tmp.setPosition(f1);
			tmp.setRotation(a);
			window.draw(tmp);
		}
		window.display();
	}
}
