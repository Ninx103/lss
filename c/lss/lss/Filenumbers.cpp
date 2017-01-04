#include "Filenumbers.h"

bool operator !=(const NumberSet & lhs, const NumberSet & rhs) {
	if (lhs.strnum != rhs.strnum) {
		return true;
	}
	return false;
}

bool operator !=(const Numbers_t & lhs, const Numbers_t & rhs) {
	for (auto i : lhs) {
		if (lhs.find(i.first)->second != rhs.find(i.first)->second) {
			return true;
		}
	}
	return false;
}

BaseNumberSet::BaseNumberSet(Numbers_t & set) :
Count(1)
{
	FileNumbersMap = set;
}

string BaseNumberSet::FileStr(string basename)
{
	string outputname = basename;
	for (auto pos : FileNumbersMap)
	{
		string str = pos.second.strnum;
		outputname.replace(pos.first, str.size(), str);
	}
	return outputname;
}

int BaseNumberSet::getHighestPositionCount()
{
	int highestcount = 0;
	int highestpos = 0;
	for (auto pos : FileNumbersMap) {
		if ((pos.second.positioncount > highestcount) ||
			((pos.second.positioncount == highestcount && 
			pos.first > highestpos))) 
		{
			highestcount = pos.second.positioncount;
			highestpos = pos.first;
		}
	}
	return highestpos;
}

bool BaseNumberSet::isIncrementOf(BaseNumberSet set, int pos) {
	for (auto i : FileNumbersMap) {
		if (pos == i.first) continue;
		if (i.second != set[i.first]) {
			return false;
		}
	}
	return true;
}

bool BaseNumberSet::IncrementBaseSet(const Numbers_t & set) {

	list<int> count = {};
	for ( auto pos : FileNumbersMap) {
		if (pos.second != set.find(pos.first)->second) {
			count.push_front(pos.first);
		}
	}
	if (count.size() == 1) {
		++Count;
		FileNumbersMap[count.front()].positioncount++;
		return true;
	}
	return false;
}

NumberSet BaseNumberSet::operator[](int position)
{
	return FileNumbersMap[position];
}