#ifndef _VARIANT_H_
#define _VARIANT_H_

#include <string>

namespace clipcc {

namespace type {
    constexpr unsigned char string = 0x01;
    constexpr unsigned char integer = 0x02;
    constexpr unsigned char number = 0x04;
    constexpr unsigned char boolean = 0x08;
}

class variant {
public:
    variant();
    variant(int v);
    variant(double v);
    variant(const char *v);
    variant(const std::string &v);
    variant(bool v);
    ~variant();
    variant &operator=(const variant &v);
    variant &operator=(int v);
    variant &operator=(double v);
    variant &operator=(const char *v);
    variant &operator=(const std::string &v);
    variant &operator=(bool v);
    operator int() const;
    operator double() const;
    operator std::string() const;
    operator bool() const;
    double operator+(double x);
    friend double operator+(double x, const variant &y);
    double operator-(double x);
    friend double operator-(double x, const variant &y);
    double operator*(double x);
    friend double operator*(double x, const variant &y);
    double operator/(double x);
    friend double operator/(double x, const variant &y);
    unsigned char type() const;
    bool is_int() const;
private:
    mutable unsigned char value_type;
    unsigned char basic_type;

    mutable bool value_bool;
    mutable int value_int;
    mutable double value_double;
    mutable std::string value_str;
};

}

#endif // !_VARIANT_H_
