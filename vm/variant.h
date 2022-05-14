#ifndef _VARIANT_H_
#define _VARIANT_H_

#include <string>

namespace ccvm {

namespace type {
    constexpr unsigned char string = 0x01;
    constexpr unsigned char integer = 0x02;
    constexpr unsigned char number = 0x04;
}

class variant {
public:
    variant();
    ~variant();
    variant &operator=(const variant &v);
    variant &operator=(int v);
    variant &operator=(double v);
    variant &operator=(const char *v);
    variant &operator=(const std::string &v);
    operator int() const;
    operator double() const;
    operator std::string() const;
    unsigned char get_type() const;
private:
    mutable unsigned char value_type;
    unsigned char basic_type;

    mutable int value_int;
    mutable double value_double;
    mutable std::string value_str;
};

}

#endif // !_VARIANT_H_
