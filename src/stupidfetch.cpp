#include "utils.hpp"
#include "weather.hpp"
#include "offload.hpp"
#include "config.hpp"
#include "gpu.hpp"
// TODO: Make this better

#define ar std::string
using ConfigMap = std::unordered_map<std::string,std::string>;

int main(int argc, char* argv[]) {
    const char* home_env = getenv("HOME");
    if (!home_env)
    {
        std::cerr << "ERROR: No HOME environment variable set. Something is really wrong with your installation." << std::endl;
        return 1;
    }
    if (!std::filesystem::exists(ar(home_env)+"/.config/stupidfetch"))
    {
        if(!std::filesystem::create_directories(ar(home_env)+"/.config/stupidfetch"))
        {
            std::cerr << "ERROR: Cannot create config directory! Am I stupid?" << std::endl;
            return 1;
        }
    }
    std::string HOME = ar(home_env) + "/.config/stupidfetch/";
    if (!std::filesystem::exists(HOME+"default.conf")){
        std::ofstream configer(HOME+"default.conf");
        if(configer)
        {
            configer <<
            "# Stupidfetch config file\n" << 
            "# It's very basic, do not use spaces between the pairs kthx.\n" <<
            "# COLOR is the accent color.\n" <<
            "# Valid values are RED, GREEN, YELLOW, BLUE, CYAN, PURPLE, WHITE, DISTRO or any integer between 0-255.\n" <<
            "COLOR=DISTRO\n" <<
            "DISPLAY_HOSTNAME_USERNAME=YES\n" <<
            "DISPLAY_OS=YES\n" << 
            "DISPLAY_PRODUCTNAME=YES\n" <<
            "DISPLAY_KERNEL=YES\n" <<
            "DISPLAY_UPTIME=YES\n" <<
            "DISPLAY_PACKAGES=YES\n" <<
            "DISPLAY_PROCESSES=YES\n" <<
            "DISPLAY_SHELL=YES\n" <<
            "DISPLAY_DISPLAYS=YES\n" <<
            "DISPLAY_DE=YES\n" <<
            "DISPLAY_WM=YES\n" <<
            "DISPLAY_TERMINAL_SIZE=NO\n" <<
            "DISPLAY_TERMINAL=YES\n" <<
            "DISPLAY_CPU=YES\n" <<
            "DISPLAY_CPU_TEMP=YES\n" <<
            "DISPLAY_GPU=YES\n" <<
            "# By default the GPU lookup uses lspci which is kinda slow. Changing this\n" <<
            "# Makes the info less accurate, but is slightly faster.\n" <<
            "GPU_FAST_LOOK=NO\n" <<
            "# Memory also includes swap. If swap doesn't exist it doesn't display it.\n" <<
            "DISPLAY_MEMORY=YES\n" <<
            "DISPLAY_ROOTDISK=YES\n" <<
            "DISPLAY_ALLDISK=NO\n" <<
            "DISPLAY_IP_LOCAL=YES\n" <<
            "DISPLAY_IP_PUBLIC=NO\n" <<
            "DISPLAY_WIRELESS=NO\n" <<
            "DISPLAY_BATTERY=YES\n" <<
            "DISPLAY_WEATHER=NO\n" <<
            "DISPLAY_LOCALE=YES\n";
            configer.close();
        }
        else
        {
            std::cerr << "ERROR: Cannot create config file! Am I stupid?" << std::endl;
            return 1;
        }
    }
    std::string ConfigLoadPath = HOME + "default.conf";
    if (argc >= 2) 
    {
        if (ar(argv[1]) == "-v" || ar(argv[1]) == "--version")
        {
            std::cout << "stupidfetch v1.2" << std::endl <<
            "by Halved :3" << std::endl;
            return 0;
        }
        if (ar(argv[1]) == "-h" || ar(argv[1]) == "--help")
        {
            std::cout << "Stupidfetch is a very inefficient neofetch-like tool for fetching and displaying system information." << std::endl;
            std::cout << UNDERL  << "Usage:" << RESET << BOLD << " sfetch" << RESET << ITALIC << " <options>" << RESET << std::endl;
            std::cout << UNDERL << BOLD << "Options:" << RESET << std::endl;
            std::cout <<BOLD<< "  -h, --help"<<RESET<<"                                  Display this help message." << std::endl;
            std::cout <<BOLD<< "  -v, --version"<<RESET<<"                               Show the currently installed version of stupidfetch." << std::endl;
            std::cout <<BOLD<< "  -c, --config"<<RESET<<"                                Use a different config file than the default one." << std::endl;
            std::cout << "                                              By default uses the configuration file at $HOME/stupidfetch/default.conf" << std::endl;
            return 0;
        }
        if (ar(argv[1]) == "-c" || ar(argv[1]) == "--config")
        {
            if (argc >= 3)
            {
                ConfigLoadPath = HOME + ar(argv[2]) + ".conf";
            } 
            else 
            {
                std::cout << "Error! Did not specify config." << std::endl;
                return 1;
            }
        }
    }
    ConfigMap c = LoadConfig(ConfigLoadPath);
    std::string DEFCOLOR = CYAN;
    std::string GETCOLOR = GetStrConfig(c,"COLOR");
    if (GETCOLOR == "RED")          DEFCOLOR = RED;
    else if (GETCOLOR == "BLUE")    DEFCOLOR = BLUE;
    else if (GETCOLOR == "YELLOW")  DEFCOLOR = YELLOW;
    else if (GETCOLOR == "GREEN")   DEFCOLOR = GREEN;
    else if (GETCOLOR == "WHITE")   DEFCOLOR = WHITE;
    else if (GETCOLOR == "PURPLE")  DEFCOLOR = PURPLE;
    else if (GETCOLOR == "CYAN")    DEFCOLOR = CYAN;
    else if (GETCOLOR == "DISTRO")  DEFCOLOR = distroColor();
    else                            DEFCOLOR = colormein(GETCOLOR);
    /*std::cout << "DEBUG TEXT: \n";
    for(int i = 0; i <= 255; i++)
    {
        if (i > 15 && i % 6 == 0 && i < 232) std::cout << std::endl;
        std::cout << colormein(std::to_string(i)) << "█" << RESET;
    } std::cout << std::endl;*/
    // STUPIDFETCH BEGINS HERE
    //        /\_/\            ...
    //     = (• . •) =        ...
    //       /     \          ...

    HostPair hosty = getHostUser(); // this one is special because we need the num anyways
    if(GetConfig(c,"DISPLAY_HOSTNAME_USERNAME"))
    {
        HostPair host = getHostUser();
        std::cout << BOLD << DEFCOLOR << host.username << RESET << "@" << BOLD << DEFCOLOR <<host.hostname << RESET << std::endl;
    }
    std::cout << std::string(hosty.hostname.length()+hosty.username.length()+1, '-') << std::endl;
    
    if(GetConfig(c,"DISPLAY_OS"))
    {
        std::string osname = getOSName();
        std::cout << BOLD << DEFCOLOR << "OS" << RESET << ": " << osname << std::endl;
    }

    if(GetConfig(c,"DISPLAY_PRODUCTNAME"))
    {
        std::string productn = getProduct();
        std::cout << BOLD << DEFCOLOR << "Host" << RESET << ": " << productn << std::endl;
    }

    if(GetConfig(c,"DISPLAY_KERNEL"))
    {
        std::string kerneln = getKernel();
        std::cout << BOLD << DEFCOLOR << "Kernel" << RESET << ": " << kerneln << std::endl;
    }

    if(GetConfig(c,"DISPLAY_UPTIME"))
    {
        std::string uptimes = getUptime();
        std::cout << BOLD << DEFCOLOR << "Uptime" << RESET << ": " << uptimes << std::endl;
    }
    
    if(GetConfig(c,"DISPLAY_PACKAGES"))
    {
        PackInfo puka = getPkgManCnt();
        std::cout << BOLD << DEFCOLOR << "Packages" << RESET << ": " << puka.count << " (" << puka.manager << ")"<< std::endl;
    }

    if(GetConfig(c,"DISPLAY_PROCESSES"))
    {
        int procm = getProcessNum();
        std::cout << BOLD << DEFCOLOR << "Processes" << RESET << ": " << procm << std::endl;
    }


    if(GetConfig(c,"DISPLAY_SHELL"))
    {
        std::string shelll = getShell();
        std::cout << BOLD << DEFCOLOR << "Shell" << RESET << ": " << shelll << std::endl;
    }

    if(GetConfig(c,"DISPLAY_TERMINAL"))
    {
        std::string termy = getTerminal();
        std::cout << BOLD << DEFCOLOR << "Terminal" << RESET << ": " << termy << std::endl;
    }

    if(GetConfig(c,"DISPLAY_DISPLAYS"))
    {
        std::vector<DisplayInfo> disps = getDisp();
        for(DisplayInfo d: disps)
        {
        std::cout << BOLD << DEFCOLOR << "Display (" << d.name << ")" << RESET << ": " << d.reshz << std::endl;
        }
    }

    if(GetConfig(c,"DISPLAY_DE"))
    {
        SessionInfo dewm = getDeWm();
        std::cout << BOLD << DEFCOLOR << "DE" << RESET << ": " << dewm.denv << std::endl;
    }

    if(GetConfig(c,"DISPLAY_WM"))
    {
        SessionInfo dewm = getDeWm();
        std::cout << BOLD << DEFCOLOR << "WM" << RESET << ": " << dewm.wmgr << std::endl;
    }

    if(GetConfig(c,"DISPLAY_TERMINAL_SIZE"))
    {
        std::string tern = getTermSize();
        std::cout << BOLD << DEFCOLOR << "Terminal size" << RESET << ": " << tern << std::endl;
    }

    if(GetConfig(c,"DISPLAY_CPU"))
    {
        CPUInfo crisp = getCpu();
        std::cout << BOLD << DEFCOLOR << "CPU" << RESET << ": " << crisp.name << std::endl;
    }

    if(GetConfig(c,"DISPLAY_CPU_TEMP"))
    {
        CPUInfo crisp = getCpu();
        std::cout << BOLD << DEFCOLOR << "CPU Temp" << RESET << ": " << crisp.temp << std::endl;
    }

    if(GetConfig(c,"DISPLAY_GPU"))
    {
        std::vector<std::string> ultra = getGpu(GetConfig(c,"GPU_FAST_LOOK"));
        for(int i=0; i<(int)(ultra.size());i++)
        {
        std::cout << BOLD << DEFCOLOR << "GPU " << i+1 << RESET << ": " << ultra[i] << std::endl;
        }
    }

    if(GetConfig(c,"DISPLAY_MEMORY"))
    {
        std::string memy = getMemory();
        std::cout << BOLD << DEFCOLOR << "Memory" << RESET << ": " << memy << std::endl;
        std::string possibleSwap = getSwapMaybe();
        if(!(possibleSwap == "noswap"))
        {
            std::cout << BOLD << DEFCOLOR << "Swap" << RESET << ": " << possibleSwap << std::endl;
        }
    }

    if(GetConfig(c,"DISPLAY_ROOTDISK"))
    {
        std::string usedr = getUsageRoot();
        std::cout << BOLD << DEFCOLOR << "Disk (/)" << RESET << ": " << usedr << std::endl;
    }

    if(GetConfig(c,"DISPLAY_ALLDISK"))
    {
        std::vector<DriveInfo> wheels = getNiceDriveListBecauseWeShouldAllHaveOurHappyMoments();
        for (DriveInfo d: wheels)
        {
        std::cout << BOLD << DEFCOLOR << (d.physical ? "Physical" : "Removable")<< " Disk (" << d.model << ")"<< RESET << ": " << std::fixed <<  std::setprecision(2) << d.capacity << " GiB [" << d.type << "]"<< std::endl;
        }
    }

    if(GetConfig(c,"DISPLAY_IP_LOCAL"))
    {
        IPPair rope = getIP();
        std::cout << BOLD << DEFCOLOR << "Local IP" << RESET << ": " << rope.local << std::endl;
    }

    if(GetConfig(c,"DISPLAY_IP_PUBLIC"))
    {
        IPPair rope = getIP(true);
        std::cout << BOLD << DEFCOLOR << "Public IP" << RESET << ": " << rope.publik << std::endl;
    }

    if(GetConfig(c,"DISPLAY_WIRELESS"))
    {
        std::string tech = getWifi();
        std::cout << BOLD << DEFCOLOR << "Wireless" << RESET << ": " << tech << std::endl;
    }

    if(GetConfig(c,"DISPLAY_WEATHER"))
    {
        std::string heat = getLocalWeather();
        std::cout << BOLD << DEFCOLOR << "Weather" << RESET << ": " << heat << std::endl;
    }

    if(GetConfig(c,"DISPLAY_BATTERY"))
    {
        BatteryInfo armed = getBattery();
        std::cout << BOLD << DEFCOLOR << "Battery" << RESET << ": " << GREEN << armed.capacity << "%"<< RESET << " [" << armed.status << "]"<< std::endl;
    }

    if(GetConfig(c,"DISPLAY_LOCALE"))
    {
        std::string useless = getLocale();
        std::cout << BOLD << DEFCOLOR << "Locale" << RESET << ": " << useless << std::endl;
    }


    return 0;
}
