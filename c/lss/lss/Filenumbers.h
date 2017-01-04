#pragma once
#include <string>
#include <map>
#include <list>

using namespace std;

struct NumberSet {
	int positioncount;
	string strnum;
	int intnum;
};

class FileNumberSet {
	int Count;
	map<int, NumberSet> FileNumbersMap;
public:
	FileNumberSet() {};

	void insertNumberSet(int position, string strnum, int positioncount);
	void insertNumberSet(int position, string strnum);
	void insertNumberSet(int position, NumberSet set);
	
	int getCount() { return Count; }
	string FileStr(string basename);
	int getHighestPositionCount();
	bool isIncrementOf(FileNumberSet set, int pos);

	bool IncrementBaseSet(FileNumberSet set);
	NumberSet operator[](int position);
};