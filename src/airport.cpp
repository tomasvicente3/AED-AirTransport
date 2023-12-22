#include "airport.h"

Airport::Airport() = default;

Airport::Airport(const std::string &code): code(code) {}

Airport::Airport(std::string const &code, std::string const &name, std::string const &city, std::string const &country,
                 float const &latitude,
                 float const &longitude) {
    this->code = code;
    this->name = name;
    this->city = city;
    this->country = country;
    location.setLatitude(latitude);
    location.setLongitude(longitude);
}

const std::string &Airport::getCode() const {
    return code;
}

void Airport::setCode(const std::string &code) {
    Airport::code = code;
}

const std::string &Airport::getName() const {
    return name;
}

void Airport::setName(const std::string &name) {
    Airport::name = name;
}

const std::string &Airport::getCity() const {
    return city;
}

void Airport::setCity(const std::string &city) {
    Airport::city = city;
}

const std::string &Airport::getCountry() const {
    return country;
}

void Airport::setCountry(const std::string &country) {
    Airport::country = country;
}

const Position &Airport::getLocation() const {
    return location;
}

void Airport::setLocation(const Position &location) {
    Airport::location = location;
}

