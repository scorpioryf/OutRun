#pragma once
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;
class MyList
{
public:
	MyList();
	~MyList();
	void loadList();
	void writeList();
	void insert(int temp);
	void show();
	void clear();
private:
	vector<int> list_vec;
};


