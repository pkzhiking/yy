/*
 * Logger.cpp
 *
 *  Created on: Apr 7, 2012
 *      Author: Guo Jiuliang
 */
#include "include/Logger.h"
ofstream Logger::outstream ;
bool Logger::ifOutStreamValid = false;
int Logger::staticDebugCounter = 0;
int Logger::staticErrorCounter = 0;
int Logger::staticAnalyzeCounter = 0;
int Logger::staticInfoCounter = 0;
ofstream Logger::nullStream;


