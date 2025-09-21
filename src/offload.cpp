// Definitions and includes and other stuff
#include "offload.hpp"
#include "curler.hpp"
#include "utils.hpp"
using std::string;
using std::vector;
using std::ifstream;
using std::ostringstream;
using std::getenv; //im 99% sure i didnt use std:: sometimes and im too lazy to fix em
using std::getline; // so these are to be safe
using std::stoi; // this too

// Juicy implementation

// getOSName - Get pretty os name and return it.
string getOSName() 
{
    string NAME = "Unknown", temp;
    ifstream release("/etc/os-release");
    if (!release.is_open()) return NAME;
    
    while (std::getline(release, temp)) 
    {
        if (temp.find("PRETTY_NAME=") == 0)
        {
            NAME = temp.substr(13);
            if (!NAME.empty() && NAME.back() == '"') NAME.pop_back();
            return NAME;
        }
    }
    return NAME;
}

// getProduct - Get the product name of the host.
string getProduct()
{
    ifstream pdname("/sys/class/dmi/id/product_name");
    string name = "unknown";
    if (pdname.is_open()) {
        std::getline(pdname, name);
        pdname.close();
    }
    return name;
}

// getHostUser - Get hostname and username and return it.
HostPair getHostUser()
{
    HostPair result = {"unknown", "unknown"};
    const char* user = getenv("USER");
    if (!user) user = "unknown";
    result.username = user;
    char host[256];
    if(gethostname(host, sizeof(host)) != 0) result.hostname = "unknown";
    result.hostname = host;
    return result;
}

// getKernel - Get kernel version and return it.
string getKernel()
{
    struct utsname m;
    if(uname(&m) != 0) return "unknown";
    return string(m.release);
}

// getUptime - Get uptime and prettify and return it.
string getUptime() 
{
    ifstream uptime("/proc/uptime");
    if (!uptime.is_open()) return "unknown";

    double dseconds;
    uptime >> dseconds;

    long seconds = static_cast<long>(dseconds);

    if (seconds < 60) return std::to_string(seconds) + " secs"; // when too smol, escape

    int days = seconds / 86400; // magic number!
    int hours = (seconds / 3600) % 24; // magic number!
    int minutes = (seconds / 60) % 60; // magic number!

    ostringstream fancyUptime;
    if (days > 0) fancyUptime << days << "days, ";
    if (hours > 0 || days > 0) fancyUptime << hours << " hours, ";
    fancyUptime << minutes << " mins";

    return fancyUptime.str();
}

// getShell - Get the shell you are on. It was supposed to return the version too
// but that's too time consuming :,(
string getShell()
{
    const char* spath = getenv("SHELL");
    if (!spath) return "unknown";

    string shell(spath);
    size_t pos = shell.find_last_of('/');
    if (pos != string::npos) shell = shell.substr(pos+1);
    return shell;
}

// getTerm - size
string getTermSize() 
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) return "unknown";

    return std::to_string(w.ws_col) + " columns x " + std::to_string(w.ws_row) + " rows";
}

string getTerminal()
{
    pid_t ppid = getppid();
    string currentPid = std::to_string(ppid);
    string temmy;
    
    while(true) {
        string parentPid = exec(("cat /proc/" + currentPid + "/stat | cut -d ' ' -f 4").c_str());
        string parny = exec(("ps -p " + parentPid + " -o comm=").c_str());
        // did we go too far?
        if (parny.empty() || parny == "systemd" || parny == "init") {
            temmy = "unknown";
            break;
        }
        if (parny == "bash" || parny == "zsh" || parny == "sh" || parny == "dash" || parny == "fish") {
            currentPid = parentPid;
            continue;
            } else {
            temmy = parny; // found 'em
            break;
        }
    }
    return temmy;
}

// getProcessNum - sfetch 1.1
int getProcessNum()
{
    return stoi(exec("ps -e --no-headers | wc -l"));
}

