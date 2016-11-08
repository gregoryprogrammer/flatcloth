#ifndef FLATCLOTH_FC_TOOLS_H
#define FLATCLOTH_FC_TOOLS_H

#include "fc_types.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define LOGFORMAT "%-16s  %-26s : %-24s  >  "

// extract relative path filename
#define __LOCALFILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

enum class LogLevel {
    INFO,
    DEBUG,
    WARNING,
    ERROR,
};

void fclog(LogLevel level, const char *filename, const char *function, const char *fmt, ...);

#define log_info(...)    do { fclog(LogLevel::INFO,    __LOCALFILENAME__, __FUNCTION__, __VA_ARGS__); } while (0)
#define log_debug(...)   do { fclog(LogLevel::DEBUG,   __LOCALFILENAME__, __FUNCTION__, __VA_ARGS__); } while (0)
#define log_warning(...) do { fclog(LogLevel::WARNING, __LOCALFILENAME__, __FUNCTION__, __VA_ARGS__); } while (0)
#define log_error(...)   do { fclog(LogLevel::ERROR,   __LOCALFILENAME__, __FUNCTION__, __VA_ARGS__); } while (0)

std::vector<std::string> split(const std::string &str, char delim);

std::string dirname(const std::string &filename);

Quad make_quad(FaceVertex fv0, FaceVertex fv1, FaceVertex fv2, FaceVertex fv3);

template<class T>
T number_cast(const std::string &str)
{
    std::istringstream iss{str};
    T number;
    iss >> number;
    return number;
}

#endif // FLATCLOTH_FC_TOOLS_H
