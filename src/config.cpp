#include "config.hpp"

ConfigMap LoadConfig(std::string path)
{
    ConfigMap confucius;
    std::ifstream confile(path);
    std::string line;
    
    while (std::getline(confile,line))
    {
        if (line.empty() || line[0] == '#') continue; // Skip commentius

        size_t pos = line.find('=');
        if (pos == std::string::npos) continue; // malformed pair

        std::string key = line.substr(0,pos);
        std::string value = line.substr(pos+1);
        confucius[key] = value;
    }
    return confucius;
}

// Basically returns whatever you put in if it's either a boolean or a integer.
int GetConfig(ConfigMap& conf, std::string value)
{
    int returner = 0;
    auto it = conf.find(value);
    if (it != conf.end())
    {
        std::string temp = it->second;
        bool notInt = false;
        try
        {
            int _IGNORE_THANKS = stoi(temp);
        } catch (...)
        {
            notInt = true;
        }
        std::transform(temp.begin(), temp.end(), temp.begin(), [](char c){ return std::toupper(c); });
        if (!notInt)
        {
            returner = stoi(temp);
        }
        else if (temp == "YES" || temp == "YEP" || temp == "TRUE" || temp == "UHHUH" || temp == "YUP" || temp == "OFCOURSE" || temp == "SURE")
        {
            returner = 1;
        } 
        else if (temp == "NO" || temp == "NOPE" || temp == "FALSE" || temp == "NUHUH" || temp == "NEVER" || temp == "HELLNO" || temp == "NAH")
        {
            returner = 0;
        }
    }
    return returner;
}

// Gives you the value for the key directly.
std::string GetStrConfig(ConfigMap& conf, std::string value)
{
    std::string returner = "unknown";
    auto it = conf.find(value);
    if (it != conf.end())
    {
        returner = it->second;
    }
    return returner;
}