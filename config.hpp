#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cctype>
#include <string>
#include <algorithm>
using ConfigMap = std::unordered_map<std::string, std::string>;
/* Config - Configuration related stuff
*/

ConfigMap LoadConfig(std::string path);
int GetConfig(ConfigMap& conf, std::string value); // Get yes-no config variables
std::string GetStrConfig(ConfigMap& conf, std::string value); // Get string config variables. none yet but i like futureproofing(lie)