// helper function to getDisp
// parseEDID - parse the EDID.
// i do not know what this code does.
// i do not want to know.
DisplayInfo parseEDID(const unsigned char* edid, size_t len) 
{
    DisplayInfo result = {"unknown","unknown"};
    if (len < 128) return result;

    for (int i = 54; i <= 108; i += 18) {
        if (edid[i] == 0x00 && edid[i+1] == 0x00 && edid[i+2] == 0x00 && edid[i+3] == 0xFC) {
            char name[14] = {0};
            for (int j = 0; j < 13; j++) {
                char c = edid[i + 5 + j];
                if (c == 0x0A) break;
                name[j] = c;
            }
            result.name = string(name);
            break;
        }
        else if (edid[i] == 0x00 && edid[i+1] == 0x00 && edid[i+2] == 0x00 && edid[i+3] == 0xFE) { // My display's serial number was here. IDK why.
            char name[14] = {0};                                                                     // Wikipedia says it's "unspecified text".
            for (int j = 0; j < 13; j++) {
                char c = edid[i + 5 + j];
                if (c == 0x0A) break;
                name[j] = c;
            }
            result.name = string(name);
            break;
        }
        else {
            // If all else fails, be lazy.
            uint16_t rawVendor = (edid[8] << 8) | edid[9];
            char vendor[4];
            vendor[0] = ((rawVendor >> 10) & 0x1F) + 'A' - 1;
            vendor[1] = ((rawVendor >> 5) & 0x1F) + 'A' - 1;
            vendor[2] = (rawVendor & 0x1F) + 'A' - 1;
            vendor[3] = '\0';

            uint16_t productId = edid[10] | (edid[11] << 8);

            ostringstream oss2;
            oss2 << vendor << std::uppercase << std::hex << std::setw(4) << std::setfill('0') << productId;
            result.name = oss2.str();
        }
    }

    int hActive = ((edid[56] >> 4) << 8) + edid[54];
    hActive |= (edid[56] & 0xF) << 8;
    hActive = edid[54] + ((edid[56] & 0xF0) << 4);
    int hBlank = edid[55] + ((edid[56] & 0x0F) << 8);

    int vActive = edid[58] + ((edid[61] & 0xF0) << 4);
    int vBlank = edid[59] + ((edid[61] & 0x0F) << 8);

    int pixelClock = (edid[54] + (edid[55] << 8)) * 10; 
    pixelClock = edid[54] + (edid[55] << 8);
    pixelClock *= 10;

    hActive = edid[56] + ((edid[58] & 0xF0) << 4);
    hBlank = edid[57] + ((edid[58] & 0x0F) << 8);
    vActive = edid[59] + ((edid[61] & 0xF0) << 4);
    vBlank = edid[60] + ((edid[61] & 0x0F) << 8);

    float refreshRate = 0.0f;
    if (pixelClock > 0 && hActive > 0 && vActive > 0) 
    {
        int hTotal = hActive + hBlank;
        int vTotal = vActive + vBlank;
        refreshRate = (float)pixelClock * 1000.0f / (hTotal * vTotal);
    }

    ostringstream oss;
    oss << hActive << "x" << vActive << " @ " << std::fixed << std::setprecision(0) << refreshRate << " Hz";
    result.reshz = oss.str();
    return result;
}

// getDisp - get display on machine and return :(
vector<DisplayInfo> getDisp()
{
    vector<DisplayInfo> HATRED;
    for (const auto& entry : std::filesystem::directory_iterator("/sys/class/drm")) // shamelessly copied from the drive thingies.
    {
       string diskName = entry.path().filename();

        // Skip the f8key f8ke f8kes ::::)
        if (diskName.find("render") == 0) continue;

        DisplayInfo PURE;

        vector<unsigned char> CHECK = readFileByte("/sys/class/drm/" + diskName + "/edid");
        if (CHECK.size() >= 128) {
            PURE = parseEDID(CHECK.data(), 128);
            HATRED.push_back(PURE);
        }
    }
    return HATRED;
}

