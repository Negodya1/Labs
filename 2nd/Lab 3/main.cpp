#include <iostream>
#include <SDL.h>
#include <vector>
#include <cmath>

const short int SCREEN_WIDTH = 640;
const short int SCREEN_HEIGHT = 480;

short int xplus = 0;
short int yplus = 0;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Rect rect;

const Uint8* _keyPressed = SDL_GetKeyboardState(NULL);
bool _keyLocked = false;

#define PI 3.14159265

class ErrorValue {
public:
	ErrorValue(short int id = 0, int wrong = 0) {
		_id = id;

		_whatsGoingOn = "Negative or zero value for ";
		if (id == -1) _whatsGoingOn += "Width\n";
		else if (id == -2) _whatsGoingOn += "Length\n";
		else if (id == -3) _whatsGoingOn += "Radius\n";
		else if (id == -4) _whatsGoingOn += "Height\n";

		_wrongValue = wrong;
	}

	std::string _whatsGoingOn;
	short int _id;
	int _wrongValue;

	void printInfo() {
		std::cout << _whatsGoingOn;
		std::cout << "Wrong value = " << _wrongValue;
		std::cout << "\n\n";
	}
};

class ErrorButton {
public:
	ErrorButton() {
		_whatsGoingOn = "Wrong button\n\n";
	}

	std::string _whatsGoingOn;


	void printInfo() {
		std::cout << _whatsGoingOn;
	}
};

class ErrorZoom {
public:
	ErrorZoom() {
		_whatsGoingOn = "Negative zoom value\n\n";
	}

	std::string _whatsGoingOn;


	void printInfo() {
		std::cout << _whatsGoingOn;
	}
};

class ErrorBorder {
public:
	ErrorBorder(std::string brdr = "up") {
		_whatsGoingOn = "Object in collision with ";
		_whatsGoingOn += brdr;
		_whatsGoingOn += " window border\n\n";
	}

	std::string _whatsGoingOn;

	void printInfo() {
		std::cout << _whatsGoingOn;
	}
};

class Funcs {
public:
	static std::vector<int> radiusPos(double degree, double radius) {
		std::vector<int> res = { 0, 0 };

		if (degree == 0) res[0] = radius;
		else if (degree == 180) res[0] -= radius;
		else if (degree == 90) res[1] -= radius;
		else if (degree == 270) res[1] = radius;
		else if (degree == 45) {
			res[0] = radius * sin(45 * PI / 180);
			res[1] -= radius * sin(45 * PI / 180);
		}
		else if (degree == 135) {
			res[0] -= radius * sin(135 * PI / 180);
			res[1] -= radius * sin(135 * PI / 180);
		}
		else if (degree == 225) {
			res[0] -= radius * sin(225 * PI / 180);
			res[1] = radius * sin(225 * PI / 180);
		}
		else if (degree == 315) {
			res[0] = radius * sin(315 * PI / 180);
			res[1] = radius * sin(315 * PI / 180);
		}
		else if (degree == 30) {
			res[0] = radius * sin(60 * PI / 180);
			res[1] -= radius * 0.5;
		}
		else if (degree == 60) {
			res[0] = radius * 0.5;
			res[1] -= radius * sin(60 * PI / 180);
		}
		else if (degree == 120) {
			res[0] -= radius * 0.5;
			res[1] -= radius * sin(60 * PI / 180);
		}
		else if (degree == 150) {
			res[0] -= radius * sin(60 * PI / 180);
			res[1] -= radius * 0.5;
		}
		else if (degree == 210) {
			res[0] -= radius * sin(60 * PI / 180);
			res[1] = radius * 0.5;
		}
		else if (degree == 240) {
			res[0] = -radius * 0.5;
			res[1] = radius * sin(60 * PI / 180);
		}
		else if (degree == 300) {
			res[0] = radius * 0.5;
			res[1] = radius * sin(60 * PI / 180);
		}
		else if (degree == 330) {
			res[0] = radius * sin(60 * PI / 180);
			res[1] = radius * 0.5;
		}
		else {
			res[0] = radius * cos(degree * PI / 180);
			res[1] = radius * sin(degree * PI / 180);
		}

		return res;
	}

