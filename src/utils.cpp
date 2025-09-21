#include "utils.hpp"
#define elif else if
std::string exec(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "error";
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    // Remove trailing newline
    if (!result.empty() && result.back() == '\n')
        result.pop_back();
    return result;
}

std::string readFileTrim(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::string line;
    getline(file, line);
    while (!line.empty() && (line.back() == '\n' || line.back() == '\r' || line.back() == ' '))
        line.pop_back();
    return line;
}

std::vector<unsigned char> readFileByte(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return {};
    return std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});
}

std::string colormein(std::string cnum)
{
    return BASE+cnum+"m";
}

std::string distroColor()
{
    std::string chosen = "79";

    // Code was taken from offload.cpp because WHY NOT.
    std::string NAME = "Unknown", temp;
    std::ifstream release("/etc/os-release");
    if (!release.is_open()) return CYAN;
    
    while (std::getline(release, temp)) 
    {
        if (temp.find("ID=") == 0)
        {
            NAME = temp.substr(3);
            if (!NAME.empty() && NAME.back() == '"') NAME.pop_back();
        }
    }
    // these names will 99% not work
    if (NAME == "arch")         chosen = CYAN;//"\033[38;2;23;147;209m";
    elif (NAME == "ubuntu")     chosen = "\033[38;2;224;72;51m";
    elif (NAME == "manjaro")    chosen = "\033[38;2;27;185;120m";
    elif (NAME == "nix")        chosen = "\033[38;2;126;186;228m"; // color 1
    elif (NAME == "debian")     chosen = "\033[38;2;215;10;83m";
    elif (NAME == "fedora")     chosen = "\033[38;2;41;65;114m";
    elif (NAME == "redhat")     chosen = "\033[38;2;238;0;0m";
    elif (NAME == "gentoo")     chosen = "\033[38;2;94;38;205m";
    elif (NAME == "linuxmint")  chosen = "\033[38;2;135;207;94m";
    else                        chosen = CYAN;


    return chosen;
}