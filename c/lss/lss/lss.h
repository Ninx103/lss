#pragma once
#include <list>
#include <vector>
#include <map>
#include <string>
#include <numeric>
#include <cstdio>
#include <iostream>


#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
using namespace std;
using namespace boost::filesystem;
using boost::regex_search;
using boost::regex_replace;
using boost::regex;
using boost::sregex_iterator;
using namespace std;

namespace lss {
	void lss(char * p);
}

struct NumberSet {
	int positioncount;
	string strnum;
	int intnum;
};

typedef map<const int, NumberSet> Numbers_t;

class BaseNumberSet {
	string BaseName;
	int Count;
	Numbers_t FileNumbersMap;
	list<Numbers_t *> MatchingNumberSets;

public:
	BaseNumberSet() : BaseName(""), Count(0){};
	BaseNumberSet(string basename, Numbers_t & set);

	int getCount() { return Count; }
	string asString() { return BaseName; };
	string asPaddedString();

	int getHighestPositionCount();
	bool isIncrementOf(Numbers_t & set, int position);
	bool IncrementBaseSet(const Numbers_t & set);

	const NumberSet operator[](int position);
};

struct BaseGroups {
	list<BaseNumberSet> Bases;
	list<Numbers_t> Files;

	BaseGroups() : Bases({}), Files({}) {}
	BaseGroups(list<BaseNumberSet> bases, list<Numbers_t> files) :
		Bases(bases), Files(files) {};
	void print();
};