	static std::vector<int> linePos(int i, int x1, int x2, int y1, int y2) {
		std::vector<int> res;

		double x = x2 - x1;
		double y = y2 - y1;

		if (x < 0) x *= -1;
		if (y < 0) y *= -1;

		double degree = 360 - atan(y / x) * 180 / PI;

		return radiusPos(degree, i);
	}
};

struct Color {
	short int r = 255;
	short int g = 255;
	short int b = 255;
};

class Rectangle {
private:
	double _w;
	double _l;
public:
	Rectangle(int w = 1, int l = 1) {
		setWidth(w);
		setLength(l);
	}

	double getDWidth() {
		return _w;
	}
	int getWidth() {
		return _w;
	}
	void setWidth(int w = 1) {
		try {
			if (w <= 0) {
				_w = 1;
				ErrorValue err = { -1, w };
				throw err;
			}
			else _w = w;
		}
		catch (ErrorValue err) {
			err.printInfo();
		}
	}
	int getLength() {
		return _l;
	}
	double getDLength() {
		return _l;
	}
	void setLength(int l = 1) {
		try {
			if (l <= 0) {
				_l = 1;
				ErrorValue err = { -2, l };
				throw err;
			}
			else _l = l;
		}
		catch (ErrorValue err) {
			err.printInfo();
		}
	}

	virtual double calcSquare() {
		return _w * _l;
	}
	
	virtual std::vector<int> render(short int y, Color col = {0, 0, 255}, short int mod = 1, bool notback = true) {
		int startPosX = (_w + _l) * mod + 10 + xplus;
		int startPosY = y + yplus;

		std::vector<int> points = { startPosX, startPosY, 0 };

		for (int i = 0; i < _w * mod; i++) {
			rect = { startPosX + i, startPosY + i, 1, 1 };
			if (notback || i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));

			if (i == _w * mod - 1) {
				points.push_back(rect.x);
				points.push_back(rect.y);
				points.push_back(1);
			}

			rect.x -= _l * mod;
			rect.y += _l * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));
		}

		for (int i = _l * mod; i > 0; i--) {
			rect = { startPosX - i, startPosY + i - 1, 1, 1 };
			if (notback || i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));

			if (i == _l * mod) {
				points.push_back(rect.x);
				points.push_back(rect.y);
				points.push_back(1);
			}

			rect.x += _w * mod;
			rect.y += _w * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));

			if (i == _l * mod) {
				points.push_back(rect.x);
				points.push_back(rect.y);
				points.push_back(1);
			}
		}

		SDL_UpdateWindowSurface(window);
		return points;
	}
};

class Circle : public Rectangle {
public:
	Circle(int r = 1) {
		setRadius(r);
	}

	void setRadius(int r = 1) {
		try {
			if (r <= 0) {
				setWidth(1);
				ErrorValue err = { -3, r };
				throw err;
			}
			else setWidth(r);
		}
		catch (ErrorValue err) {
			err.printInfo();
		}
	}

	virtual double calcSquare() {
		return PI * (getWidth() * getWidth());
	}

	virtual std::vector<int> render(short int y, Color col = { 0, 0, 255 }, short int mod = 1, bool notback = true) {
		int startPosX = getWidth() * mod * 2 + 10 + xplus;
		int startPosY = getWidth() * mod + y + yplus;
		
		for (double i = 0; i <= 90; i += 0.1) {
			std::vector<int> vec = Funcs::radiusPos(i, getWidth() * mod);

			rect = { startPosX + vec[0], startPosY + vec[1] / 2, 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));

			int iint = i;

			rect = { startPosX - vec[0], startPosY + vec[1] / 2, 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));

			rect = { startPosX - vec[0], startPosY - vec[1] / 2, 1, 1 };
			if (notback || iint % 5 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));

			rect = { startPosX + vec[0], startPosY - vec[1] / 2, 1, 1 };
			if (notback || iint % 5 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));
		}
		
		SDL_UpdateWindowSurface(window);

		std::vector<int> points = { (int)(startPosX - getWidth() * mod), startPosY, 1, (int)(startPosX + getWidth() * mod), startPosY, 1 };
		return points;
	}
};

