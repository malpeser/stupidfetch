#pragma once
#include "utils.hpp"
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <utility>

std::vector<std::string> getGpu(int fast = 0); // get GPU name & fancies.