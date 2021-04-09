//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "windows.h"

using namespace std;

struct Flash {
	string color, type, brand, volumetype;
	float volume;
	void Out();
};

struct MyStack {
	struct Node
	{
		Flash data;
		Node* prev;
	};
	Node* Top = NULL;
	int Count;
	bool Push(Flash);
	bool Pop(Flash&);
	void Info();
};

bool MyStack::Pop(Flash& data) {
	if (!Top) return false;
	Node* temp = Top->prev;
	data = Top->data; delete Top;
	Top = temp; Count--;
	return true;
}

bool MyStack::Push(Flash data) {
	if (!Top)
	{
		Top = new Node;
		Top->prev = NULL;
		Count = 1;
	}
	else
	{
		Node* temp;
		temp = new Node;
		temp->prev = Top;
		Top = temp;
		Count++;
	}
	Top->data = data;//!!!Узкое место
	return true;
}

void MyStack::Info() {
	if (!Top)
		cout << "--->Cтек пуст\n" << endl;
	else
	{
		cout << endl << "Информация:" << endl;
		cout << "\tРазмер стека = " << Count << "\n\n";
		Top->data.Out();
		cout << endl;
		//cout << "\tTop data =" << Top->data << endl << endl;
	}
}

void Flash::Out() {
	cout << "\nБренд: " << brand << "\nОбъём памяти: " << volume << volumetype << "\nЦвет: " << color << "\nТип: " << type << endl;
};

void Push(MyStack&);
bool GetFile(MyStack&);

int main() {
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	Flash k, f;
	int task, i = 0, n, n1;

	MyStack S, I;
	do {
		cout << "\nВыберите действие\n1. Добавить товар в корзину\n2. Вытащить товар из корзины\n3. Очистить корзину\n4. Взять элементы из файла\n5. Содержимое корзины\n0. Выход: ";
		cin >> task;

		if (task == 1) {
			Push(S);
		}
		else if (task == 2) {
			cout << "Выбор режима (1 - одна характеристика, 2 - все характеристики): ";
			cin >> n;

			if (n == 1) {
				n = 0;
				cout << "\nВыберите характеристику\n1. Бренд\n2. Объём памяти\n3. Цвет\n4. Тип: ";
				cin >> n1;

				if (n1 == 1){
					cout << "\nБренд: ";
					cin >> f.brand;
					while (S.Pop(k)){
						I.Push(k);
						if (k.brand == f.brand && n != 0){
							I.Pop(k);
							n = 0;
						}
					}
					while (I.Pop(k)) {
						S.Push(k);
					}
				}
				else if (n1 == 2){
					cout << "\nОбъём памяти: ";
					cin >> f.volume;
					cout << "\nЕдиница измерения памяти: ";
					cin >> f.volumetype;
					while (S.Pop(k)){
						I.Push(k);
						if (k.volume == f.volume && k.volumetype == f.volumetype && n != 0){
							I.Pop(k);
							n = 0;
						}
					}
					while (I.Pop(k)){
						S.Push(k);
					}
				}
				else if (n1 == 3) {
					cout << "\nЦвет: ";
					cin >> f.color;
					while (S.Pop(k)){
						I.Push(k);
						if (k.color == f.color && n != 0){
							I.Pop(k);
							n = 0;
						}
					}
					while (I.Pop(k)) {
						S.Push(k);
					}
				}
				else if (n1 == 4){
					cout << "\nТип: "; cin >> f.type;
					while (S.Pop(k)) {
						I.Push(k);
						if (k.type == f.type && n != 0) {
							I.Pop(k);
							n = 0;
						}
					}
					while (I.Pop(k)) {
						S.Push(k);
					}
				}
				else
					if (n != 0) {
						cout << "\nНет подходящей флешки(\n";
					}
				}
				else if (n == 2) {
					cout << "\nБренд: ";
					cin >> f.brand;
					cout << "\nОбъём памяти: ";
					cin >> f.volume;
					cout << "\nЕдиница измерения: ";
					cin >> f.volumetype;
					cout << "\nЦвет: ";
					cin >> f.color;
					cout << "\nТип: ";
					cin >> f.type;
					while (S.Pop(k)) {
						I.Push(k);
						if (k.brand == f.brand && k.volume == f.volume && k.volumetype == f.volumetype && k.color == f.color && k.type == f.type && n != 0) {
							I.Pop(k);
							n = 0;
						}
					}
					while (I.Pop(k)) {
						S.Push(k);
					}
					if (n != 0) {
						cout << "\nНет подходящей флешки(\n";
					}
				}
				else {
					cout << "\nОшибка в характеристике\n";
				}
			}
			else if (task == 3) {
				i = 0;
				while (S.Pop(k));
				S.Info();
			}
			else if (task == 4 && i == 0) {
				i = 1;
				GetFile(S);
				S.Info();
			}
			else if (task == 4 && i == 1) {
				cout << "\nФайл уже изъят\n";
			}
			else if (task == 5) {
				cout << "\n___________________________\n";
				while (S.Pop(k)) {
					I.Push(k);
					k.Out();
				}
				while (I.Pop(k)) {
					S.Push(k);
				}
				//cout << " " << k;
				cout << endl;
				cout << "\n___________________________\n";
				cout << endl;
			}
		} while (task != 0 || task > 5);
	cout << "\n___________________________\n";
	while (S.Pop(k)) {
		I.Push(k);
		k.Out();
	}
	while (I.Pop(k)) {
		S.Push(k);
	}
	//cout << " " << k;
	cout << endl;
	cout << "\n___________________________\n";
	cout << endl;
}

bool GetFile(MyStack& Stack) {
	ifstream F("file.txt");
	if (!F) {
		cout << "\nФайл отсутствует(\n" << endl;
		return false;
	}
	else {
		Flash k;

		while (F >> k.brand >> k.volume >> k.volumetype >> k.color >> k.type) {//Пока можем извлечь
			Stack.Push(k);
		}

		F.close();
		return true;
	}
}

void Push(MyStack& Stack) {
	Flash k;
	cout << "\nБренд: ";
	cin >> k.brand;
	cout << "\nОбъём памяти: ";
	cin >> k.volume;
	cout << "\nЕдиница измерения: ";
	cin >> k.volumetype;
	cout << "\nЦвет: ";
	cin >> k.color;
	cout << "\nТип: ";
	cin >> k.type;
	Stack.Push(k);
}