// getDeWm - get the desktop environment and window manager.
SessionInfo getDeWm()
{
    SessionInfo result = {"unknown","unknown"};
    string output = exec("ps -e -o comm=");
    string displays = " (X11)";
    const char* wayland = getenv("WAYLAND_DISPLAY");
    if(wayland) displays = " (Wayland)";
    static const std::unordered_map<string,string> wmMap = {
        {"kwin_wayland", "KWin"},
        {"Hyprland", "Hyprland"},
        {"kwin_x11", "KWin"},
        {"kwin", "KWin"},
        {"mutter", "Mutter"},
        {"gnome-shell", "Mutter"},
        {"xfwm4", "Xfwm"},
        {"openbox", "Openbox"},
        {"blackbox", "Blackbox"},
        {"fluxbox", "Fluxbox"},
        {"i3", "i3"},
        {"i3wm", "i3"},
        {"bspwm", "bspwm"},
        {"dwm", "dwm"},
        {"xmonad", "xmonad"},
        {"herbstluftwm", "herbstluftwm"},
        {"awesome", "Awesome"},
        {"icewm", "IceWM"},
        {"cwm", "cwm"},
        {"windowmaker", "Window Maker"},
        {"sway", "Sway"},
        {"wayfire", "Wayfire"},
        {"weston", "Weston"}
    };
    
    for (const auto& [procName, prettyName] : wmMap) {
        if (output.find(procName) != string::npos) {
            result.wmgr = prettyName + displays;
        }
    }
    
    const char* sus = getenv("DESKTOP_SESSION");
    if (!sus || sus[0] == '\0') {
        result.denv = "unknown";
    }

    string session(sus);

    static const std::unordered_map<std::string, std::string> deMap = {
        {"gnome", "GNOME"},
        {"plasma", "KDE Plasma"},
        {"kde", "KDE Plasma"},
        {"kde-plasma", "KDE Plasma"},
        {"xfce", "XFCE"},
        {"xfce4", "XFCE"},
        {"lxde", "LXDE"},
        {"lxqt", "LXQt"},
        {"mate", "MATE"},
        {"cinnamon", "Cinnamon"},
        {"budgie-desktop", "Budgie"},
        {"deepin", "Deepin"},
        {"i3", "i3"},
        {"awesome", "Awesome"},
        {"sway", "Sway"},
        {"openbox", "Openbox"},
        {"fluxbox", "Fluxbox"},
        {"blackbox", "Blackbox"},
        {"enlightenment", "Enlightenment"},
        {"generic", "Generic"},
        {"unity", "Unity"},
        {"hyprland-uwsm", "Hyprland"}
    };

    auto it = deMap.find(session);
    if (it != deMap.end()) {
        result.denv = it->second;
    }

    return result;
}

// getBattery - get battery and charge status in a nice struct.
BatteryInfo getBattery()
{
    BatteryInfo result = {1984,"unknown"};
    ifstream cap("/sys/class/power_supply/BAT0/capacity");
    ifstream stat("/sys/class/power_supply/BAT0/status");
    string temp;
    if (cap.is_open())
    {
        std::getline(cap, temp);
        result.capacity = std::stoi(temp);
    }
    if (stat.is_open())
    {
        std::getline(stat, temp);
        result.status = temp;
    }
    return result;
}

// getLocale - get the locale...
string getLocale() 
{
    const char* locale = std::getenv("LC_ALL");
    if (locale && *locale) return locale;
    
    locale = std::getenv("LC_CTYPE");
    if (locale && *locale) return locale;

    locale = std::getenv("LANG"); // third time's the charm!
    if (locale && *locale) return locale;

    return "unknown"; // it's not
}

