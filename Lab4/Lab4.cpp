//#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Book {
	string name;
	int pages;
	bool type;
	double weight;

	void Out();
	bool Seria(char*& data, int& n);
};
void Book::Out() {
	cout << "\nName = " << name << " Pages = " << pages << " Type = " << type << " Weight = " << weight << endl;
}
Book DeSeria(char* data, int n);

void ClearBinaryFile() {
	fstream out("out.dat", ios::out | ios::binary);
	out.close();
}
void PrintBinaryFile(char* data, int n) {
	fstream out("out.dat", ios::app | ios::binary);
	out.write((char*)&n, sizeof(int));
	out.write(data, n);
	out.close();
}
bool InputBinaryFile() {
	fstream in("out.dat", ios::in | ios::binary);
	if (!in) {
		cout << "-->Error, there are no input binary file" << endl;
		return false;
	}

	int i = 1, n;
	Book C;

	while (!in.eof()) {
		if (in.read((char*)&n, sizeof(int))) {
			cout << "\t - " << i << " - " << " seria length : " << n << endl << endl;
			char* data = new char[n];
			in.read(data, n);
			i++;
			C = DeSeria(data, n);
			C.Out();
			cout << "\n--------------\n";
			delete[] data;
		}
	}
	in.close();
	return true;
}

bool Book::Seria(char*& data, int& n) {
	size_t s = name.size();

	int n0 = sizeof(size_t);
	int n1 = s;
	int n2 = sizeof(int);
	int n3 = sizeof(bool);
	int n4 = sizeof(double);

	n = n0 + n1 + n2 + n3 + n4;

	data = new char[n];

	char* d0 = reinterpret_cast<char*>(&s);
	char* d1 = const_cast<char*>(name.c_str());
	char* d2 = reinterpret_cast<char*>(&pages);
	char* d3 = reinterpret_cast<char*>(&type);
	char* d4 = reinterpret_cast<char*>(&weight);

	for (int i = 0; i < n0; i++) data[i] = d0[i];
	for (int i = 0; i < n1; i++) data[i + n0] = d1[i];
	for (int i = 0; i < n2; i++) data[i + n0 + n1] = d2[i];
	for (int i = 0; i < n3; i++) data[i + n0 + n1 + n2] = d3[i];
	for (int i = 0; i < n4; i++) data[i + n0 + n1 + n2 + n3] = d4[i];

	return true;
}
Book DeSeria(char* data, int n) {
	Book temp;
	int n0, n1, n2, n3, n4;
	n0 = sizeof(size_t);
	n2 = sizeof(int);
	n3 = sizeof(bool);
	n4 = sizeof(double);

	size_t s = *reinterpret_cast<size_t*>(data);
	n1 = s;

	string ss(data + n0, s);
	temp.name = ss;
	temp.pages = *reinterpret_cast<int*>(data + n0 + n1);
	temp.type = *reinterpret_cast<bool*>(data + n0 + n1 + n2);
	temp.weight = *reinterpret_cast<double*>(data + n0 + n1 + n2 + n3);

	return temp;
}

struct MyQueue {
	struct Node {
		int n; //Размер серии
		char* data; //Сама серия
		Node* next;
	};
	Node *First = NULL;
	int Count = 0;
	bool Push(char*, int);
	bool Pop(char*&, int&);
	void Info();
};

bool MyQueue::Push(char* data, int n) {
	if (!First) {
		First = new Node;
		First->next = NULL;
		First->data = new char[n];
		for (int i = 0; i < n; i++) {
			First->data[i] = data[i];
		}
		First->n = n;
		Count = 1;
	}
	else {
		Node *temp;
		temp = First;
		while (temp->next != NULL) temp = temp->next;
		temp->next = new Node;
		temp->next->next = NULL;
		temp->next->data = new char[n];
		for (int i = 0; i < n; i++) {
			temp->next->data[i] = data[i];
		}
		temp->next->n = n;
		Count++;
	}
	return true;
}
bool MyQueue::Pop(char*& data, int& n) {
	if (!First) return false;
	Node* temp = First->next;
	n = First->n;
	data = new char[n];

	for (int i = 0; i < n; i++) {
		data[i] = First->data[i];
	}
	delete First->data;
	delete First;
	First = temp;
	Count--;
	return true;
}
void MyQueue::Info() {
	if (!First) cout << "--->Queue is empty" << endl;
	else {
		cout << endl << "Queue info: " << endl;
		cout << "\tQueue size = " << Count << endl;
		//cout << "YeFirst data" << First->data << endl << endl;
		//First->data.Out();
	}
}

MyQueue ReadBinaryFile() {
	MyQueue qu;
	fstream in("out.dat", ios::in | ios::binary);
	if (!in) {
		cout << "-->Error, there are no input binary file" << endl;
		return qu;
	}

	int i = 1, n;
	Book C;

	while (!in.eof()) {
		if (in.read((char*)&n, sizeof(int))) {
			char* data = new char[n];
			in.read(data, n);
			i++;
			qu.Push(data, n);
			delete[] data;
		}
	}

	in.close();
	return qu;
}

int main() {
	setlocale(LC_ALL, "rus");

	MyQueue qu;

	short int m;
	do {
		cout << "\nВыберите действие\n1. Добавить книгу в очередь\n2. Удалить книгу из очереди\n3. Очистить очередь\n4. Сохранить в файл\n5. Загрузить из файла\n6. Вывод списка\n0. Выход: ";
		cin >> m;

		if (m == 1) {
			string name;
			int pages;
			bool type;
			double weight;

			cout << "\nНазвание: ";
			cin >> name;
			cout << "\nКол-во страниц: ";
			cin >> pages;
			cout << "\nТип: ";
			cin >> type;
			cout << "\nВес: ";
			cin >> weight;

			Book b = { name, pages, type, weight };

			char* data;
			int n;
			b.Out();
			b.Seria(data, n);
			qu.Push(data, n);

			delete[] data;
		}
		
		if (m == 2) {
			cout << "\nВведите название искомой книги: ";
			string name;
			cin >> name;
			bool flag = false;
			char* data;
			int n;

			for (unsigned int i = 0; i < qu.Count && flag ==  false; i++) {
				qu.Pop(data, n);
				Book b = DeSeria(data, n);
				if (b.name == name) {
					flag = true;
				}
				if (flag == false) qu.Push(data, n);
				delete[] data;
			}
			if (flag == false) cout << "\nТакой книги нет(\n";
		}

		if (m == 3) {
			char* data;
			int n;
			while (qu.Pop(data, n)) {
				Book b = DeSeria(data, n);
				b.Out();
				delete[] data;
			}
		}

		if (m == 4) {
			ClearBinaryFile();

			for (int i = 0; i < qu.Count; i++) {
				char* data;
				int n;
				qu.Pop(data, n);
				PrintBinaryFile(data, n);
				delete[] data;
			}
			cout << "\n---------------\n";

			InputBinaryFile();
		}

		if (m == 5) {
			qu = ReadBinaryFile();
		}

		if (m == 6) {
			char* data;
			int n;
			MyQueue qu2 = qu;
			while (qu.Pop(data, n)) {
				Book b = DeSeria(data, n);
				b.Out();
			}
			qu = qu2;
		}
		
		qu.Info();
	} while (m != 0 && m < 6);

	char* data;
	int n;
	while (qu.Pop(data, n)) {
		Book b = DeSeria(data, n);
		b.Out();
		delete[] data;
	}
}