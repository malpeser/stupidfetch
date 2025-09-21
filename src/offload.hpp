#pragma once

//includs
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <asm/termbits.h>
#include <algorithm>
#include <sys/socket.h>
#include <string>
#include <filesystem>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <iomanip>
#include <sys/ioctl.h>

/*
 * offload - everything is here so it's better right? right? right?
 */

// structs. because why not i like structs

struct HostPair {
    std::string hostname;
    std::string username;
};

struct IPPair {
    std::string publik;
    std::string local;
};

struct PackInfo {
    std::string manager;
    int count;
};

struct SessionInfo {
    std::string denv;
    std::string wmgr;
};

struct CPUInfo {
    std::string name;
    std::string temp;
    //float usage;
};

struct Drives {
    std::string name;
    std::string size;
    bool isPhysical;
};

struct BatteryInfo {
    int capacity;
    std::string status;
};

struct DriveInfo {
    float capacity;
    std::string model;
    bool physical;
    std::string type;
};

struct DisplayInfo {
    std::string name;
    std::string reshz;
};


struct CpuTimes { // dont want this actually
    long long user, nice, system, idle, iowait, irq, softirq, steal;

    long long total() const {
        return user + nice + system + idle + iowait + irq + softirq + steal;
    }

    long long idleTime() const {
        return idle + iowait;
    }
};


// **** USERSPACE RELATED **** //
std::string getOSName();
std::string getProduct();
HostPair getHostUser(); // bad name but it gets both hostname and username and returns it in hostname@username way.
std::string getKernel();
std::string getUptime(); // why is this a string you may ask. it's because i'm fucking lazy thats why.
std::string getShell();
std::string getTermSize();
std::string getTerminal();
int getProcessNum(); // self-explanatory
std::vector<DisplayInfo> getDisp(); // vector because we may have multiple displays. also yes this is uuserspace too.
SessionInfo getDeWm(); // smushing these two together is a great idea! no
BatteryInfo getBattery(); // int. also this is userspace because i said so.
std::string getLocale(); // why would this ever be useful
PackInfo getPkgManCnt(); // gets both package manager and how many packages we have.


// **** CPU RELATED **** //
CPUInfo getCpu();
std::string getCpuTemp(); // helper function to above
float getCpuUsg(); // helper function to above


// **** GPU/MEMORY RELATED **** //
// GPU stuff has been moved to its own file.
std::string getMemory(); // you may wonder again. why is this a string. refer to above.
std::string getSwapMaybe(); // if this returns "noswap" there is no swap :(


// **** DISK RELATED **** //
std::vector<DriveInfo> getNiceDriveListBecauseWeShouldAllHaveOurHappyMoments(); // :) :) :) :) :)
std::string getUsageRoot(); // root drive thingy. basically returns usedGB/maxGB (round((used/max)*100))


// **** NETWORK RELATED **** //
IPPair getIP(bool doPublic = false); // local && public
std::string getWifi(); // because why not.
