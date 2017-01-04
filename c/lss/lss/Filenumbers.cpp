#include "Filenumbers.h"

Numberset::Numberset() :
	position(-1), strnum("-1"), intnum(-1) {
}

Numberset::Numberset(unsigned short position, string strnum) :
	position(position), strnum(strnum) {
	intnum = std::stoi(strnum);
}

bool Numberset::operator == (Numberset & right)
{
	if ((position == right.position) &&
		(strnum == right.strnum)) {
		return true;
	}
	return false;
}

bool Numberset::operator!=(Numberset & right)
{
	if (position != right.getPosition() ||
		strnum != right.getStrnum()) {
		return true;
	}
	return false;
}

BaseVersion::BaseVersion(list<Numberset> basefilenumbers) :
	count(1), basefilenumbers(basefilenumbers) {
}

BaseVersion::BaseVersion(int count, list<Numberset> basefilenumbers) :
	count(count), basefilenumbers(basefilenumbers) {
}

string BaseVersion::baseNumbersStr()
{
	string outstring = "{ ";
	for (Numberset i : basefilenumbers) {
		outstring +=  to_string(i.getPosition()) + " : " +
			"(\"" + i.getStrnum() + "\", " +
			to_string(i.getIntnum()) + ") ";
	}
	outstring += "}";
	return outstring;
}

bool BaseVersion::operator==(BaseVersion & right)
{
	if (count == right.getCount() &&
		basefilenumbers == right.getBaseFileNumbers()) {
		return true;
	}
	return false;
}

bool BaseVersion::operator==(list<Numberset> & right)
{
	if (basefilenumbers == right) {
		return true;
	}
	return false;
}

bool BaseVersion::operator!=(BaseVersion & right)
{
	if (count != right.getCount() ||
		basefilenumbers != right.getBaseFileNumbers()) {
		return true;
	}
	return false;
}

bool BaseVersion::operator!=(list<Numberset> & right)
{
	if (basefilenumbers != right) {
		return true;
	}
	return false;
}
