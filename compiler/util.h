#ifndef _UTIL_H_
#define _UTIL_H_

#include <concepts>
#include <string>

namespace clipcc {

#define ENDL "\n"

template <std::convertible_to<std::string> Ty>
inline std::string &operator<<(std::string &str, const Ty &v) {
    return str.append(v);
}

std::string stringify(const std::string &str);
bool is_valid_number(const std::string &str);

}

#endif // !_UTIL_H_