// getPkgManCnt - get the package manager, and the amount of packages we have.
PackInfo getPkgManCnt()
{
    PackInfo result;

    if (std::filesystem::exists("/usr/bin/pacman")) {
        result.manager = "pacman";
        result.count = std::stoi(exec("pacman -Qq | wc -l"));
    }
    else if (std::filesystem::exists("/usr/bin/dpkg")) {
        result.manager = "apt";
        result.count = std::stoi(exec("dpkg --get-selections | grep -cv deinstall"));
    }
    else if (std::filesystem::exists("/usr/bin/rpm")) {
        result.manager = "rpm";
        result.count = std::stoi(exec("rpm -qa | wc -l"));
    }
    else if (std::filesystem::exists("/usr/bin/xbps-query")) {
        result.manager = "xbps";
        result.count = std::stoi(exec("xbps-query -l | wc -l"));
    }
    else if (std::filesystem::exists("/usr/bin/nix-env")) {
        result.manager = "nix";
        result.count = std::stoi(exec("nix-env -q | wc -l"));
    }
    else if (std::filesystem::exists("/usr/bin/emerge")) {
        result.manager = "portage";
        result.count = std::stoi(exec("qlist -I | wc -l"));
    }
    else if (std::filesystem::exists("/sbin/apk") || std::filesystem::exists("/usr/bin/apk")) {
        result.manager = "apk";
        result.count = std::stoi(exec("apk info | wc -l"));
    }
    else {
        result.manager = "unknown";
        result.count = 0;
    }

    return result;
}

// helper function for getCpuUsg
CpuTimes getCpuTimes() {
    ifstream stats("/proc/stat");
    string line;
    CpuTimes times = {};

    if (stats.is_open())
    {
        std::getline(stats, line);
        std::sscanf(line.c_str(), "cpu %lld %lld %lld %lld %lld %lld %lld %lld",
                    &times.user, &times.nice, &times.system, &times.idle,
                    &times.iowait, &times.irq, &times.softirq, &times.steal);
    }

    return times;
}
// getCpuUsg - meh
float getCpuUsg()
{
    CpuTimes t1 = getCpuTimes();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    CpuTimes t2 = getCpuTimes();

    long long totalDelta = t2.total() - t1.total();
    long long idleDelta = t2.idleTime() - t1.idleTime();

    if (totalDelta == 0) return 0.0;

    return 100.0 * (1.0 - (float)idleDelta / totalDelta);
}
// getCpuTemp - meh
string getCpuTemp() 
{
    const string search = "/sys/class/thermal/";
    const string pref = "thermal_zone";

    for (const auto& entry : std::filesystem::directory_iterator(search))
    {
        if (!entry.is_directory()) continue;

        string dir = entry.path().filename();
        if (dir.find(pref) != 0) continue;

        string tpath = entry.path().string() + "/type";
        ifstream tfile(tpath);
        if (!tfile.is_open()) continue;

        string tstr;
        std::getline(tfile, tstr);
        tfile.close();

        string lstr = tstr;
        std::transform(lstr.begin(), lstr.end(), lstr.begin(), [](char c){ return std::tolower(c); });


        if (lstr.find("cpu") != string::npos || lstr.find("x86_pkg_temp") != string::npos)
        {
            string tempp = entry.path().string() + "/temp";
            ifstream tempf(tempp);
            if (!tempf.is_open()) continue;

            string temp_str;
            std::getline(tempf, temp_str);
            tempf.close();

            try {
                int tempget = std::stoi(temp_str);
                float temp = tempget / 1000.0f;
                std::ostringstream dumbfix;
                dumbfix << std::fixed << std::setprecision(1) << temp << "°C";
                return dumbfix.str();
                //return std::to_string(temp) + "°C";
            } catch (...) {
                return "unknown";
            }
        }
    }

    return "unknown";
}

