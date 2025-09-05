#include "weather.hpp"
#include "curler.hpp"

// a simple function? in my stupidfetch?
// absolute cinema :D
std::string getLocalWeather()
{
    return fetchme("https://wttr.in/?format=%l+%C+%t");
}