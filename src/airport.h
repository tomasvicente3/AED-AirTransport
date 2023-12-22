#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "position.h"

class Airport {
private:
    std::string code;
    std::string name;
    std::string city;
    std::string country;
    Position location = Position(0, 0);
public:
    Airport();

    explicit Airport(const std::string &code);

    Airport(const std::string &code, const std::string &name, const std::string &city, const std::string &country,
            const float &latitude, const float &longitude);

    const std::string &getCode() const;

    void setCode(const std::string &code);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getCity() const;

    void setCity(const std::string &city);

    const std::string &getCountry() const;

    void setCountry(const std::string &country);

    const Position &getLocation() const;

    void setLocation(const Position &location);
};

struct AirportHash {
    std::size_t operator()(const Airport &airport) const {
        return std::hash<std::string>()(airport.getCode());
    }
};

struct AirportEquals {
    bool operator()(const Airport &airport1, const Airport &airport2) const {
        return airport1.getCode() == airport2.getCode();
    }
};

typedef std::unordered_set<Airport, AirportHash, AirportEquals> airportTable;

template<typename T>
using airportMap = std::unordered_map<Airport, T, AirportHash, AirportEquals>;

#endif