//#include "stdafx.h"
#include <iostream>
using namespace std;

struct Product {
	int volume;
	double price;
	void Out();
};
void Product::Out() {
	cout << "\nКоличество = " << volume << ' ' << "Цена = " << price << endl;
}

struct MyQueue {
	struct Node {
		Product data;
		Node* next;
	};
	int remain;
	double profit = 0;
	Node *First = NULL;
	int Count = 0;
	bool Push(Product);
	bool Pop(Product&);
	void Info();
	int Sell(int, double, bool);
};

bool MyQueue::Push(Product data) {
	if (!First) {
		First = new Node;
		First->next = NULL;
		First->data = data;
		Count = 1;
		remain = First->data.volume;
	}
	else {
		Node *temp;
		temp = First;
		while (temp->next != NULL) temp = temp->next;
		temp->next = new Node;
		temp->next->data = data;
		remain += temp->next->data.volume;
		temp->next->next = NULL;
		Count++;
	}
	return true;
}

bool MyQueue::Pop(Product& data) {
	if (!First) return false;
	Node* temp = First->next;
	data = First->data;
	remain -= First->data.volume;
	delete First;
	First = temp;
	Count--;
	return true;
}



void MyQueue::Info() {
	if (!First) {
		cout << "--->Очередь пуста" << endl;
		cout << "\nОстаток = " << remain << "\nДоход = " << profit;
	}
	else {
		cout << endl << "Информация: " << endl;
		cout << "\tРазмер очереди = " << Count << endl;
		//cout << "YeFirst data" << First->data << endl << endl;
		First->data.Out();

		cout << "\nОстаток = " << remain << "\nДоход = " << profit;
	}
}

int MyQueue::Sell(int volume, double price, bool fsell) {
	//cout << "\nDebugging:\n" << volume << endl;
	Product temp;

	if (volume == 0) return true;


	if (price < First->data.price && fsell == true) {
		cout << "\nСлишком низкая цена\n";
		return false;
	}
	else {
		if (volume > remain) {
			cout << "\nНедостаточно товаров\n";
			return -1;
		}
		if (First->data.volume > volume) {
			First->data.volume -= volume;
			profit += (price - First->data.price) * volume;
			//profit += price * volume;
			remain -= volume;
			return 0;
		}
		else {
			profit += (price - First->data.price) * First->data.volume;
			volume -= First->data.volume;
			Pop(temp);
			return volume;
		}
	}
}




int main()
{
	setlocale(LC_ALL, "rus");
	int n = 10;
	Product k;
	Product k1 = { 100, 10.0 }, k2 = { 10, 20.0 };
	MyQueue Q;

	Q.Info();
	Q.Push(k1);
	Q.Push(k2);
	Q.Info();
	int sellc, sellp, selld;
	do {
		cout << "\n__________________\n";
		cout << "\nВведите кол-во товара на продажу (0 для выхода): ";
		cin >> sellc;
		if (sellc > 0) {
			cout << "\nВведите стоимость продажи: ";
			cin >> sellp;
			if (sellp < 0) sellp = 0;
			selld = Q.Sell(sellc, sellp, true);
			while (selld > 0) {
				selld = Q.Sell(selld, sellp, false);
			}
			Q.Info();
		}
	} while (sellc > 0);
	cout << "\n__________________\n";
	while (Q.Pop(k)) k.Out();
	cout << endl;
	Q.Info();

	return 0;
}


