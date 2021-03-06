#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdlib>
using namespace std;

int GetInt(int);
float *NewMass(int);
void GetMass(int, float *arr, short int);
void OutputMass(int, float *arr);
void NewElement(int, float, int &, float *&arr);
void DeleteElement(int, int &, float *&arr);
int ElementPos(int, int, float *arr);
int GetFileSize(string);

void MasGenerator(int MassiveSize);

int main()
{
	setlocale(LC_ALL, "rus");

	{
		int count;

		srand(time(0));
		count = rand() % 100;


		ifstream p("Input3.txt");
		if (!p) {
			cout << "\nВходной файл не обнаружен, генерирую новый...";
			MasGenerator(count);
			cout << " ...входной файл сгенерирован\n";
		}
		else {
			bool y;
			cout << "\nВходной файл обнаружен, сгенерировать новый? ";
			cin >> y;
			if (y == 1) MasGenerator(count);
		}
		p.close();
	}

	//cout << GetFileSize("Input3.txt");

	int count;
	float element;
	short int mode;
	int pos;
	bool elementmode;
	cout << "Выберите режим заполнения (1-3): ";
	do {
		cin >> mode;
		if (mode < 1 || mode > 3) cout << "\nНеправильный режим: ";
	} while (mode < 1 || mode > 3);
	if (mode != 3) {
		cout << "Введите кол-во элементов в массиве: ";
		cin >> count;
	}
	else count = GetFileSize("Input3.txt");

	float *arr = NewMass(count);

	GetMass(count, arr, mode);
	cout << "\nМассив: ";
	OutputMass(count, arr);

	cout << "\nВыберите режим элемента (0 - с клавиатуры, 1 - случайные): ";
	cin >> elementmode;
	unsigned short int act;
	do {
		cout << "\n1. Добавить элемент в конец массива\n2. Добавить элемент в начало массива\n3. Добавить элемент на k-ую позицию в массиве массива\n4. Удалить последний элемент массива\n5. Удалить первый элемент массива\n6. Удалить k-ый элемент массива\n7. Добавить элемент массива после первого элемента с заданным значением\n8. Удалить все элементы с заданным значением\n0. Выход\nВыберите действие:";
		do {
			cin >> act;
			if (act > 8) cout << "\nНеправильный номер действия: ";
		} while (act > 8);


		if (act == 1) {
			pos = count + 1;

			if (elementmode == 0) {
				cout << "\nВведите элемент: ";
				cin >> element;
			}
			else element = rand() % 100;

			NewElement(pos, element, count, arr);
		}

		if (act == 2) {
			pos = 1;

			if (elementmode == 0) {
				cout << "\nВведите элемент: ";
				cin >> element;
			}
			else element = rand() % 100;

			NewElement(pos, element, count, arr);
		}

		if (act == 3) {
			pos = GetInt(count);
			pos++;

			if (pos != 0) {
				if (elementmode == 0) {
					cout << "\nВведите элемент: ";
					cin >> element;
				}
				else element = rand() % 100;

				NewElement(pos, element, count, arr);
			}
		}

		if (act == 4) {
			pos = count;

			DeleteElement(pos, count, arr);
		}

		if (act == 5) {
			pos = 1;

			DeleteElement(pos, count, arr);
		}

		if (act == 6) {
			pos = GetInt(count);
			pos++;

			if (pos != 0) {
				DeleteElement(pos, count, arr);
			}
		}

		if (act == 7) {
			cout << "\nВведите элемент, после которого добавить новый: ";
			cin >> element;

			pos = ElementPos(element, count, arr) + 1;
			if (pos == -1) cout << "\nТакого элемента нет";
			else {
				if (elementmode == 0) {
					cout << "\nВведите элемент: ";
					cin >> element;
				}
				else element = rand() % 100;

				NewElement(pos, element, count, arr);
			}
		}

		if (act == 8) {
			int delement;
			cout << "\nВведите значение элементов, которые будут удалены: ";
			cin >> delement;
			pos = ElementPos(delement, count, arr);

			if (pos == -1) cout << "\nНи одного такого элемента нет";
			else {
				while (pos != -1) {
					DeleteElement(pos, count, arr);
					pos = ElementPos(delement, count, arr);
				}
			}
		}

		cout << "\nМассив: ";
		OutputMass(count, arr);
	} while (act != 0);

	delete[] arr;
}

int GetInt(int count) {
	int pos;

	cout << "\nВведите номер элемента или -1 для выхода: ";
	do {
		cin >> pos;
		if (pos < -1 || pos > count) cout << "Число слишком большое или меньше -1: ";
	} while (pos < -1 || pos > count);

	return pos;
}

float *NewMass(int count) {
	float *arr = new float[count];

	return arr;
}
int *NewIntMass(int count) {
	int *arr = new int[count];

	return arr;
}

int GetFileSize(string name) {
	int temp, counter = 0;
	ifstream f("Input3.txt");
	while (f >> temp) {
		counter++;
	}
	f.close();

	return counter;
}

void GetMass(int count, float *arr, short int mode) {
	if (mode == 1) {
		for (unsigned int i = 0; i < count; i++) {
			cin >> arr[i];
		}
	}
	else if (mode == 2) {
		for (unsigned int i = 0; i < count; i++) {
			arr[i] = i * 2;
		}
	}
	else if (mode == 3) {
		ifstream f("Input3.txt");

		for (unsigned i = 0; i < count; i++) {
			f >> arr[i];
		}

		f.close();
	}
}

void OutputMass(int count, float *arr) {
	for (unsigned int i = 0; i < count; i++) {
		cout << endl << arr[i];
	}
	cout << endl;
}

void NewElement(int pos, float element, int &count, float *&arr) {
	float *buf = NewMass(count + 1);
	bool flag = 0;

	for (int i = 0; i <= count; i++) {
		if (i + 1 != pos && flag == 0) {
			buf[i] = arr[i];
		}
		else if (i + 1 == pos && flag == 0) {
			buf[i] = element;
			i--;
			flag = 1;
		}
		else {
			buf[i + 1] = arr[i];
		}
	}

	delete[] arr;
	arr = buf;
	count++;
}

void DeleteElement(int pos, int &count, float *&arr) {
	float *buf = NewMass(count - 1);
	bool flag = 0;

	for (int i = 0; i <= count; i++) {
		if (i + 1 != pos && flag == 0) {
			buf[i] = arr[i];
		}
		else if (i + 1 == pos && flag == 0) {
			flag = 1;
		}
		else {
			buf[i - 1] = arr[i];
		}
	}

	delete[] arr;
	arr = buf;
	count--;
}

int ElementPos(int element, int count, float *arr) {
	for (unsigned int i = 0; i < count; i++) {
		if (arr[i] == element) return i + 1;
	}

	return -1;
}

void RandMas(int v[], int n, int A, int B) {
	srand(time(0));
	for (unsigned int i = 0; i < n; i++) {
		v[i] = rand() % (B - A + 1) + A;
	}
}

void MasGenerator(int MassiveSize) {
	int *v = NewIntMass(MassiveSize);

	//Случайный массив
	RandMas(v, MassiveSize, 1, MassiveSize / 2);

	ofstream p("Input3.txt");
	for (unsigned i = 0; i < MassiveSize; i++) {
		p << v[i] << ' ';
		if (i % 17 == 0 && i != 0) p << endl;
	}
	p.close();

	delete[] v;
}