#ifndef LOGGER_H
#define LOGGER_H
//#include "analyzer-config.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
using std::time;
using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::abort;
using std::ostream;
using std::ofstream;
using std::fstream;
class Logger
{
public:
	static void createStream(string filename)
	{
		if (Logger::ifOutStreamValid == false)
		{
			Logger::outstream.open(filename.c_str(),fstream::app|fstream::out);
			Logger::nullStream.open("dev/null");
			if(outstream.is_open()==true)
			{
				ifOutStreamValid = true;
			}
		}
	}
	/* for debug */
	static void d(string tag,string msg)
	{
#ifdef PLUGIN_DEBUG
		++staticDebugCounter;
		if(Logger::ifOutStreamValid==false)
		{
			createStream("log.txt");
		}
		if(Logger::ifOutStreamValid==false)
		{
			cerr<<"Out stream is not available."<<endl;
			abort();
		}
		outstream<<"[debug]"<<getCurrentTimeString()<<"["<<staticDebugCounter<<"]["<<tag<<"]["<<msg<<"]"<<endl;
#endif
	}
	static ostream& d(string tag)
	{
#ifdef PLUGIN_DEBUG
		++staticDebugCounter;
		if(Logger::ifOutStreamValid==false)
		{
			createStream("log.txt");
		}
		if(Logger::ifOutStreamValid==false)
		{
			cerr<<"Out stream is not available."<<endl;
			abort();
			return outstream;
		}
		outstream<<"[debug]"<<getCurrentTimeString()<<"["<<staticDebugCounter<<"]["<<tag<<"]"<<"msg:";
		return outstream;
#else
		return nullStream;
#endif
	}
	/* for error */
	static void e(string tag,string msg)
	{
		++staticErrorCounter;
		if(Logger::ifOutStreamValid==false)
		{
			createStream("log.txt");
		}
		if(Logger::ifOutStreamValid==false)
		{
			cerr<<"Out stream is not available."<<endl;
			abort();
		}
		outstream<<"[error]"<<getCurrentTimeString()<<"["<<staticErrorCounter<<"]["<<tag<<"]["<<msg<<"]"<<endl;
	}
	static ostream& e(string tag)
	{
		++staticErrorCounter;
		if(Logger::ifOutStreamValid==false)
		{
			createStream("log.txt");
		}
		if(Logger::ifOutStreamValid==false)
		{
			cerr<<"Out stream is not available."<<endl;
			abort();
			return outstream;
		}
		outstream<<"[error]"<<getCurrentTimeString()<<"["<<staticErrorCounter<<"]["<<tag<<"]"<<"msg:";
		return outstream;
	}
	/* for info */
	static void i(string tag,string msg)
	{
		++staticInfoCounter;
		if(Logger::ifOutStreamValid==false)
		{
			createStream("log.txt");
		}
		if(Logger::ifOutStreamValid==false)
		{
			cerr<<"Out stream is not available."<<endl;
			abort();
		}
		outstream<<"[info]"<<getCurrentTimeString()<<"["<<staticInfoCounter<<"]["<<tag<<"]["<<msg<<"]"<<endl;
	}
	static ostream& i(string tag)
	{
		++staticInfoCounter;
		if(Logger::ifOutStreamValid==false)
		{
			createStream("log.txt");
		}
		if(Logger::ifOutStreamValid==false)
		{
			cerr<<"Out stream is not available."<<endl;
			abort();
			return outstream;
		}
		outstream<<"[info]"<<getCurrentTimeString()<<"["<<staticInfoCounter<<"]["<<tag<<"]"<<"msg:";
		return outstream;
	}
	/* for analyze */
	static void a(string tag,string msg)
	{
		++staticAnalyzeCounter;
		if(Logger::ifOutStreamValid==false)
		{
			createStream("log.txt");
		}
		if(Logger::ifOutStreamValid==false)
		{
			cerr<<"Out stream is not available."<<endl;
			abort();
		}
		outstream<<"[analyze]"<<getCurrentTimeString()<<"["<<staticAnalyzeCounter<<"]["<<tag<<"]["<<msg<<"]"<<endl;
	}
	static ostream& a(string tag)
	{
		++staticAnalyzeCounter;
		if(Logger::ifOutStreamValid==false)
		{
			createStream("log.txt");
		}
		if(Logger::ifOutStreamValid==false)
		{
			cerr<<"Out stream is not available."<<endl;
			abort();
			return outstream;
		}
		outstream<<"[analyze]"<<getCurrentTimeString()<<"["<<staticAnalyzeCounter<<"]["<<tag<<"]"<<"msg:";
		return outstream;
	}
private:
	Logger(){}
	static string getCurrentTimeString()
	{
		  time_t rawtime;
		  struct tm * timeinfo;

		  /* 100 should enough for time string */
		  char buffer [100];

		  time ( &rawtime );
		  timeinfo = localtime ( &rawtime );

		  strftime(buffer,99,"[%m-%d %H:%M:%S]",timeinfo);
		  return buffer;
	}
private:
	static ofstream outstream;
	static bool ifOutStreamValid;
	static int staticDebugCounter;
	static int staticInfoCounter;
	static int staticErrorCounter;
	static int staticAnalyzeCounter;
	static ofstream nullStream;
};
#endif /* LOGGER_H */
