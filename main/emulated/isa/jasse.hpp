#pragma once
//
// Jasse basic functions
//
//
//

#include <cstring>
#include <string>
#include <sstream>
#include <stdexcept>


#define __JASSE_ASSERTION


//
#if defined(_WIN32)
#define __FILENAME(x) (strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x)
#elif defined(__linux__)
#define __FILENAME(x) (strrchr(x, '/')  ? strrchr(x, '/')  + 1 : x)
#endif

#define __SHOULD_NOT_REACH_HERE(file, line, func) \
    std::ostringstream __should_not_reach_here_oss; \
    __should_not_reach_here_oss << "SHOULD_NOT_REACH_HERE - " << __FILENAME(file) << " " << func << ":" << line; \
    throw std::runtime_error(__should_not_reach_here_oss.str());

#define SHOULD_NOT_REACH_HERE \
    __SHOULD_NOT_REACH_HERE(__FILE__, __LINE__, __FUNCTION__)

//
#define __ASSERT(file, line, func) \
    std::ostringstream __should_not_reach_here_oss; \
    __should_not_reach_here_oss << "ASSERT - " << __FILENAME(file) << " " << func << ":" << line; \
    throw std::runtime_error(__should_not_reach_here_oss.str());

#if defined(__JASSE_ASSERTION)
#define ASSERT(expr) \
    if (!(expr)) {__ASSERT(__FILE__, __LINE__, __FUNCTION__);}
#else
#define ASSERT(expr)
#endif

//
#define __ASSERTM(msg, file, line, func) \
    std::ostringstream __should_not_reach_here_oss; \
    __should_not_reach_here_oss << "ASSERT: " << msg << " - " << __FILENAME(file) << " " << func << ":" << line; \
    throw std::runtime_error(__should_not_reach_here_oss.str());

#if defined(__JASSE_ASSERTION)
#define ASSERTM(expr, msg) \
    if (!(expr)) {__ASSERTM(msg, __FILE__, __LINE__, __FUNCTION__);}
#else
#define ASSERTM(expr, msg)
#endif
    
