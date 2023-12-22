#ifndef AIRLINE_H
#define AIRLINE_H

#include <string>
#include <unordered_set>

class Airline {
private:
    std::string code;
    std::string name;
    std::string callsign;
    std::string country;
public:
    Airline(std::string code, std::string name, std::string callsign, std::string country);

    explicit Airline(std::string code);

    const std::string &getCode() const;

    void setCode(const std::string &code);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getCallsign() const;

    void setCallsign(const std::string &callsign);

    const std::string &getCountry() const;

    void setCountry(const std::string &country);
};

struct AirlineHash {
    std::size_t operator()(const Airline &airline) const {
        return std::hash<std::string>()(airline.getCode());
    }
};

struct AirlineEquals {
    bool operator()(const Airline &airline1, const Airline &airline2) const {
        return airline1.getCode() == airline2.getCode();
    }
};

typedef std::unordered_set<Airline, AirlineHash, AirlineEquals> airlineTable;

#endif