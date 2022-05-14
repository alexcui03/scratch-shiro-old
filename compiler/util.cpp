#include "util.h"

#include <string>

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

}
