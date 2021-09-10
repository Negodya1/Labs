//#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include <chrono>

unsigned short int charToNum(char str);

struct datetime {
	unsigned short int second;
	unsigned short int minute;
	unsigned short int hour;
	unsigned short int day;
	unsigned short int month;
	unsigned short int year;

	void printInfo();
	void sysTime();
	/*void printNext();
	void printPrev();*/
};

void datetime::printInfo() {
	if (day < 10) std::cout << 0;
	std::cout << day << '.';
	if (month < 10) std::cout << 0;
	std::cout << month << '.' << year << ' ';

	if (hour < 10) std::cout << 0;
	std::cout << hour << ':';
	if (minute < 10) std::cout << 0;
	std::cout << minute << '.';
	if (second < 10) std::cout << 0;
	std::cout << second;
}

void datetime::sysTime() {
	auto const time = std::chrono::system_clock::now();
	std::time_t seconds = std::chrono::system_clock::to_time_t(time);

	std::stringstream ss;
	ss << std::ctime(&seconds);
	std::string sysDate = ss.str();

	day = 0;
	year = 0;
	month = 0;
	hour = 0;
	minute = 0;
	second = 0;

	if (charToNum(sysDate[8]) != 404) day = charToNum(sysDate[8]);
	else day = 0;
	day = day * 10 + charToNum(sysDate[9]);

	if (sysDate[4] == 'J') {
		if (sysDate[5] == 'a') month = 1;
		else {
			if (sysDate[6] == 'n') month = 6;
			else month = 7;
		}
	}
	else if (sysDate[4] == 'F') month = 2;
	else if (sysDate[4] == 'M') {
		if (sysDate[6] == 'r') month = 3;
		else month = 5;
	}
	else if (sysDate[4] == 'A') {
		if (sysDate[5] == 'p') month = 4;
		else month = 8;
	}
	else if (sysDate[4] == 'S') month = 9;
	else if (sysDate[4] == 'O') month = 10;
	else if (sysDate[4] == 'N') month = 11;
	else if (sysDate[4] == 'D') month = 12;

	year = charToNum(sysDate[20]) * 10;
	year = (year + charToNum(sysDate[21])) * 10;
	year = (year + charToNum(sysDate[22])) * 10;
	year += charToNum(sysDate[23]);

	hour = charToNum(sysDate[11]) * 10;
	hour += charToNum(sysDate[12]);

	minute = charToNum(sysDate[14]) * 10;
	minute += charToNum(sysDate[15]);

	second = charToNum(sysDate[17]) * 10;
	second += charToNum(sysDate[18]);
}
/*void datetime::printNext() {
	datetime temp;
	temp.day = day;
	temp.month = month;
	temp.year = year;

	unsigned short int monthSize = 31;
	if (month == 2) monthSize = 28;
	else if (month == 4 || month == 6 || month == 9 || month == 11) monthSize = 30;

	if (day == monthSize) {
		temp.day = 1;
		temp.month++;
	}
	else temp.day++;

	if (temp.month == 13) {
		temp.month = 1;
		temp.year++;
	}

	if (temp.day < 10) std::cout << 0;
	std::cout << temp.day << '.';
	if (temp.month < 10) std::cout << 0;
	std::cout << temp.month << '.' << temp.year << " - ";
}
void datetime::printPrev() {
	datetime temp;
	temp.day = day;
	temp.month = month;
	temp.year = year;

	unsigned short int monthSize = 31;
	if (month == 3) monthSize = 28;
	else if (month == 5 || month == 7 || month == 10 || month == 12) monthSize = 30;

	if (day == 1) {
		temp.day = monthSize;
		temp.month--;
	}
	else temp.day--;

	if (temp.month == 0) {
		temp.month = 12;
		temp.year--;
	}

	if (temp.day < 10) std::cout << 0;
	std::cout << temp.day << '.';
	if (temp.month < 10) std::cout << 0;
	std::cout << temp.month << '.' << temp.year << std::endl;
}*/

struct timedelta {
	short int second;
	short int minute;
	short int hour;
	short int day;
	short int month;
	short int year;

	void minus(datetime t1, datetime t2);
	datetime timePlus(datetime t);
	void plusDelta(timedelta d);
};

void timedelta::minus(datetime t1, datetime t2) {
	second = t1.second - t2.second;
	minute = t1.minute - t2.minute;
	hour = t1.hour - t2.hour;
	day = t1.day - t2.day;
	month = t1.month - t2.month;
	year = t1.year - t2.year;
}

