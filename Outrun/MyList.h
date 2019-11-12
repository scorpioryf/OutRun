#pragma once
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