class Triangle : public Rectangle {
public:
	Triangle(int w = 1, int l = 1) {
		setWidth(w);
		setLength(l);
	}

	virtual double calcSquare() {
		return (getDWidth() * getDLength()) / 2;
	}

	virtual std::vector<int> render(short int y, Color col = { 0, 0, 255 }, short int mod = 1, bool notback = true) {
		int startPosX = (getWidth() + (getLength() / 5)) * mod + 10 + xplus;
		int startPosY = y + yplus;
		
		std::vector<int> points = { startPosX, startPosY, 0, (int)(startPosX + getWidth() * mod), startPosY, 1, startPosX - getLength() * mod / 5 + 1, startPosY + getLength() * mod - 1, 1 };

		for (int i = 0; i < getWidth() * mod; i++) {
			rect = { startPosX + i, startPosY, 1, 1 };
			if (notback || i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));
		}

		for (int i = 0; i < getLength() * mod; i++) {
			rect = { startPosX - i / 5, startPosY + i, 1, 1 };
			if (notback || i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));
		}

		for (int i = 0; i <= (getWidth() * mod + getLength() * mod / 2); i += 1) {
			std::vector<int> vec = Funcs::linePos(i, startPosX - getLength() * mod / 5, startPosX + getWidth() * mod, startPosY + getLength() * mod, startPosY);

			rect = { startPosX - getLength() * mod / 5 + vec[0], startPosY + getLength() * mod + vec[1], 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, col.r, col.g, col.b));
		}

		SDL_UpdateWindowSurface(window);
		return points;
	}
};

class Shape {
public:
	Rectangle& _shape;
	short int _h;

	Shape(Rectangle rect, short int h = 1):_shape(*new Rectangle) {
		_shape = rect;
		setH(h);
	}
	Shape(Circle circ, short int h = 1) :_shape(*new Circle) {
		_shape = circ;
		setH(h);
	}
	Shape(Triangle tri, short int h = 1) :_shape(*new Triangle) {
		_shape = tri;
		setH(h);
	}

	void setH(int h = 1) {
		try {
			if (h <= 0) {
				_h = 1;
				ErrorValue err = { -3, h };
				throw err;
			}
			else _h = h;
		}
		catch (ErrorValue err) {
			err.printInfo();
		}
	}

	void renderCylinder(short int mod = 1) {
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
		
		int startPosX = mod * 2 + 10 + xplus;
		int startPosY = mod + 10 + yplus;
		
		std::vector<int> points = _shape.render(10, {0, 0, 255}, mod);
		for (int i = 0; i * 3 < points.size(); i++) {
			SDL_Rect temp = { points[i * 3], points[i * 3 + 1] + 1, 1, _h * mod };
			if (points[i * 3 + 2] == 1) {
				SDL_FillRect(screenSurface, &temp, SDL_MapRGB(screenSurface->format, 255, 0, 0));
			}
			else {
				temp.h = 1;
				for (short int j = 0; j < _h * mod; j++) {
					temp.y = points[i * 3 + 1] + j + 1;
					if (j % 2 == 0) SDL_FillRect(screenSurface, &temp, SDL_MapRGB(screenSurface->format, 255, 0, 0));
				}
			}
		}
		_shape.render(10 + _h * mod, { 0, 255, 0 }, mod, false);

		SDL_UpdateWindowSurface(window);
	}
};

template <typename T>
T mySum(T num1, T num2) {
	return num1 + num2;
}

template <typename T>
T myMult(T num1, T num2) {
	return num1 * num2;
}

template <typename T>
T myDiv(T num1, T num2) {
	return num1 / num2;
}

template <typename T, typename U, typename I>
class myClass {
private:
	T value1;
	U value2;
	I value3;
public:
	myClass(T _val1, U _val2, I _val3) {
		value1 = _val1;
		value2 = _val2;
		value3 = _val3;
	}

	T sum() {
		return value1 + value2 + value3;
	}
	T mult() {
		return value1 * value2 * value3;
	}
	T div() {
		return value1 / value2 / value3;
	}
};