datetime timedelta::timePlus(datetime t) {
	t.second += second;
	while (t.second > 59 || t.second < -59) {
		if (t.second > 59) {
			t.second -= 60;
			t.minute++;
		}
		else {
			t.second += 60;
			t.minute--;
		}
	}

	t.minute += minute;
	while (t.minute > 59 || t.minute < -59) {
		if (t.minute > 59) {
			t.minute -= 60;
			t.hour++;
		}
		else {
			t.minute += 60;
			t.hour--;
		}
	}

	t.hour += hour;
	while (t.hour > 23 || t.hour < -23) {
		if (t.hour > 23) {
			t.hour -= 24;
			t.day++;
		}
		else {
			t.hour += 24;
			t.day--;
		}
	}

	unsigned short int monthSize = 31;
	if (t.month == 2) {
		if (t.year % 4 == 0) monthSize = 29;
		else monthSize = 28;
	}
	else if (t.month == 4 || t.month == 6 || t.month == 9 || t.month == 11) monthSize = 30;

	t.day += day;
	while (t.day > monthSize) {
		t.month++;
		t.day -= monthSize;

		if (t.month > 12) {
			t.month = 1;
			t.year++;
		}

		if (t.month == 2) {
			if (t.year % 4 == 0) monthSize = 29;
			else monthSize = 28;
		}
		else if (t.month == 4 || t.month == 6 || t.month == 9 || t.month == 11) monthSize = 30;
	}

	t.month += month;
	while (t.month > 12) {
		t.month -= 12;
		t.year++;
	}

	t.year += year;

	return t;
}

void timedelta::plusDelta(timedelta d) {
	second += d.second;
	minute += d.minute;
	hour += d.hour;
	day += d.day;
	month += d.month;
	year += d.year;
}

unsigned short int charToNum(char str) {
	if (str == '0') return 0;
	else if (str == '1') return 1;
	else if (str == '2') return 2;
	else if (str == '3') return 3;
	else if (str == '4') return 4;
	else if (str == '5') return 5;
	else if (str == '6') return 6;
	else if (str == '7') return 7;
	else if (str == '8') return 8;
	else if (str == '9') return 9;

	return 404;
}

int main() {
	std::vector<datetime> arr;
	datetime date;
	timedelta delta;
	timedelta delta2;

	std::ifstream f("input.txt");
	if (f) {
		while (!f.eof()) {
			std::string temp;
			f >> temp;

			date.day = charToNum(temp[0]);
			date.day *= 10;
			date.day += charToNum(temp[1]);

			date.month = charToNum(temp[3]);
			date.month *= 10;
			date.month += charToNum(temp[4]);

			date.year = charToNum(temp[6]);
			date.year *= 10;
			date.year += charToNum(temp[7]);
			date.year *= 10;
			date.year += charToNum(temp[8]);
			date.year *= 10;
			date.year += charToNum(temp[9]);

			f >> temp;

			date.hour = charToNum(temp[0]);
			date.hour *= 10;
			date.hour += charToNum(temp[1]);

			date.minute = charToNum(temp[3]);
			date.minute *= 10;
			date.minute += charToNum(temp[4]);

			date.second = charToNum(temp[6]);
			date.second *= 10;
			date.second += charToNum(temp[7]);

			arr.push_back(date);
		}
	}
	f.close();

	if (!arr.empty()) {
		for (int i = 0; i < arr.size(); i++) {
			std::cout << i + 1 << ". ";

			arr[i].printInfo();
			if (i != arr.size() - 1) {
				std::cout << " - ";
				arr[i + 1].printInfo();
			}
			if (i != 0) {
				std::cout << " - ";
				arr[i - 1].printInfo();
			}
			std::cout << std::endl;
		}

		std::cout << "\n\n\n";

		for (int i = 0; i < arr.size(); i++) {
			if (i < arr.size() - 1 && arr[i].day % 2 == 1) {
				if (arr[i + 1].day % 2 == 1) {
					std::cout << i + 1 << ". ";

					arr[i].printInfo();
					if (i != arr.size() - 1) {
						std::cout << " - ";
						arr[i + 1].printInfo();
					}
					if (i != 0) {
						std::cout << " - ";
						arr[i - 1].printInfo();
					}
					std::cout << std::endl;
				}
			}
		}

		std::cout << "\n\n\n";

		date.sysTime();
		date.printInfo();

		std::cout << "\n\n\n";

		arr[arr.size() - 1].printInfo();
		std::cout << std::endl;
		arr[arr.size() - 2].printInfo();
		std::cout << "\n\n\n";

		delta.minus(arr[arr.size() - 1], arr[arr.size() - 2]);

		date = delta.timePlus(arr[arr.size() - 1]);
		date.printInfo();
		std::cout << "\n\n\n";

		delta2.minus(arr[arr.size() - 1], arr[arr.size() - 2]);
		delta.plusDelta(delta2);

		date = delta.timePlus(arr[arr.size() - 1]);
		date.printInfo();
		std::cout << std::endl;
	}

	return 0;
}
//