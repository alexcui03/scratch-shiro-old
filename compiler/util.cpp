#include "util.h"

#include <string>
#include <cctype>

namespace clipcc {

std::string stringify(const std::string &str) {
    std::string res = "";
    for (char c : str) {
        switch (c) {
        case '\'':  res.append("\\\'"); break;
        case '\"':  res.append("\\\""); break;
        case '\?':  res.append("\\?"); break;
        case '\\': res.append("\\\\"); break;
        case '\a': res.append("\\a"); break;
        case '\b': res.append("\\b"); break;
        case '\f': res.append("\\f"); break;
        case '\n': res.append("\\n"); break;
        case '\r': res.append("\\r"); break;
        case '\t': res.append("\\t"); break;
        case '\v': res.append("\\v"); break;
        default: res.push_back(c);
        }
    }
    return res;
}

bool is_valid_number(const std::string &str) {
    if (str.size() == 0) return false;
    int i = 0;
    bool point = false;
    if (str[0] == '+' || str[0] == '-') ++i;
    for (; i < str.size(); ++i) {
        if (str[i] == '.') {
            if (point) return false;
            point = true;
        }
        if (!std::isdigit(str[i])) return false;
    }
    return true;
}

}