// helper function to getCpu
string getCpuName() 
{
    ifstream cpun("/proc/cpuinfo");
    if (!cpun.is_open()) return "unknown";

    string line;
    string model;
    int cores = 0;

    while (getline(cpun, line)) {
        if (line.empty()) continue;

        auto colon_pos = line.find(':');
        if (colon_pos == string::npos) continue;

        string key = line.substr(0, colon_pos);
        string value = line.substr(colon_pos + 1);

        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (key == "model name" && model.empty()) {
            model = value;
        } 
        else if (key == "processor") {
            cores++;
        }
    }

    if (model.empty() || cores == 0) return "unknown";

    ifstream freqr("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"); // i <3 hardcoding paths
    string ghz;
    float ghzi = 0.0;

    if (freqr.is_open())
    {
        string khzs;
        std::getline(freqr, khzs);
        freqr.close();

        try {
            int khzi = std::stoi(khzs);
            ghzi = khzi / 1000000.0;
            std::ostringstream oss_freq;
            oss_freq << std::fixed << std::setprecision(2) << ghzi << "GHz";
            ghz = oss_freq.str();
        } catch (...) {
            ghz = "unknown";
        }
    } else {
        ghz = "unknown";
    }

    ostringstream oss;
    oss << model << " (" << cores << " cores) @ " << ghz;
    return oss.str();
}


// getCpu - cpu
CPUInfo getCpu() 
{
    CPUInfo c;
    c.name = getCpuName();
    //c.usage = getCpuUsg();
    c.temp = getCpuTemp();
    return c;
}


// getMemory - memory :)
string getMemory() 
{
    ifstream mem("/proc/meminfo");
    if (!mem.is_open()) return "unknown";

    string line;
    long long totalkb = 0;
    long long availkb = 0;

    while (getline(mem, line)) {
        if (line.find("MemTotal:") == 0) {
            sscanf(line.c_str(), "MemTotal: %lld kB", &totalkb);
        } else if (line.find("MemAvailable:") == 0) {
            sscanf(line.c_str(), "MemAvailable: %lld kB", &availkb);
        }

        if (totalkb > 0 && availkb > 0) break;
    }

    if (totalkb == 0) return "unknown";

    long long usedkb = totalkb - availkb;

    double gbtotal = totalkb / 1024.0 / 1024.0;
    double gbused = usedkb / 1024.0 / 1024.0;
    double perc = (double)usedkb / totalkb * 100.0;

    ostringstream oss;
    oss << std::fixed << std::setprecision(2)
        << gbused << " GiB / " << gbtotal << " GiB ("
        << GREENA << perc << "%" << RESET << ")";

    return oss.str();
}

// getSwapMaybe - get swap, maybe
string getSwapMaybe()
{
    ifstream mem("/proc/meminfo");
    if (!mem.is_open()) return "noswap";

    string line;
    long long totalkb = 0;

    while (getline(mem, line)) {
        if (line.find("SwapTotal:") == 0) {
            sscanf(line.c_str(), "SwapTotal: %lld kB", &totalkb);
            break;
        }
    }

    if (totalkb == 0) return "noswap";

    double total = totalkb / 1024.0 / 1024.0;

    ostringstream oss;
    oss << std::fixed << std::setprecision(2) << total << " GiB";

    return oss.str();
}


// helper function 4 below, THANK YOU RANDOM GUY FROM STACKOVERFLOW.
string getLocalIPThroughUDPOrSomethingIForgot() {
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    sockaddr_in loopback;

    if (sock == -1) {
        std::cerr << "Could not socket\n";
        return "unknown";
    }

    std::memset(&loopback, 0, sizeof(loopback));
    loopback.sin_family = AF_INET;
    loopback.sin_addr.s_addr = 1337;   // can be any IP address
    loopback.sin_port = htons(9);      // using debug port

    if (connect(sock, reinterpret_cast<sockaddr*>(&loopback), sizeof(loopback)) == -1) {
        close(sock);
        //std::cerr << "Could not connect\n";
        return "unknown";
    }

    socklen_t addrlen = sizeof(loopback);
    if (getsockname(sock, reinterpret_cast<sockaddr*>(&loopback), &addrlen) == -1) {
        close(sock);
        //std::cerr << "Could not getsockname\n";
        return "unknown";
    }

    close(sock);

    char buf[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &loopback.sin_addr, buf, INET_ADDRSTRLEN) == 0x0) {
        //std::cerr << "Could not inet_ntop\n";
        return "unknown";
    } else {
        return string(buf);
    }
}


