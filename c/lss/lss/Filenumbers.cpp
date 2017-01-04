#include "Filenumbers.h"




bool operator !=(NumberSet & lhs, NumberSet & rhs) {
	if (lhs.strnum != rhs.strnum) {
		return true;
	}
	return false;
}

void FileNumberSet::insertNumberSet(int position, string strnum, int positioncount)
{
	NumberSet set = { positioncount, strnum, stoi(strnum) };
	insertNumberSet(position, set);
}


void FileNumberSet::insertNumberSet(int position, string strnum)
{
	NumberSet set = { 0 , strnum, stoi(strnum) };
	insertNumberSet(position, set);
}

void FileNumberSet::insertNumberSet(int position, NumberSet set)
{
	if (FileNumbersMap.find(position) == FileNumbersMap.end() ){
		FileNumbersMap.insert({ position, set });
	}
}

string FileNumberSet::FileStr(string basename)
{
	string outputname = basename;
	for (auto pos : FileNumbersMap)
	{
		string str = pos.second.strnum;
		outputname.replace(pos.first, str.size(), str);
	}
	return outputname;
}

int FileNumberSet::getHighestPositionCount()
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

bool FileNumberSet::isIncrementOf(FileNumberSet set, int pos) {
	for (auto i : FileNumbersMap) {
		if (pos == i.first) continue;
		if (i.second != set[i.first]) {
			return false;
		}
	}
	return true;
}

bool FileNumberSet::IncrementBaseSet(FileNumberSet set) {

	list<int> count;
	for (auto pos : FileNumbersMap) {
		if (pos.second != set[pos.first]) {
			count.push_front(pos.first);
		}
	}
	if (count.size() == 1) {
		Count++;
		FileNumbersMap[count.front()].positioncount++;
		return true;
	}
	return false;
}

NumberSet FileNumberSet::operator[](int position)
{
	return FileNumbersMap[position];
}