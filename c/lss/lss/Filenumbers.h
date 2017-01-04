#pragma once
#include <string>
#include <list>

using namespace std;
class Numberset {
	struct set {
		short position;
		string strnum;
		int intnum;
	};
	list<set>;
public:
	Numberset();
	Numberset(unsigned short start, string strnum);

	short getPosition() { return position; };
	string getStrnum() { return strnum; };
	int getIntnum() { return intnum; };

	bool operator == (Numberset & right);
	bool operator != (Numberset & right);
	string str() { return to_string(position) + ": ( " + strnum + ", " + to_string(intnum) + ")"; }
};

class BaseVersion {
	int count;
	list<Numberset> basefilenumbers;
public:
	BaseVersion() : count(0) {};
	BaseVersion(list<Numberset> basefilenumbers);
	BaseVersion(int count, list<Numberset> basefilenumbers);
	
	int getCount() { return count; };
	list<Numberset> getBaseFileNumbers() { return basefilenumbers; };
	string baseNumbersStr();
	string str() { return to_string(count) + ": " + baseNumbersStr(); }

	bool operator == (BaseVersion & right);
	bool operator == (list<Numberset> & right);
	bool operator != (BaseVersion & right);
	bool operator != (list<Numberset> & right);
};
