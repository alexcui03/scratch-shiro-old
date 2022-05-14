#include "variant.h"

namespace ccvm {

variant::variant() {
    this->value_type = type::integer;
    this->value_int = 0;
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

variant::operator int() const {
    if (this->value_type & type::integer) return this->value_int;
    if (this->basic_type == type::number)
        this->value_int = this->value_double;
    else
        this->value_int = std::stoi(this->value_str);
    this->value_type |= type::integer;
    return this->value_int;
}

variant::operator double() const {
    if (this->value_type & type::number) return this->value_double;
    if (this->basic_type == type::number)
        this->value_double = this->value_int;
    else
        this->value_double = std::stod(this->value_str);
    this->value_type |= type::number;
    return this->value_double;
}

variant::operator std::string() const {
    if (this->value_type & type::string) return this->value_str;
    if (this->basic_type & type::integer)
        this->value_str = std::to_string(this->value_int);
    else
        this->value_str = std::to_string(this->value_double);
    this->value_type |= type::string;
    return this->value_str;
}

unsigned char variant::get_type() const {
    return this->basic_type;
}

}
