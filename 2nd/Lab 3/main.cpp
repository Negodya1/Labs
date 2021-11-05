#include <iostream>
#include <SDL.h>
#include <vector>
#include <cmath>

const short int SCREEN_WIDTH = 640;
const short int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Rect rect;

const Uint8* _keyPressed = SDL_GetKeyboardState(NULL);
bool _keyLocked = false;

#define PI 3.14159265

class Funcs {
public:
	static std::vector<int> radiusPos(double degree, int radius) {
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

class Parall {
private:
	unsigned int _w;
	unsigned int _l;
	unsigned int _h;
public:
	Parall(unsigned int w = 1, unsigned int l = 1, unsigned int h = 1) {
		_w = w;
		_h = h;
		_l = l;
	}

	int getHeight() {
		return _h;
	}
	void setHeight(unsigned int h = 1) {
		_h = h;
	}

	int getWidth() {
		return _w;
	}
	void setWidth(unsigned int w = 1) {
		_w = w;
	}
	int getLength() {
		return _l;
	}
	void setLength(unsigned int l = 1) {
		_l = l;
	}

	virtual double calcVolume() {
		return _w * _h * _l;
	}
	
	virtual void render() {
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));

		short int mod = 1;
		if (_w + _h + _l < 10) mod = 100;
		else if (_w + _h + _l < 100) mod = 10;
		else if (_w + _h + _l < 200) mod = 2;

		int startPosX = (_w + _l) * mod + 10;
		int startPosY = 10;

		for (int i = 0; i < _w * mod; i++) {
			rect = { startPosX + i, startPosY + i, 1, 1 };
			if (i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			rect.x -= _l * mod;
			rect.y += _l * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			rect.y += (_h - _l) * mod;
			rect.x += _l * mod;
			if (i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			rect.y += _l * mod;
			rect.x -= _l * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));
		}

		for (int i = _l * mod; i > 0; i--) {
			rect = { startPosX - i, startPosY + i - 1, 1, 1 };
			if (i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));

			rect.x += _w * mod;
			rect.y += _w * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));

			rect.y += (_h - _w) * mod;
			rect.x -= _w * mod;
			if (i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));

			rect.y += _w * mod;
			rect.x += _w * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));
		}

		for (int i = 0; i < _h * mod; i++) {
			rect = { startPosX, startPosY + i, 1, 1 };
			if (i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));

			rect.x += _w * mod;
			rect.y += _w * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));

			rect.y -= _w * mod;
			rect.y += _l * mod;
			rect.x -= (_w + _l) * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));

			rect.y += _w * mod;
			rect.x += _w * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));
		}

		SDL_UpdateWindowSurface(window);
	}
};

class Circle : public Parall {
private:
	unsigned int _r;
public:
	Circle(unsigned int r = 1, unsigned int h = 1) {
		_r = r;
		setHeight(h);
	}

	virtual double calcVolume() {
		return (PI * getHeight() * (_r * _r));
	}

	virtual void render() {
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));

		short int mod = 1;
		if (_r + getHeight() < 8) mod = 100;
		else if (_r + getHeight() < 80) mod = 10;
		else if (_r + getHeight() < 160) mod = 2;

		int startPosX = _r * mod * 2 + 10;
		int startPosY = _r * mod + 10;

		for (double i = 0; i <= 90; i += 0.1) {
			std::vector<int> vec = Funcs::radiusPos(i, _r * mod);

			rect = { startPosX + vec[0], startPosY + vec[1] / 2, 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			int ri = _r;
			int iint = i;
			
			rect.y += getHeight() * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));

			rect = { startPosX - vec[0], startPosY + vec[1] / 2, 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			rect.y += getHeight() * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));

			rect = { startPosX - vec[0], startPosY - vec[1] / 2, 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			if (iint % 5 == 0) {
				rect.y += getHeight() * mod;
				SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));
			}

			rect = { startPosX + vec[0], startPosY - vec[1] / 2, 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			if (iint % 5 == 0) {
				rect.y += getHeight() * mod;
				SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));
			}
		}

		for (int i = 0; i < getHeight() * mod; i++) {
			int ri = _r;
			rect = { startPosX + (ri * mod), startPosY + i, 1, 1 };

			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));

			rect.x -= 2 * _r * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));
		}

		SDL_UpdateWindowSurface(window);
	}
};

