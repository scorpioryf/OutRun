#pragma once
using namespace std;
class Mylist
{
public:
	Mylist();
	~Mylist();
	void loadList();
	void writeList();
	void insert(int temp);
	void show();
	void clear();
private:
	vector<int> list_vec;
};


