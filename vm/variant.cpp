#include "variant.h"

#include <cmath>

namespace ccvm {

variant::variant() {
    this->value_type = type::integer;
    this->value_int = 0;
}

variant::variant(int v) {
    this->value_type = this->basic_type = type::integer;
    this->value_int = v;
}

variant::variant(double v) {
    this->value_type = this->basic_type = type::number;
    this->value_double = v;
}

variant::variant(const char *v) {
    this->value_type = this->basic_type = type::string;
    this->value_str = v;
}

variant::variant(const std::string &v) {
    this->value_type = this->basic_type = type::string;
    this->value_str = v;
}

variant::variant(bool v) {
    this->value_type = this->basic_type = type::boolean;
    this->value_bool = v;
}

variant::~variant() {}

variant &variant::operator=(const variant &v) {
    this->value_type = this->basic_type = v.value_type;
}

variant &variant::operator=(int v) {
    this->value_type = this->basic_type = type::integer;
    this->value_int = v;
}

variant &variant::operator=(double v) {
    this->value_type = this->basic_type = type::number;
    this->value_double = v;
}

variant &variant::operator=(const char *v) {
    this->value_type = this->basic_type = type::string;
    this->value_str = v;
}

variant &variant::operator=(const std::string &v) {
    this->value_type = this->basic_type = type::string;
    this->value_str = v;
}

variant &variant::operator=(bool b) {
    this->value_type = this->basic_type = type::boolean;
    this->value_bool = b;
}

variant::operator int() const {
    if (this->value_type & type::integer) return this->value_int;
    if (this->basic_type == type::number)
        this->value_int = this->value_double;
    else if (this->basic_type == type::boolean)
        this->value_int = this->value_bool;
    else
        this->value_int = std::stoi(this->value_str);
    this->value_type |= type::integer;
    return this->value_int;
}

variant::operator double() const {
    if (this->value_type & type::number) return this->value_double;
    if (this->basic_type == type::integer)
        this->value_double = this->value_int;
    else if (this->basic_type == type::boolean)
        this->value_double = this->value_bool;
    else
        this->value_double = std::stod(this->value_str);
    this->value_type |= type::number;
    return this->value_double;
}

variant::operator std::string() const {
    if (this->value_type & type::string) return this->value_str;
    if (this->basic_type == type::integer)
        this->value_str = std::to_string(this->value_int);
    else if (this->basic_type == type::boolean)
        this->value_str = this->value_bool ? "true" : "false";
    else
        this->value_str = std::to_string(this->value_double);
    this->value_type |= type::string;
    return this->value_str;
}

variant::operator bool() const {
    if (this->value_type & type::boolean) return this->value_bool;
    if (this->basic_type == type::string)
        this->value_bool = !(this->value_str == "false" || this->value_str == "" || this->value_str == "0");
    else if (this->basic_type == type::integer)
        this->value_bool = this->value_int;
    else
        this->value_bool = this->value_double;
    this->value_type |= type::boolean;
    return this->value_bool;
}

unsigned char variant::type() const {
    return this->basic_type;
}

bool variant::isInt() const {
    if (this->basic_type == type::integer) return true;
    if (this->basic_type == type::number) {
        // in scratch, NaN is always considered as an integer.
        if (std::isnan(this->value_double)) return true;
        // integers mod 1 is always 0
        return !std::fmod(this->value_double, 1.0);
    }
    if (this->basic_type == type::boolean) return true;
    if (this->basic_type == type::string)
        return this->value_str.find('.') == std::string::npos;
    return false;
}

}
