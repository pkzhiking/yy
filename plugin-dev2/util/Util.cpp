/*
 * Util.cpp
 *
 *  Created on: Apr 14, 2012
 *      Author: Guo Jiuliang
 */
#include "include/Util.h"
int
Util::stringToInt(const string& value)
{
	stringstream sstr;
	sstr<<value;
	int result;
	sstr>>result;
	return result;
}
string
Util::intToString(int value)
{
	stringstream sstr;
	sstr<<value;
	return sstr.str();
}

double
Util::stringToDouble(const string& strValue)
{
    stringstream sstr;
    sstr << strValue;
    double dResult;
    sstr >> dResult;
    return dResult;
}
