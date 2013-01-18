/*
 * Util.h
 *
 *  Created on: Apr 14, 2012
 *      Author: Guo Jiuliang
 */

#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <sstream>
using std::string;
using std::stringstream;
class Util
{
public:
	static int stringToInt(const string&);
	static string intToString(int);
};
#endif /* UTIL_H_ */
