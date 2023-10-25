#ifndef KORS_HELPFUL_H
#define KORS_HELPFUL_H

#include <string>

namespace kors::logger {
struct Helpful {
    static std::string className(const std::string& sig);
    static std::string methodName(const std::string& sig);
};
}

#endif // KORS_HELPFUL_H
