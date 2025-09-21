#include <bits/stdc++.h>
using namespace std;
// Examine this fuckery.


std::vector<std::string> splitLines(const std::string& str) {
    std::vector<std::string> lines;
    std::istringstream ss(str);
    std::string line;
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }
    return lines;
}
std::string getDistroLogo() {
    return R"(
      .-~~~-.  
     \/       \\ 
    |         |
    |   ( )   |
     \\       \/
      `-----'  
    )";
}


int main() {
    std::vector<std::string> logoLines = splitLines(getDistroLogo());

    std::vector<std::string> infoLines = {
        "line1",
        "line 2 blah blah",
        "big boner down the lane",
        "stupidfetch is stupid",
        "line 67 mango"
    };

    size_t maxLines = std::max(logoLines.size(), infoLines.size());

   
    while (logoLines.size() < maxLines) logoLines.push_back("");
    while (infoLines.size() < maxLines) infoLines.push_back("");

   
    for (size_t i = 0; i < maxLines; ++i) {
        std::cout << std::left << std::setw(20) << logoLines[i] << "  " << infoLines[i] << '\n';
    }

    return 0;
}
