#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <vector>
#include <string>

using std::vector;
using std::string;

struct ConfigUnit
{
    string name;
    string flagOn;
};

class Config
{
private:
    vector<ConfigUnit> vec_Conf;
    static Config *cPConfig;
public:
    Config();
    ~Config();
    void Init();
    bool IsOn(string);
    static Config& GetInstance();
};


#endif
