#include "fc_tools.h"
#include "components.h"

#include <sstream>
#include <string>
#include <libgen.h>
#include <cstring>
#include <sstream>
#include <cstdarg>

#define LOGFORMAT "%s  %-16s  > "

void fclog(uint32_t level, const char *module, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[64];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, 64, "%Y-%m-%d %H:%M:%S", timeinfo);


    if (level == 0 ) {
        fprintf(stderr, "INFO    " LOGFORMAT, timestamp, module);
    } else if (level == 1 ) {
        fprintf(stderr, ANSI_COLOR_BLUE);
        fprintf(stderr, "DEBUG   " LOGFORMAT, timestamp, module);
    } else if (level == 2 ) {
        fprintf(stderr, ANSI_COLOR_YELLOW);
        fprintf(stderr, "WARNING " LOGFORMAT, timestamp, module);
    } else if (level == 3 ) {
        fprintf(stderr, ANSI_COLOR_RED);
        fprintf(stderr, "ERROR   " LOGFORMAT, timestamp, module);
    }

    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n" ANSI_COLOR_RESET);

    va_end(args);
}

std::vector<std::string> split(const std::string &str, char delim)
{
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string dirname(const std::string &filename)
{
    if (filename.find_first_of("/") == std::string::npos) {
        return "./";
    } else if (filename == "/") {
        return "/";
    }

    auto ret = filename.substr(0, filename.find_last_of("/") + 1);
    return ret;
}

Quad make_quad(FaceVertex fv0, FaceVertex fv1, FaceVertex fv2, FaceVertex fv3)
{
    auto face_0 = Face{fv0, fv1, fv2};
    auto face_1 = Face{fv2, fv1, fv3};

    return Quad{face_0, face_1};
}