// getNiceDriveListBecauseWeShouldAllHaveOurHappyMoments - get every drive in system and their size and if they are removable or not
vector<DriveInfo> getNiceDriveListBecauseWeShouldAllHaveOurHappyMoments() 
{
    vector<DriveInfo> result;

    for (const auto& entry : std::filesystem::directory_iterator("/sys/block")) {
       string diskName = entry.path().filename();

        // Skip the f8key f8ke f8kes ::::)
        if (diskName.find("loop") == 0 || diskName.find("ram") == 0 || diskName.find("dm-") == 0) continue;

        DriveInfo drive;

        string sizeStr = readFileTrim("/sys/block/" + diskName + "/size");
        int sectSize = stoi(readFileTrim("/sys/block/" + diskName + "/queue/logical_block_size"));
        if (!sizeStr.empty()) 
        {
            unsigned long long sectors = std::stoull(sizeStr);
            unsigned long long bytes = sectors * static_cast<unsigned long long>(sectSize); 
            drive.capacity = static_cast<float>(bytes) / 1073741824.0f; // magic number!!
        } else drive.capacity = 0.0f;

        drive.model = readFileTrim("/sys/block/" + diskName + "/device/model");
        string remStr = readFileTrim("/sys/block/" + diskName + "/removable");
        drive.physical = (remStr != "1");
        string typeStr = (readFileTrim("/sys/block/" + diskName + "/queue/rotational") == "0") ? "SSD" : "HDD";
        drive.type = typeStr;

        result.push_back(drive);
    }

    return result;
}

string getUsageRoot()
{
    struct statvfs stat;

    if (statvfs("/", &stat) != 0) return "unknown";

    unsigned long long total = stat.f_blocks * stat.f_frsize;
    unsigned long long free = stat.f_bfree * stat.f_frsize;
    unsigned long long used = total - free;

    float totalGB = static_cast<float>(total) / (1024.0f * 1024.0f * 1024.0f);
    float usedGB = static_cast<float>(used) / (1024.0f * 1024.0f * 1024.0f);
    float percent = (total == 0) ? 0.0f : (used * 100.0f / total);
    string fstype = exec("findmnt -no FSTYPE /");
    if (fstype.empty()) return "unknown";
    ostringstream oss;
    oss << std::fixed << std::setprecision(2)
        << usedGB << " GiB / " << totalGB << " GiB (" << GREENA << percent << "%" << RESET <<  ")";
    if (fstype != "unknown") {
        oss << " - " << fstype;
    }
    return oss.str();
}


// getIP - Get public and local IP and pack it up nicely.
IPPair getIP(bool doPublic)
{
    IPPair r;
    
    // Local
        r.local = getLocalIPThroughUDPOrSomethingIForgot();
    // Public
    if (doPublic)
    {
    try {
        r.publik = fetchme("http://api.ipify.org");
    } catch (...) { r.publik = "unknown"; }
    } else {r.publik="nope:3";}
    return r;
}

// getWifi - Get WiFi SSID
string getWifi()
{
    string output = exec("nmcli | grep \"connected to\"");
    if (output.empty()) return "NOWIFI";
    size_t pos = output.find("to ");
    if (pos == std::string::npos)
        return "NOWIFI";

    std::string ssid = output.substr(pos + 3);
    return ssid.empty() ? "NOWIFI" : ssid; // stupid but works :(
}