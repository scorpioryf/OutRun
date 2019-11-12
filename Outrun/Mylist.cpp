#include "pch.h"
#include "Mylist.h"


Mylist::Mylist()
{
}

Mylist::~Mylist()
{
	list_vec.clear();
}

void Mylist::insert(int temp) {
	for (unsigned int i = 0; i < list_vec.size(); i++) {
		if (list_vec[i] >= temp) {
			list_vec.insert(list_vec.begin() + i, temp);
			return;
		}
	}
	list_vec.push_back(temp);
}

void Mylist::loadList() {
	ifstream infile;
	int temp;
	infile.open("list/list.txt");
	if (!infile.is_open()) {
		cout << "LOAD Open file failed!" << endl;
	}
	while (!infile.eof()) {
		infile >> temp;
		if (temp < 0) {
			break;
		}
		list_vec.push_back(temp);
	}
	infile.close();
}

void Mylist::writeList() {
	ofstream outfile;
	outfile.open("list/list.txt");
	if (!outfile.is_open()) {
		cout << "WRITE Open file failed!" << endl;
	}
	for (unsigned int i = 0; i < list_vec.size(); i++) {
		outfile << list_vec[i];
		if (i != list_vec.size() - 1) {
			outfile << " ";
		}
	}
	outfile.close();
}

void Mylist::show() {
	for (unsigned int i = 0; i < list_vec.size(); i++) {
		cout << "No." << i + 1 << "   " << list_vec[i] << endl;
	}
}

void Mylist::clear() {
	list_vec.clear();
}