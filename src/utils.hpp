#pragma once

/*
 * utils - look mom i'm typing with color! wowzers!!
 */

//includes
#include <string>
#include <array>
#include <fstream>
#include <unistd.h>
#include <filesystem>
#include <vector>

// color codes awesome
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define ITALIC  "\033[3m"
#define UNDERL  "\033[4m"
// By default these are bold.
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define PURPLE  "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
// These are light (aka not bold.)
#define REDA    "\033[0;31m"
#define GREENA  "\033[0;32m"
#define YELLOWA "\033[0;33m"
#define BLUEA   "\033[0;34m"
#define PURPLEA "\033[0;35m"
#define CYANA   "\033[0;36m"

#define BASE    "\033[38;5;"

#define NIXCOLOR2 "\033[38;2;82;119;195m" // for future


// Function to use any ASCII color.
std::string colormein(std::string cnum);
std::string distroColor();

// functions
std::string exec(const std::string& cmd);  // popen wrapper. used all over the place.
std::string readFileTrim(const std::string& path); // used in getNiceDriveListBecauseWeShouldAllHaveOurHappyMoments and probably somewhere else too.
std::vector<unsigned char> readFileByte(const std::string& path); // used in getDisp to interface with the EDID.
