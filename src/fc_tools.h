#ifndef FLATCLOTH_FC_TOOLS_H
#define FLATCLOTH_FC_TOOLS_H

#include "fc_types.h"

#include <cstdio>
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

#define FCLOG_INFO 0
#define FCLOG_DEBUG 1
#define FCLOG_WARNING 2
#define FCLOG_ERROR 3

void fclog(uint32_t level, const char *module, const char *fmt, ...);

#define log_info(module, ...)    fclog(FCLOG_INFO, module, __VA_ARGS__);
#define log_debug(module, ...)   fclog(FCLOG_DEBUG, module, __VA_ARGS__);
#define log_warning(module, ...) fclog(FCLOG_WARNING, module, __VA_ARGS__);
#define log_error(module, ...)   fclog(FCLOG_ERROR, module, __VA_ARGS__);

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
