#pragma once
#include <list>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct NumberSet {
	int positioncount;
	string strnum;
	int intnum;
};

typedef map<const int, NumberSet> Numbers_t;

class BaseNumberSet {
	int Count;
	Numbers_t FileNumbersMap;

public:
	BaseNumberSet() {};
	BaseNumberSet(Numbers_t & set);

	int getCount() { return Count; }
	string FileStr(string basename);
	int getHighestPositionCount();
	bool isIncrementOf(BaseNumberSet set, int pos);

	bool IncrementBaseSet(const Numbers_t & set);
	NumberSet operator[](int position);
};