int main(int argc, char* args[]) {
	setlocale(LC_ALL, "rus");
	short int _lastRender = 0;
	bool _shifting = false;
	short int globalMod = 10;

	std::cout << mySum<int>(10, 15) << std::endl << mySum<double>(5.5, 6.7) << "\n\n" << myMult<int>(2, 10) << std::endl << myMult<double>(10.0, 0.5) << "\n\n" << myDiv<int>(5, 2) << std::endl << myDiv<double>(10.0, 3.0) << "\n\n";
	
	myClass<int, double, float> clss1 = { 10, 2.5, 0.5 };
	myClass<double, int, bool> clss2 = {5.5, 5, 1};
	std::cout << clss1.sum() << std::endl << clss1.mult() << std::endl << clss1.div() << "\n\n" << clss2.sum() << std::endl << clss2.mult() << std::endl << clss2.div() << "\n\n";

	Rectangle _par = { -6, 5 };

	Circle _circ = { -5 };

	Triangle _tri = { 5, -4 };
	
	Shape _buf1 = { _par, 8 };
	Shape _buf2 = { _circ, -8 };
	Shape _buf3 = { _tri, 8 };

	bool quit = false;
	SDL_Event sEvent;
	SDL_Rect srect;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) std::cout << "SDL could not initialize! SDL_ErrorValue: " << SDL_GetError() << std::endl;
	else {
		window = SDL_CreateWindow("Lab3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) std::cout << "Window could not be created! SDL_ErrorValue: " << SDL_GetError() << std::endl;
		else {
			screenSurface = SDL_GetWindowSurface(window);

			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
			SDL_UpdateWindowSurface(window);

			while (!quit) {
				while (SDL_PollEvent(&sEvent) != 0) {
					if (sEvent.type == SDL_QUIT) {
						quit = true;
					}
					else if (sEvent.type == SDL_KEYDOWN && !_keyLocked) {
						_keyPressed = SDL_GetKeyboardState(NULL);

						if (_keyPressed[SDL_SCANCODE_1]) {
							SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
							_par.render(10, {0, 128, 255}, globalMod);
							_keyLocked = true;
							_lastRender = 1;
						}
						else if (_keyPressed[SDL_SCANCODE_2]) {
							SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
							_circ.render(10, { 0, 255, 128 }, globalMod);
							_keyLocked = true;
							_lastRender = 2;
						}
						else if (_keyPressed[SDL_SCANCODE_3]) {
							SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
							_tri.render(10, { 0, 255, 255 }, globalMod);
							_keyLocked = true;
							_lastRender = 3;
						}
						else if (_keyPressed[SDL_SCANCODE_4]) {
							_buf1.renderCylinder(globalMod);
							_keyLocked = true;
							_lastRender = 4;
						}
						else if (_keyPressed[SDL_SCANCODE_5]) {
							_buf2.renderCylinder(globalMod);
							_keyLocked = true;
							_lastRender = 5;
						}
						else if (_keyPressed[SDL_SCANCODE_6]) {
							_buf3.renderCylinder(globalMod);
							_keyLocked = true;
							_lastRender = 6;
						}
						else if (_keyPressed[SDL_SCANCODE_LSHIFT]) {
							_shifting = !_shifting;
							_keyLocked = true;
						}
						else if (_keyPressed[SDL_SCANCODE_UP]) {
							if (_shifting) yplus -= 5;
							else yplus--;

							try {
								if (yplus <= -15) {
									yplus = -14;
									ErrorBorder err = { };
									throw err;
								}
							}
							catch (ErrorBorder err) {
								err.printInfo();
							}

							SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
							if (_lastRender == 1) _par.render(10, { 0, 128, 255 }, globalMod);
							else if (_lastRender == 2) _circ.render(10, { 0, 255, 128 }, globalMod);
							else if (_lastRender == 3) _tri.render(10, { 0, 255, 255 }, globalMod);
							else if (_lastRender == 4) _buf1.renderCylinder(globalMod);
							else if (_lastRender == 5) _buf2.renderCylinder(globalMod);
							else if (_lastRender == 6) _buf3.renderCylinder(globalMod);
						}
						else if (_keyPressed[SDL_SCANCODE_DOWN]) {
							if (_shifting) yplus += 5;
							else yplus++;

							try {
								if (yplus >= 465) {
									yplus = 464;
									ErrorBorder err = { "down" };
									throw err;
								}
							}
							catch (ErrorBorder err) {
								err.printInfo();
							}

							SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
							if (_lastRender == 1) _par.render(10, { 0, 128, 255 }, globalMod);
							else if (_lastRender == 2) _circ.render(10, { 0, 255, 128 }, globalMod);
							else if (_lastRender == 3) _tri.render(10, { 0, 255, 255 }, globalMod);
							else if (_lastRender == 4) _buf1.renderCylinder(globalMod);
							else if (_lastRender == 5) _buf2.renderCylinder(globalMod);
							else if (_lastRender == 6) _buf3.renderCylinder(globalMod);
						}
						else if (_keyPressed[SDL_SCANCODE_LEFT]) {
							if (_shifting) xplus -= 5;
							else xplus--;

							try {
								if (xplus <= -15) {
									xplus = -14;
									ErrorBorder err = { "left" };
									throw err;
								}
							}
							catch (ErrorBorder err) {
								err.printInfo();
							}

							SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
							if (_lastRender == 1) _par.render(10, { 0, 128, 255 }, globalMod);
							else if (_lastRender == 2) _circ.render(10, { 0, 255, 128 }, globalMod);
							else if (_lastRender == 3) _tri.render(10, { 0, 255, 255 }, globalMod);
							else if (_lastRender == 4) _buf1.renderCylinder(globalMod);
							else if (_lastRender == 5) _buf2.renderCylinder(globalMod);
							else if (_lastRender == 6) _buf3.renderCylinder(globalMod);
						}
						else if (_keyPressed[SDL_SCANCODE_RIGHT]) {
							if (_shifting) xplus += 5;
							else xplus++;

							try {
								if (xplus >= 625) {
									xplus = 624;
									ErrorBorder err = { "right" };
									throw err;
								}
							}
							catch (ErrorBorder err) {
								err.printInfo();
							}

							SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
							if (_lastRender == 1) _par.render(10, { 0, 128, 255 }, globalMod);
							else if (_lastRender == 2) _circ.render(10, { 0, 255, 128 }, globalMod);
							else if (_lastRender == 3) _tri.render(10, { 0, 255, 255 }, globalMod);
							else if (_lastRender == 4) _buf1.renderCylinder(globalMod);
							else if (_lastRender == 5) _buf2.renderCylinder(globalMod);
							else if (_lastRender == 6) _buf3.renderCylinder(globalMod);
						}
						else {
							try {
								_keyLocked = true;
								ErrorButton err;
								throw err;
							}
							catch (ErrorButton err) {
								err.printInfo();
							}
						}
					}
					else if (sEvent.type == SDL_KEYUP) _keyLocked = false;
					else if (sEvent.type == SDL_MOUSEWHEEL) {
						if (sEvent.wheel.y > 0) {
							if (_shifting) globalMod += 5;
							else globalMod++;
						}
						else {
							if (_shifting) globalMod -= 5;
							else globalMod--;
						}

						try {
							if (globalMod <= 0) {
								globalMod = 1;
								ErrorZoom err;
								throw err;
							}
						}
						catch (ErrorZoom err) {
							err.printInfo();
						}

						SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
						if (_lastRender == 1) _par.render(10, { 0, 128, 255 }, globalMod);
						else if (_lastRender == 2) _circ.render(10, { 0, 255, 128 }, globalMod);
						else if (_lastRender == 3) _tri.render(10, { 0, 255, 255 }, globalMod);
						else if (_lastRender == 4) _buf1.renderCylinder(globalMod);
						else if (_lastRender == 5) _buf2.renderCylinder(globalMod);
						else if (_lastRender == 6) _buf3.renderCylinder(globalMod);
					}
					else {
						
					}
				}
			}
		}
	}

	SDL_FreeSurface(screenSurface);
	screenSurface = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	return 0;
}