class Triangle : public Parall {
public:
	Triangle(unsigned int w = 1, unsigned int l = 1, unsigned int h = 1) {
		setHeight(h);
		setWidth(w);
		setLength(l);
	}

	virtual double calcVolume() {
		return ((getWidth() * getLength()) / 2) * getHeight();
	}

	virtual void render() {
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));

		short int mod = 1;
		if (getWidth() + getHeight() + getLength() < 10) mod = 100;
		else if (getWidth() + getHeight() + getLength() < 100) mod = 10;
		else if (getWidth() + getHeight() + getLength() < 200) mod = 2;

		int startPosX = (getWidth() + (getLength() / 5)) * mod + 10;
		int startPosY = 10;

		for (int i = 0; i < getHeight() * mod; i++) {
			rect = { startPosX, startPosY + i + 1, 1, 1 };
			if (i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));

			rect.x += getWidth() * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));

			rect.x -= (getWidth() * mod + getLength() * mod / 5) - 1;
			rect.y += getLength() * mod - 1;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 0, 0));
		}

		for (int i = 0; i < getWidth() * mod; i++) {
			rect = { startPosX + i, startPosY, 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			rect.y += getHeight() * mod;
			if (i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));
		}

		for (int i = 0; i < getLength() * mod; i++) {
			rect = { startPosX - i / 5, startPosY + i, 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			rect.y += getHeight() * mod;
			if (i % 2 == 0) SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));
		}

		for (int i = 0; i <= (getWidth() * mod + getLength() * mod / 2); i += 1) {
			std::vector<int> vec = Funcs::linePos(i, startPosX - getLength() * mod / 5, startPosX + getWidth() * mod, startPosY + getLength() * mod, startPosY);

			rect = { startPosX - getLength() * mod / 5 + vec[0], startPosY + getLength() * mod + vec[1], 1, 1 };
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 255));

			rect.y += getHeight() * mod;
			SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 255, 0));
		}

		SDL_UpdateWindowSurface(window);
	}
};

int main(int argc, char* args[]) {
	setlocale(LC_ALL, "rus");

	Parall _par = { 4, 6, 5 };
	Parall& _buf = _par;

	Circle _circ = { 7, 5 };
	Parall& _buf2 = _circ;

	Triangle _tri = { 6, 6, 6 };
	Parall& _buf3 = _tri;

	std::vector<int> test = Funcs::radiusPos(135, 10);
	std::cout << test[0] << ' ' << test[1] << std::endl;

	bool quit = false;
	SDL_Event sEvent;
	SDL_Rect srect;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
	else {
		window = SDL_CreateWindow("Lab3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		else {
			screenSurface = SDL_GetWindowSurface(window);

			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
			SDL_UpdateWindowSurface(window);

			while (!quit) {
				while (SDL_PollEvent(&sEvent) != 0) {
					if (sEvent.type == SDL_QUIT) {
						quit = true;
					}
					else if (sEvent.type == SDL_KEYDOWN) {
						_keyPressed = SDL_GetKeyboardState(NULL);
					}
					else _keyLocked = false;
				}

				if (_keyPressed != 0 && !_keyLocked) {
					if (_keyPressed[SDL_SCANCODE_1]) {
						_buf.render();

						std::cout << "Площадь параллелограмма = " << _buf.calcVolume() << "\n";
						std::cout << _par.calcVolume() << "\n\n";
						_keyLocked = true;
					}
					else if (_keyPressed[SDL_SCANCODE_2]) {
						_buf2.render();

						std::cout << "Площадь цилиндра = " << _buf2.calcVolume() << "\n";
						std::cout << _circ.calcVolume() << "\n\n";
						_keyLocked = true;
					}
					else if (_keyPressed[SDL_SCANCODE_3]) {
						_buf3.render();

						std::cout << "Площадь прямоугольника = " << _buf3.calcVolume() << "\n";
						std::cout << _tri.calcVolume() << "\n\n";
						_keyLocked = true;
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