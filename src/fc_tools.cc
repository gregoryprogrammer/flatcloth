#include "fc_tools.h"
#include "components.h"

#include <sstream>
#include <string>
#include <libgen.h>
#include <cstring>
#include <sstream>
#include <cstdarg>

void fclog(LogLevel level, const char *filename, const char *function, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[64];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, 64, "%Y-%m-%d %H:%M:%S", timeinfo);

    switch (level) {
        case LogLevel::INFO:
            fprintf(stderr, ANSI_COLOR_GREEN);
            fprintf(stderr, "INFO    " LOGFORMAT, timestamp, filename, function);
            break;

        case LogLevel::DEBUG:
            fprintf(stderr, ANSI_COLOR_BLUE);
            fprintf(stderr, "DEBUG   " LOGFORMAT, timestamp, filename, function);
            break;

        case LogLevel::WARNING:
            fprintf(stderr, ANSI_COLOR_YELLOW);
            fprintf(stderr, "WARNING " LOGFORMAT, timestamp, filename, function);
            break;

        case LogLevel::ERROR:
            fprintf(stderr, ANSI_COLOR_RED);
            fprintf(stderr, "ERROR   " LOGFORMAT, timestamp, filename, function);
            break;
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
